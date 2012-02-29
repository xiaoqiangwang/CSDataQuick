#ifndef PVOBJECT_H
#define PVOBJECT_H

#include <QObject>
#include <QVariant>

#include <cadef.h>

#define ENTER_CA \
    if(m_cac)\
    {ca_attach_context(m_cac);{

#define LEAVE_CA \
    }}


class PvObject : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariant value READ value WRITE setValue NOTIFY valueChanged)
    Q_PROPERTY(QVariant channel  READ channel  WRITE setChannel)
    Q_PROPERTY(QVariant severity READ severity)

public:
    explicit PvObject(QObject *parent = 0);
    ~PvObject();

    /* context */
    static long init_ca()
    {
        int status;

        status = ca_context_create(ca_enable_preemptive_callback);
        if(status != ECA_NORMAL)
            return(CA_EXTRACT_MSG_NO(status));

        m_cac = ca_current_context();

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


    long connect(const char *name);
    long disconnect();
    long ensureConnection();


    long monitor(unsigned long);
    long unmonitor();
    static void getCallback(struct event_handler_args);

    void setValue(const QVariant val);
    QVariant value();

    void setChannel(const QVariant name);
    QVariant channel();

    QVariant severity() {return _severity;}

signals:
    void valueChanged();

public slots:


private:
    QVariantList _value;
    QVariant _name;

    // pv alarm info
    QVariant _status;			// alarm status
    QVariant _severity;			// alarm severity
    QVariant _sec;				// time stamp - seconds since Midnight Jan.1, 1990
    QVariant _nsec;			// time stamp - nano seconds within second

    // pv control info
    QVariant _units; // units

    QVariant _nostr;		// no. of state strings
    QVariantList _strs;     // state strings separated by ;

    QVariant _precision;	// precision for float and double type

    QVariant _updisplim;  // upper display limit
    QVariant _lodisplim;  // lower display limit

    QVariant _upalrmlim;  // upper alarm   limit
    QVariant _loalrmlim;  // lower alarm   limit

    QVariant _upwarnlim;  // upper warn    limit
    QVariant _lowarnlim;	 // lower warn    limit

    QVariant _upctrllim;  // upper control limit
    QVariant _loctrllim;  // lower control limit


    chid _chid;
    evid _evid;
    bool _getCallbacked;
    static struct ca_client_context *m_cac;
};

#endif // PVOBJECT_H
