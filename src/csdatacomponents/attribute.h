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

#endif // ATTRIBUTE_H
