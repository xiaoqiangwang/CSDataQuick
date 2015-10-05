#ifndef CSDATA_H
#define CSDATA_H

#include <QDateTime>
#include <QFlags>
#include <QObject>
#include <QQmlParserStatus>
#include <QUrl>
#include <QVariant>

class CSDataEngine;

class CSDataAlarm : public QObject
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

    CSDataAlarm(QObject *parent=0);
    void setAlarm(Severity severity, int status, const QString message);
    void reset();

signals:
    void alarmChanged();

private:
    int _status;
    Severity _severity;
    QString _message;
};

class CSDataRange : public QObject
{
    Q_OBJECT

    Q_PROPERTY(double lower MEMBER _lower NOTIFY rangeChanged)
    Q_PROPERTY(double upper MEMBER _upper NOTIFY rangeChanged)
public:
    CSDataRange(QObject *parent=0);
    Q_INVOKABLE bool isValid() const;
    void setRange(double lower, double upper);
    void reset();

signals:
    void rangeChanged();

private:
    double _lower;
    double _upper;
};

class CSData : public QObject, public QQmlParserStatus
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)

    Q_PROPERTY(QString      source      READ source     WRITE setSource     NOTIFY sourceChanged)
    Q_PROPERTY(QString      host        READ host       WRITE setHost       NOTIFY hostChanged)
    Q_PROPERTY(bool         connected   READ connected  WRITE setConnected  NOTIFY connectionChanged)

    Q_PROPERTY(FieldType    fieldType   READ fieldType  WRITE setFieldType  NOTIFY fieldTypeChanged)
    Q_PROPERTY(qulonglong   count       READ count      WRITE setCount      NOTIFY countChanged)
    Q_PROPERTY(QVariant     value       READ value      WRITE setValue      NOTIFY valueChanged)

    Q_PROPERTY(int  accessRight READ accessRight  WRITE setAccessRight   NOTIFY accessRightChanged)
    Q_PROPERTY(CSDataAlarm* alarm           MEMBER _alarm NOTIFY alarmChanged)
    Q_PROPERTY(QDateTime    timeStamp   READ timeStamp  WRITE setTimeStamp  NOTIFY timeStampChanged)

    // numeric type
    Q_PROPERTY(int          precision   READ precision  WRITE setPrecision  NOTIFY precisionChanged)
    Q_PROPERTY(QString      units       READ units       WRITE setUnits      NOTIFY unitsChanged)
    Q_PROPERTY(CSDataRange* range           MEMBER _range           NOTIFY rangeChanged)
    // enum type
    Q_PROPERTY(QStringList  stateStrings READ stateStrings  WRITE setStateStrings  NOTIFY stateStringsChanged)

public:
    explicit CSData(QObject *parent=0);
    ~CSData();

    virtual void classBegin();
    virtual void componentComplete();

    enum AccessFlag {
        NoAccess    = 0x00,
        ReadAccess  = 0x01,
        WriteAccess = 0x02,
    };
    Q_ENUMS(AccessFlag)
    Q_DECLARE_FLAGS(AccessFlags, AccessFlag)

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
    Q_ENUMS(FieldType)

    QString source() const;
    void setSource(const QString source);

    QString host() const;
    Q_INVOKABLE void setHost(const QString host);

    bool connected() const;
    Q_INVOKABLE void setConnected(bool connected);

    FieldType fieldType() const;
    Q_INVOKABLE void setFieldType(CSData::FieldType fieldType);

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

    Q_INVOKABLE void setAlarm(CSDataAlarm::Severity severity, int status, const QString message);
    Q_INVOKABLE void setRange(double lower, double upper);


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
    CSDataAlarm *_alarm;        // alarm severity
    QDateTime _timeStamp;       // date time object

    int _precision;             // precision for float and double type
    QString _units;             // units
    CSDataRange*_range;         // display limit

    QStringList _stateStrings;  // state strings separated

    CSDataEngine *_engine;
};

Q_DECLARE_METATYPE(CSDataAlarm::Severity)
Q_DECLARE_METATYPE(CSData::AccessFlags)
Q_DECLARE_METATYPE(CSData::FieldType)
Q_DECLARE_OPERATORS_FOR_FLAGS(CSData::AccessFlags)

#endif // CSDATA_H
