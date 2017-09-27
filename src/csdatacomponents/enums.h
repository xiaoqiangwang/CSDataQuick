#ifndef ENUMS_H
#define ENUMS_H

#include <QObject>

#define DISABLE_CREATION(CLASSNAME) \
    CLASSNAME();\
    CLASSNAME(QObject*);\
    CLASSNAME(const CLASSNAME&);\
    CLASSNAME& operator=(const CLASSNAME&);

class LogLevel : public QObject
{
    Q_OBJECT
    Q_ENUMS(LogLevelEnum)
public:
    enum LogLevelEnum {
        Debug = QtDebugMsg,
#if QT_VERSION >= QT_VERSION_CHECK(5,5,0)
        Info = QtInfoMsg,
#else
        Info = Debug,
#endif
        Warning = QtWarningMsg,
        Critical = QtCriticalMsg,
        Fatal = QtFatalMsg,
        System = QtSystemMsg
    };
private:
    DISABLE_CREATION(LogLevel)
};

class FrameShadow : public QObject
{
    Q_OBJECT
    Q_ENUMS(FrameShadowEnum)
public:
    enum FrameShadowEnum {
        Flat = 0,
        Raise,
        Sunken
    };
private:
    DISABLE_CREATION(FrameShadow)
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
        Sexagesimal,
        SexagesimalDMS,
        SexagesimalHMS
    };
private:
    DISABLE_CREATION(TextFormat)
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
    DISABLE_CREATION(ColorMode)
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
    DISABLE_CREATION(Direction)
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
    DISABLE_CREATION(EdgeStyle)
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
    DISABLE_CREATION(FillStyle)
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
    DISABLE_CREATION(VisibilityMode)
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
    DISABLE_CREATION(LimitsSource)
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
    DISABLE_CREATION(RelatedDisplayVisual)
};

class RelatedDisplayMode : public QObject
{
    Q_OBJECT
    Q_ENUMS(RelatedDisplayModeEnum)
public:
    enum RelatedDisplayModeEnum {
        New = 0,
        Replace
    };
private:
    DISABLE_CREATION(RelatedDisplayMode)
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
    DISABLE_CREATION(LabelStyle)
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
    DISABLE_CREATION(TimeUnit)
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
    DISABLE_CREATION(PlotStyle)
};

class Stacking : public QObject
{
    Q_OBJECT
    Q_ENUMS(StackingEnum)
public:
    enum StackingEnum {
        Column = 0,
        Row,
        RowColumn
    };
private:
    DISABLE_CREATION(Stacking)
};

class FillMode : public QObject
{
    Q_OBJECT
    Q_ENUMS(FillModeEnum)
public:
    enum FillModeEnum {
        FromEdge = 0,
        FromCenter
    };
private:
    DISABLE_CREATION(FillMode)
};

class EraseMode : public QObject
{
    Q_OBJECT
    Q_ENUMS(EraseModeEnum)
public:
    enum EraseModeEnum {
        IfNotZero = 0,
        IfZero
    };
private:
    DISABLE_CREATION(EraseMode)
};

class CartesianPlotTimeFormat : public QObject
{
    Q_OBJECT
    Q_ENUMS(CartesianPlotTimeFormatEnum)
public:
    enum CartesianPlotTimeFormatEnum {
        HHMMSS = 0,
        HHMM,
        HH00,
        MMMDDYYYY,
        MMMDD,
        MMDDHH00,
        WDHH00
    };
private:
    DISABLE_CREATION(CartesianPlotTimeFormat)
};

class CartesianPlotRangeStyle : public QObject
{
    Q_OBJECT
    Q_ENUMS(CartesianPlotRangeStyleEnum)
public:
    enum CartesianPlotRangeStyleEnum {
        Channel = 0,
        User,
        Auto
    };
private:
    DISABLE_CREATION(CartesianPlotRangeStyle)
};

class CartesianPlotYAxis : public QObject
{
    Q_OBJECT
    Q_ENUMS(CartesianPlotYAxisEnum)
public:
    enum CartesianPlotYAxisEnum {
        Y = 0,
        Y2
    };
private:
    DISABLE_CREATION(CartesianPlotYAxis)
};

class CartesianPlotAxisStyle : public QObject
{
    Q_OBJECT
    Q_ENUMS(CartesianPlotAxisStyleEnum)
public:
    enum CartesianPlotAxisStyleEnum {
        Linear = 0,
        Log10,
        Time
    };
private:
    DISABLE_CREATION(CartesianPlotAxisStyle)
};

class ArrowPosition : public QObject
{
    Q_OBJECT
    Q_ENUMS(ArrowPositionEnum)
public:
    enum ArrowPositionEnum {
        None = 0,
        Start,
        End,
        Both
    };
private:
    DISABLE_CREATION(ArrowPosition)
};

#endif // ENUMS_H
