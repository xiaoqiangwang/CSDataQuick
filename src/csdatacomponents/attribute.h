#ifndef ATTRIBUTE_H
#define ATTRIBUTE_H

#include <QObject>
#include "enums.h"

class DynamicAttributeBase : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString visibilityCalc MEMBER _visibilityCalc NOTIFY visibilityCalcChanged)
    Q_PROPERTY(ColorMode::ColorModeEnum colorMode MEMBER _colorMode NOTIFY colorModeChanged)
    Q_PROPERTY(VisibilityMode::VisibilityModeEnum visibilityMode MEMBER _visibilityMode NOTIFY visibilityModeChanged)
    Q_PROPERTY(QString channel MEMBER _channel NOTIFY channelChanged)
    Q_PROPERTY(QString channelB MEMBER _channelB NOTIFY channelChanged)
    Q_PROPERTY(QString channelC MEMBER _channelC NOTIFY channelChanged)
    Q_PROPERTY(QString channelD MEMBER _channelD NOTIFY channelChanged)

public:
    explicit DynamicAttributeBase(QObject *parent = 0);

signals:
    void channelChanged();
    void colorModeChanged();
    void visibilityModeChanged();
    void visibilityCalcChanged();

public slots:

private:
    QString _channel;
    QString _channelB;
    QString _channelC;
    QString _channelD;
    ColorMode::ColorModeEnum _colorMode;
    VisibilityMode::VisibilityModeEnum _visibilityMode;
    QString _visibilityCalc;
};

class LimitsBase : public QObject
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
    explicit LimitsBase(QObject *parent = 0);

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
