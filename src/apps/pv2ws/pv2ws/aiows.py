"""
Serve EPICS PVs over Websocket.
"""
import asyncio
import json
import traceback

import cbor2
from aiohttp import web, WSMsgType

from .wspvs import PVFactory

pvf = PVFactory()

loop = asyncio.new_event_loop()


class Client:
    """
    Websocket client
    """
    def __init__(self, ws, cid):
        self.ws = ws
        self.cid = cid

    def __eq__(self, other):
        return self.cid == other.cid

    def __repr__(self):
        return "Client %d" % self.cid

    def __str__(self):
        return "Client %d" % self.cid

    def notify(self, notification):
        message = {
            'type': 'notification',
            'cid': self.cid
        }
        if isinstance(notification, bool):
            message['connection'] = notification
        else:
            message.update(dict(notification))
        data = cbor2.dumps(message)
        asyncio.run_coroutine_threadsafe(self.ws.send_bytes(data), loop)


async def get_stats(request):
    stats = {}
    for name, clients in pvf.clients.items():
        stats[name] = len(clients)
    return web.Response(text=json.dumps(stats))


async def handle_message(request):
    ws = web.WebSocketResponse()
    await ws.prepare(request)
    clients = []
    while True:
        try:
            msg = await ws.receive()
        except asyncio.exceptions.CancelledError:
            break
        if msg.type != WSMsgType.BINARY:
            continue
        try:
            message = cbor2.loads(msg.data)
            command = message['command']
            name = message['name']
            cid = message['cid']
            client = Client(ws, cid)
            if command == 'subscribe':
                pvf.subscribe(name, client)
                clients.append(client)
            elif command == 'unsubscribe':
                pvf.clear(name, client)
                clients.remove(client)
            elif command == "put":
                pvf.write(name, message['value'])
        except:
            traceback.print_exc()
            break
    for client in clients:
        pvf.remove(client)
    clients.clear()
    return ws

app = web.Application()
app.add_routes([
    web.get('/api/stats', get_stats),
    web.get('/api/csdata', handle_message),
])

if __name__ == '__main__':
    import argparse
    parser = argparse.ArgumentParser(
            description='process variables over websocket')

    parser.add_argument('-p', '--port', type=int, default=15232,
                        help='server port number')

    args = parser.parse_args()

    web.run_app(app, port=args.port, loop=loop)
