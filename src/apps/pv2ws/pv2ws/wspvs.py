import threading
import time

from CaChannel import ca, CaChannel


class Channel:
    def __init__(self, name, connectCallback, monitorCallback):
        self.connectCallback = connectCallback
        self.monitorCallback = monitorCallback
        self.value = {}
        self.connected = False

        self.chan = CaChannel()
        self.chan.search_and_connect(name, self._connCallback)

    def close(self):
        self.chan.clear_channel()
        self.connectCallback = None
        self.monitorCallback = None

    def put(self, value):
        self.chan.array_put(value)

    def _connCallback(self, epicsArgs, _):
        if epicsArgs[1] == ca.CA_OP_CONN_UP:
            self.chan.array_get_callback(
                ca.dbf_type_to_DBR_CTRL(self.chan.field_type()),
                0,
                self._propertyCallback)
        else:
            self.connected = False
            if self.connectCallback:
                self.connectCallback(False)

    def _propertyCallback(self, epicsArgs, _):
        self.value['ftype'] = self.chan.field_type()
        self.value['count'] = self.chan.element_count()
        self.value['display'] = {}
        self.value['display']['units'] = epicsArgs.get('pv_units', '')
        self.value['display']['precision'] = epicsArgs.get('pv_precision', 0)

        choices =  epicsArgs.get('pv_statestrings', [])
        if choices:
            self.value['value'] = {
                'index': epicsArgs['pv_value'],
                'choices': choices
            }
        else:
            self.value['value'] = epicsArgs['pv_value']

        self.value['range'] = (epicsArgs.get('pv_lodisplim', 0), epicsArgs.get('pv_hidisplim', 0))
        self.chan.add_masked_array_event(
            ca.dbf_type_to_DBR_TIME(self.chan.field_type()),
            0,
            ca.DBE_VALUE | ca.DBE_ALARM,
            self._monitorCallback)

    def _monitorCallback(self, epicsArgs, _):
        if isinstance(self.value['value'], dict):
            self.value['value']['index'] = epicsArgs['pv_value']
        else:
            self.value['value'] = epicsArgs['pv_value']
        self.value['alarm'] = {}
        self.value['alarm']['severity'] = epicsArgs['pv_severity']
        self.value['alarm']['status'] = epicsArgs['pv_status']
        self.value['timeStamp'] = {
            'secsPastEpoch': epicsArgs['pv_seconds'] + ca.POSIX_TIME_AT_EPICS_EPOCH,
            'nanoseconds': epicsArgs['pv_nseconds']
        }
        if self.monitorCallback:
            self.monitorCallback(self.value)

        self.connected = True
        if self.connectCallback:
            self.connectCallback(True)


class PVFactory:
    def __init__(self):
        self.pvs = {}
        self.clients = {}
        self.lock = threading.Lock()
        self.tid = threading.Thread(target=self.poll, daemon=True)
        self.tid.start()

    def subscribe(self, name, client):
        self.lock.acquire()
        if name in self.pvs:
            self.clients[name].append(client)
            if self.pvs[name].connected:
                client.notify(self.pvs[name].value)
                client.notify(self.pvs[name].connected)
        else:
            self.clients[name] = [client]
            self.pvs[name] = Channel(name,
                                     connectCallback=lambda c: self.connectionCallback(name, c),
                                     monitorCallback=lambda c: self.monitorCallback(name, c))
        self.lock.release()

    def clear(self, name, client):
        pvs2remove = []
        self.lock.acquire()
        if client in self.clients[name]:
            self.clients[name].remove(client)
        if not self.clients[name]:
            pvs2remove.append(self.pvs.pop(name))
        self.lock.release()
        for pv in pvs2remove:
            pv.close()

    def remove(self, client):
        pvs2remove = []
        self.lock.acquire()
        for name in list(self.clients):
            if client in self.clients[name]:
                self.clients[name].remove(client)
            if not self.clients[name]:
                if name in self.pvs:
                    pvs2remove.append(self.pvs.pop(name))
                del self.clients[name]

        self.lock.release()
        for pv in pvs2remove:
            pv.close()

    def write(self, name, value):
        self.pvs[name].put(value)

    def monitorCallback(self, name, value):
        self.lock.acquire()
        for client in self.clients[name]:
            client.notify(value)

        # Notify connection if previously disconnected
        if not self.pvs[name].connected:
            for client in self.clients[name]:
                client.notify(True)

        self.lock.release()

    def connectionCallback(self, name, connected):
        # Only notify disconnection here
        # Notify connection in monitorCallback
        if not connected:
            self.lock.acquire()
            for client in self.clients[name]:
                client.notify(False)
            self.lock.release()

    @CaChannel.attach_ca_context
    def poll(self):
        t1 = time.time()
        while True:
            ca.poll()
            t2 = time.time()
            if t2 < t1  + 0.1:
                time.sleep(t1 + 0.1 - t2)
            t1 = time.time()
