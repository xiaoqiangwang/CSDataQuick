#ifndef ENUMS_H
#define ENUMS_H

#include <QObject>

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

class Direction : public QObject
{
    Q_OBJECT
    Q_ENUMS(DirectionEnum)
public:
    enum DirectionEnum {
        Up = 0,
        Right,
        Down,
        Left
    };
private:
    Direction(QObject*);
    Direction();
    Direction(const Direction&);
    Direction& operator=(const Direction&);
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

class LimitsSource : public QObject
{
    Q_OBJECT
    Q_ENUMS(LimitsSourceEnum)
public:
    enum LimitsSourceEnum {
        Channel = 0,
        Default,
        User,
        Unused
    };
private:
    LimitsSource(QObject*);
    LimitsSource();
    LimitsSource(const LimitsSource&);
    LimitsSource& operator=(const LimitsSource&);
};

class RelatedDisplayVisual : public QObject
{
    Q_OBJECT
    Q_ENUMS(RelatedDisplayVisualEnum)
public:
    enum RelatedDisplayVisualEnum {
        Menu = 0,
        Row,
        Column,
        Hidden
    };
private:
    RelatedDisplayVisual(QObject*);
    RelatedDisplayVisual();
    RelatedDisplayVisual(const RelatedDisplayVisual&);
    RelatedDisplayVisual& operator=(const RelatedDisplayVisual&);
};

class LabelStyle : public QObject
{
    Q_OBJECT
    Q_ENUMS(LabelStyleEnum)
public:
    enum LabelStyleEnum {
        Frame = 0,
        None,
        Outline,
        Limits,
        Channel
    };
private:
    LabelStyle(QObject*);
    LabelStyle();
    LabelStyle(const LabelStyle&);
    LabelStyle& operator=(const LabelStyle&);
};

class TimeUnit : public QObject
{
    Q_OBJECT
    Q_ENUMS(TimeUnitEnum)
public:
    enum TimeUnitEnum {
        MilliSecond = 0,
        Second,
        Minute,
    };
private:
    TimeUnit(QObject*);
    TimeUnit();
    TimeUnit(const TimeUnit&);
    TimeUnit& operator=(const TimeUnit&);
};

class PlotStyle : public QObject
{
    Q_OBJECT
    Q_ENUMS(PlotStyleEnum)
public:
    enum PlotStyleEnum {
        Point = 0,
        Line,
        FillUnder
    };
private:
    PlotStyle(QObject*);
    PlotStyle();
    PlotStyle(const PlotStyle&);
    PlotStyle& operator=(const PlotStyle&);
};

#endif // ENUMS_H
