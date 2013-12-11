#ifndef UTILS_H
#define UTILS_H

#include <QObject>
#include <QVariant>

class Utils : public QObject
{
    Q_OBJECT
public:
    explicit Utils(QObject *parent = 0);

    Q_INVOKABLE double calculate(QString expr, QVariantList input);
    Q_INVOKABLE bool execute(QString program);
    Q_INVOKABLE QString format(QString format, double number);
signals:

public slots:

};

class TextFormat : public QObject
{
    Q_OBJECT
    Q_ENUMS(TextFormatEnum)
public:
    enum TextFormatEnum {
        Decimal = 0,
        Exponential,
        EngNotation,
        Compact,
        Truncated,
        Hexadecimal,
        Octal,
        String,
        Sexagesiaml,
        SexagesimalDMS,
        SexagesimalHMS
    };
private:
    TextFormat(QObject*);
    TextFormat();
    TextFormat(const TextFormat&);
    TextFormat& operator=(const TextFormat&);
};

class ColorMode : public QObject
{
    Q_OBJECT
    Q_ENUMS(ColorModeEnum)
public:
    enum ColorModeEnum {
        Static = 0,
        Alarm,
        Discrete
    };
private:
    ColorMode(QObject*);
    ColorMode();
    ColorMode(const ColorMode&);
    ColorMode& operator=(const ColorMode&);
};

class EdgeStyle : public QObject
{
    Q_OBJECT
    Q_ENUMS(EdgeStyleEnum)
public:
    enum EdgeStyleEnum {
        Solid = 0,
        Dash
    };
private:
    EdgeStyle(QObject*);
    EdgeStyle();
    EdgeStyle(const EdgeStyle&);
    EdgeStyle& operator=(const EdgeStyle&);
};

class FillStyle : public QObject
{
    Q_OBJECT
    Q_ENUMS(FillStyleEnum)
public:
    enum FillStyleEnum {
        Solid = 0,
        Outline
    };
private:
    FillStyle(QObject*);
    FillStyle();
    FillStyle(const FillStyle&);
    FillStyle& operator=(const FillStyle&);
};

class VisibilityMode : public QObject
{
    Q_OBJECT
    Q_ENUMS(VisibilityModeEnum)
public:
    enum VisibilityModeEnum {
        Static = 0,
        IfNotZero,
        IfZero,
        Calc
    };
private:
    VisibilityMode(QObject*);
    VisibilityMode();
    VisibilityMode(const VisibilityMode&);
    VisibilityMode& operator=(const VisibilityMode&);
};

#endif // UTILS_H
