#ifndef ATTRIBUTE_H
#define ATTRIBUTE_H

#include <QObject>
#include "enums.h"

class QCSData;

class DynamicAttribute : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString channel  MEMBER _channel  WRITE setChannel  NOTIFY channelChanged)
    Q_PROPERTY(QString channelB MEMBER _channelB WRITE setChannelB NOTIFY channelChanged)
    Q_PROPERTY(QString channelC MEMBER _channelC WRITE setChannelC NOTIFY channelChanged)
    Q_PROPERTY(QString channelD MEMBER _channelD WRITE setChannelD NOTIFY channelChanged)
    Q_PROPERTY(QCSData *pvA MEMBER _pChannel)
    Q_PROPERTY(QCSData *pvB MEMBER _pChannelB)
    Q_PROPERTY(QCSData *pvC MEMBER _pChannelC)
    Q_PROPERTY(QCSData *pvD MEMBER _pChannelD)
    Q_PROPERTY(bool connected  MEMBER  _connected WRITE setConnected NOTIFY connectionChanged)

    Q_PROPERTY(QString altCalc        MEMBER _altCalc        WRITE setAltCalc        NOTIFY altCalcChanged)
    Q_PROPERTY(double  altCalcResult  MEMBER _altCalcResult  NOTIFY altCalcResultChanged)

    Q_PROPERTY(VisibilityMode::VisibilityModeEnum visibilityMode MEMBER _visibilityMode WRITE setVisibilityMode NOTIFY visibilityModeChanged)
    Q_PROPERTY(QString visibilityCalc MEMBER _visibilityCalc WRITE setVisibilityCalc NOTIFY visibilityCalcChanged)
    Q_PROPERTY(bool visibility MEMBER _visibility NOTIFY visibilityChanged)

public:
    explicit DynamicAttribute(QObject *parent = 0);
    ~DynamicAttribute();

    void setChannel(QString channel);
    void setChannelB(QString channel);
    void setChannelC(QString channel);
    void setChannelD(QString channel);
    void setConnected(bool connected);

    void setVisibilityMode(VisibilityMode::VisibilityModeEnum mode);
    void setVisibilityCalc(QString calc);
    void setAltCalc(QString calc);

signals:
    void channelChanged();
    void statusChanged();
    void colorModeChanged();
    void visibilityModeChanged();
    void visibilityCalcChanged();
    void visibilityChanged();
    void connectionChanged();
    void altCalcChanged();
    void altCalcResultChanged();

public slots:
    void updateValue();
    void updateAlarm();
    void updateConnection();

protected:
    void addChannel(QString channel, QCSData **ppChannel, int mask);
    void updateCalc();

private:
    QString _channel;  QCSData *_pChannel;
    QString _channelB; QCSData *_pChannelB;
    QString _channelC; QCSData *_pChannelC;
    QString _channelD; QCSData *_pChannelD;
    ColorMode::ColorModeEnum _colorMode;
    VisibilityMode::VisibilityModeEnum _visibilityMode;
    QString _visibilityCalc;
    bool _visibility;
    QString _altCalc;
    double _altCalcResult;

    bool _connected;
    int _connectionFlag;
    int _connectionMask;

    QVariantList _args;
};

class Limits : public QObject
{
    Q_OBJECT
    Q_PROPERTY(double lopr          MEMBER _lopr        NOTIFY loprChanged)
    Q_PROPERTY(double loprChannel   MEMBER _loprChannel NOTIFY loprChannelChanged)
    Q_PROPERTY(double loprDefault   MEMBER _loprDefault NOTIFY loprDefaultChanged)
    Q_PROPERTY(double loprUser      MEMBER _loprUser    NOTIFY loprUserChanged)
    Q_PROPERTY(LimitsSource::LimitsSourceEnum loprSrc   MEMBER _loprSrc NOTIFY loprSrcChanged)

    Q_PROPERTY(double hopr          MEMBER _hopr        NOTIFY hoprChanged)
    Q_PROPERTY(double hoprChannel   MEMBER _hoprChannel NOTIFY hoprChannelChanged)
    Q_PROPERTY(double hoprDefault   MEMBER _hoprDefault NOTIFY hoprDefaultChanged)
    Q_PROPERTY(double hoprUser      MEMBER _hoprUser    NOTIFY hoprUserChanged)
    Q_PROPERTY(LimitsSource::LimitsSourceEnum hoprSrc   MEMBER _hoprSrc NOTIFY hoprSrcChanged)

    Q_PROPERTY(int prec         MEMBER _prec        NOTIFY precChanged)
    Q_PROPERTY(int precChannel  MEMBER _precChannel NOTIFY precChannelChanged)
    Q_PROPERTY(int precDefault  MEMBER _precDefault NOTIFY precDefaultChanged)
    Q_PROPERTY(int precUser     MEMBER _precUser    NOTIFY precUserChanged)
    Q_PROPERTY(LimitsSource::LimitsSourceEnum precSrc MEMBER _precSrc NOTIFY precSrcChanged)

public:
    explicit Limits(QObject *parent = 0);

signals:
    void loprChanged();
    void loprChannelChanged();
    void loprDefaultChanged();
    void loprUserChanged();
    void loprSrcChanged();

    void hoprChanged();
    void hoprChannelChanged();
    void hoprDefaultChanged();
    void hoprUserChanged();
    void hoprSrcChanged();

    void precChanged();
    void precChannelChanged();
    void precDefaultChanged();
    void precUserChanged();
    void precSrcChanged();

public slots:
    void onLoprSrcChanged();
    void onHoprSrcChanged();
    void onPrecSrcChanged();

private:
    double _lopr, _loprChannel, _loprDefault, _loprUser;
    double _hopr, _hoprChannel, _hoprDefault, _hoprUser;
    int    _prec, _precChannel, _precDefault, _precUser;
    LimitsSource::LimitsSourceEnum _loprSrc, _hoprSrc, _precSrc;
};

#endif // ATTRIBUTE_H
