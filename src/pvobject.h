#ifndef PVOBJECT_H
#define PVOBJECT_H

#include <QObject>
#include <QVariant>
#include <QStringList>
#include <QQmlParserStatus>

#include <cadef.h>
#include <alarm.h>

#define ENTER_CA \
    if(m_cac)\
    {ca_attach_context(m_cac);{

#define LEAVE_CA \
    }}


class PvObject : public QObject, public QQmlParserStatus
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)

    Q_ENUMS(FieldType)

    Q_PROPERTY(bool connected READ connected WRITE setConnected NOTIFY connectionChanged)
    Q_PROPERTY(QVariant value READ value WRITE setValue NOTIFY valueChanged)
    Q_PROPERTY(QString channel  READ channel  WRITE setChannel)
    Q_PROPERTY(bool asstring READ asstring WRITE setAsstring)
    Q_PROPERTY(bool monitor READ monitor WRITE setMonitor NOTIFY monitorChanged)

    Q_PROPERTY(int severity READ severity NOTIFY statusChanged)
    Q_PROPERTY(int status   READ status NOTIFY statusChanged)
    Q_PROPERTY(QString units    READ units)
    Q_PROPERTY(int prec     READ prec)
    Q_PROPERTY(int nostr   READ nostr)
    Q_PROPERTY(QStringList strs     READ strs)
    Q_PROPERTY(QVariant upctrllim READ upctrllim)
    Q_PROPERTY(QVariant loctrllim READ loctrllim)
    Q_PROPERTY(QVariant updisplim READ updisplim)
    Q_PROPERTY(QVariant lodisplim READ lodisplim)
    Q_PROPERTY(int type READ type)
public:
    explicit PvObject(QObject *parent = 0);
    ~PvObject();

    void classBegin();
    void componentComplete();

    /* context */
    static long init_ca()
    {
        int status;

        status = ca_context_create(ca_enable_preemptive_callback);
        if(status != ECA_NORMAL)
            return(CA_EXTRACT_MSG_NO(status));

        m_cac = ca_current_context();

        ca_add_exception_event(exception_handler, 0);

        return 0;
    }
    static long exit_ca()
    {
        ENTER_CA{
            ca_context_destroy();
            m_cac = NULL;
        }LEAVE_CA

        return 0;
    }

    static void exception_handler(exception_handler_args args)
    {
        Q_UNUSED(args);
    }

    enum FieldType {
        String = 0,
        Integer = 1,
        Short = 1,
        Float = 2,
        Enum = 3,
        Char = 4,
        Long = 5,
        Double = 6,
        Invalid = 7,
    };

    /* connection management */
    long connect(const char *name);
    long disconnect();

    long monitor(unsigned long);
    long unmonitor();

    /* channel access callbacks */
    void connectCallback(struct connection_handler_args args);
    void getCallback(struct event_handler_args);
    void monitorCallback(struct event_handler_args);

    /* property access functions */
    Q_INVOKABLE void setValue(const QVariant val);
    QVariant value() {return _value;}
    void updateValue(const QVariant val);

    void * getArrayValue(unsigned long count);

    void setChannel(const QString name) {_name = name;}
    QString channel() {return _name;}

    bool asstring() {return _asstring;}
    void setAsstring(bool asstring) {_asstring = asstring;}

    bool monitor() {return _monitor;}
    void setMonitor(bool monitor) {_monitor = monitor;}

    bool connected() {return _connected;}
    void setConnected(bool connected) {_connected = connected; emit connectionChanged();}

    void updateStatus(int severity, int status);
    int severity() {return _severity;}
    int status()   {return _status;}
    QString units()    {return _units;}
    int prec()     {return _precision;}
    int nostr()    {return _nostr;}
    QStringList strs()     {return _strs;}
    QVariant upctrllim(){return _upctrllim;}
    QVariant loctrllim(){return _loctrllim;}
    QVariant updisplim(){return _updisplim;}
    QVariant lodisplim(){return _lodisplim;}
    int type() {return _chid ? ca_field_type(_chid) : Invalid; }

signals:
    void valueChanged();
    void connectionChanged();
    void statusChanged();
    void monitorChanged();

public slots:


private:
    bool _monitor;
    bool _asstring;
    bool _connected;

    void * _array;
    QVariant _value;
    QString _name;

    // pv alarm info
    int _status;            // alarm status
    int _severity;          // alarm severity
    int _sec;               // time stamp - seconds since Midnight Jan.1, 1990
    int _nsec;              // time stamp - nano seconds within second

    // pv control info
    QString _units;         // units

    int _nostr;             // no. of state strings
    QStringList _strs;      // state strings separated by ;

    int _precision;         // precision for float and double type

    QVariant _updisplim;    // upper display limit
    QVariant _lodisplim;    // lower display limit

    QVariant _upalrmlim;    // upper alarm   limit
    QVariant _loalrmlim;    // lower alarm   limit

    QVariant _upwarnlim;    // upper warn    limit
    QVariant _lowarnlim;	// lower warn    limit

    QVariant _upctrllim;    // upper control limit
    QVariant _loctrllim;    // lower control limit

    chid _chid;
    evid _evid;
    bool _getCallbacked;
    static struct ca_client_context *m_cac;
};

#endif // PVOBJECT_H
