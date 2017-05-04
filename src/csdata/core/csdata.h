#ifndef CSDATA_H
#define CSDATA_H

#include <QDateTime>
#include <QFlags>
#include <QObject>
#include <QUrl>
#include <QVariant>
#include <QVariantMap>
#include "cs_global.h"

class QCSDataEngine;

class QCSDataSourceName
{
public:
    QCSDataSourceName(const QString source) {
        int index = source.indexOf("://");
        if (index >= 0) {
            _scheme = source.left(index);
            _name = source.mid(index+3);
        }
        else {
            _name = source;
        }
        _source = source;
    }

    QString source() const { return _source; }
    QString scheme() const { return _scheme; }
    QString name() const { return _name; }
private:
    QString _name;
    QString _scheme;
    QString _source;
};

class QCS_DLLSPEC QCSDataAlarm : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Severity severity MEMBER _severity   NOTIFY alarmChanged)
    Q_PROPERTY(int      status  MEMBER  _status     NOTIFY alarmChanged)
    Q_PROPERTY(QString  message  MEMBER _message    NOTIFY alarmChanged)
public:
    enum Severity {
        NoAlarm = 0,
        MinorAlarm,
        MajorAlarm,
        InvalidAlarm,
    };
    Q_ENUMS(Severity)

    QCSDataAlarm(QObject *parent=0);
    void setAlarm(Severity severity, int status, const QString message);
    void reset();

signals:
    void alarmChanged();

private:
    int _status;
    Severity _severity;
    QString _message;
};

class QCS_DLLSPEC QCSDataRange : public QObject
{
    Q_OBJECT

    Q_PROPERTY(double lower MEMBER _lower NOTIFY rangeChanged)
    Q_PROPERTY(double upper MEMBER _upper NOTIFY rangeChanged)
public:
    QCSDataRange(QObject *parent=0);
    Q_INVOKABLE bool isValid() const;
    void setRange(double lower, double upper);
    void reset();

signals:
    void rangeChanged();

private:
    double _lower;
    double _upper;
};

class QCS_DLLSPEC QCSData : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString      source      READ source       WRITE setSource      NOTIFY sourceChanged)
    Q_PROPERTY(QString      host        READ host         WRITE setHost        NOTIFY hostChanged)
    Q_PROPERTY(bool         connected   READ connected    WRITE setConnected   NOTIFY connectionChanged)

    Q_PROPERTY(FieldType    fieldType   READ fieldType    WRITE setFieldType   NOTIFY fieldTypeChanged)
    Q_PROPERTY(qulonglong   count       READ count        WRITE setCount       NOTIFY countChanged)
    Q_PROPERTY(QVariant     value       READ value        WRITE setValue       NOTIFY valueChanged)

    Q_PROPERTY(int  accessRight         READ accessRight  WRITE setAccessRight NOTIFY accessRightChanged)
    Q_PROPERTY(QCSDataAlarm* alarm       MEMBER _alarm     NOTIFY alarmChanged)
    Q_PROPERTY(QDateTime    timeStamp   READ timeStamp    WRITE setTimeStamp   NOTIFY timeStampChanged)

    // numeric type
    Q_PROPERTY(int          precision   READ precision    WRITE setPrecision   NOTIFY precisionChanged)
    Q_PROPERTY(QString      units       READ units        WRITE setUnits       NOTIFY unitsChanged)
    Q_PROPERTY(QCSDataRange* range       MEMBER _range     NOTIFY rangeChanged)

    // enum type
    Q_PROPERTY(QStringList  stateStrings READ stateStrings  WRITE setStateStrings  NOTIFY stateStringsChanged)

    // extra properties
    Q_PROPERTY(QVariantMap  extraProperties READ extraProperties WRITE setExtraProperties)
public:
    explicit QCSData(QObject *parent=0);
    ~QCSData();

    enum AccessFlag {
        NoAccess    = 0x00,
        ReadAccess  = 0x01,
        WriteAccess = 0x02,
    };
    Q_ENUMS(AccessFlag)
    Q_DECLARE_FLAGS(AccessFlags, AccessFlag)

    enum FieldType {
        Invalid = -1,
        String = 0,
        Enum = 1,
        Char = 2,
        Short = 3,
        Integer = 4,
        Long = 5,
        Float = 6,
        Double = 7,
    };
    Q_ENUMS(FieldType)

    QString source() const;
    void setSource(const QString source);

    QString host() const;
    Q_INVOKABLE void setHost(const QString host);

    bool connected() const;
    Q_INVOKABLE void setConnected(bool connected);

    FieldType fieldType() const;
    Q_INVOKABLE void setFieldType(QCSData::FieldType fieldType);

    qulonglong count() const;
    Q_INVOKABLE void setCount(qulonglong count);

    QVariant value() const;
    Q_INVOKABLE void setValue(const QVariant value);
    Q_INVOKABLE void updateValue(const QVariant value);

    int accessRight() const;
    Q_INVOKABLE void setAccessRight(int accessRight);

    QDateTime timeStamp() const;
    Q_INVOKABLE void setTimeStamp(const QDateTime timeStamp);

    int precision() const;
    Q_INVOKABLE void setPrecision(int precision);

    QString units() const;
    Q_INVOKABLE void setUnits(const QString units);

    QStringList stateStrings() const;
    Q_INVOKABLE void setStateStrings(const QStringList stateStrings);

    Q_INVOKABLE void setAlarm(QCSDataAlarm::Severity severity, int status, const QString message);
    Q_INVOKABLE void setRange(double lower, double upper);

    QVariantMap extraProperties() const { return _extraProperties; }
    void setExtraProperties(const QVariantMap properties) { _extraProperties = properties; }

signals:
    void sourceChanged();
    void hostChanged();
    void connectionChanged();

    void fieldTypeChanged();
    void countChanged();
    void valueChanged();

    void alarmChanged();
    void accessRightChanged();
    void timeStampChanged();

    void precisionChanged();
    void unitsChanged();
    void rangeChanged();

    void stateStringsChanged();

protected:
    void reset();

    QString  _source;           // data source
    QString _host;              // host where the data exists
    bool _connected;            // data connection status

    FieldType _fieldType;       // field type
    qulonglong _count;          // number of element
    QVariant _value;            // value

    int _accessRight;           // access right
    QCSDataAlarm *_alarm;        // alarm severity
    QDateTime _timeStamp;       // date time object

    int _precision;             // precision for float and double type
    QString _units;             // units
    QCSDataRange*_range;         // display limit

    QStringList _stateStrings;  // state strings separated

    QVariantMap _extraProperties; // extra properties

    QCSDataEngine *_engine;

    // If this is instantiated in Qml2Puppet, it will fake the connection.
    bool _inPuppet;
};

Q_DECLARE_METATYPE(QCSDataAlarm::Severity)
Q_DECLARE_METATYPE(QCSData::FieldType)
Q_DECLARE_OPERATORS_FOR_FLAGS(QCSData::AccessFlags)

#endif // CSDATA_H
