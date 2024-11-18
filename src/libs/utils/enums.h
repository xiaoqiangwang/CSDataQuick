#ifndef ENUMS_H
#define ENUMS_H

#include <QObject>

#include "utils_global.h"

#define DISABLE_CREATION(CLASSNAME) \
    CLASSNAME();\
    CLASSNAME(QObject*);\
    CLASSNAME(const CLASSNAME&);\
    CLASSNAME& operator=(const CLASSNAME&);

class UTILS_EXPORT LogLevel : public QObject
{
    Q_OBJECT
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
#if QT_VERSION < QT_VERSION_CHECK(6,7,0)
        System = QtSystemMsg
#endif
    };
    Q_ENUM(LogLevelEnum)

private:
    DISABLE_CREATION(LogLevel)
};

class UTILS_EXPORT FrameShadow : public QObject
{
    Q_OBJECT
public:
    enum FrameShadowEnum {
        Flat = 0,
        Raise,
        Sunken
    };
    Q_ENUM(FrameShadowEnum)

private:
    DISABLE_CREATION(FrameShadow)
};

class UTILS_EXPORT TextFormat : public QObject
{
    Q_OBJECT
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
    Q_ENUM(TextFormatEnum)
private:
    DISABLE_CREATION(TextFormat)
};

class UTILS_EXPORT ColorMode : public QObject
{
    Q_OBJECT
public:
    enum ColorModeEnum {
        Static = 0,
        Alarm,
        Discrete
    };
    Q_ENUM(ColorModeEnum)
private:
    DISABLE_CREATION(ColorMode)
};

class UTILS_EXPORT AlarmMode : public QObject
{
    Q_OBJECT
public:
    enum AlarmModeEnum {
        Foreground = 0,
        Background
    };
    Q_ENUM(AlarmModeEnum)
private:
    DISABLE_CREATION(AlarmMode)
};

class UTILS_EXPORT Direction : public QObject
{
    Q_OBJECT
public:
    enum DirectionEnum {
        Up = 0,
        Right,
        Down,
        Left
    };
    Q_ENUM(DirectionEnum)
private:
    DISABLE_CREATION(Direction)
};

class UTILS_EXPORT EdgeStyle : public QObject
{
    Q_OBJECT
public:
    enum EdgeStyleEnum {
        Solid = 0,
        Dash
    };
    Q_ENUM(EdgeStyleEnum)
private:
    DISABLE_CREATION(EdgeStyle)
};

class UTILS_EXPORT FillStyle : public QObject
{
    Q_OBJECT
public:
    enum FillStyleEnum {
        Solid = 0,
        Outline
    };
    Q_ENUM(FillStyleEnum)
private:
    DISABLE_CREATION(FillStyle)
};

class UTILS_EXPORT VisibilityMode : public QObject
{
    Q_OBJECT
public:
    enum VisibilityModeEnum {
        Static = 0,
        IfNotZero,
        IfZero,
        Calc
    };
    Q_ENUM(VisibilityModeEnum)
private:
    DISABLE_CREATION(VisibilityMode)
};

class UTILS_EXPORT LimitsSource : public QObject
{
    Q_OBJECT
public:
    enum LimitsSourceEnum {
        Channel = 0,
        Default,
        User,
        Unused
    };
    Q_ENUM(LimitsSourceEnum)
private:
    DISABLE_CREATION(LimitsSource)
};

class UTILS_EXPORT RelatedDisplayVisual : public QObject
{
    Q_OBJECT
public:
    enum RelatedDisplayVisualEnum {
        Menu = 0,
        Row,
        Column,
        Hidden
    };
    Q_ENUM(RelatedDisplayVisualEnum)
private:
    DISABLE_CREATION(RelatedDisplayVisual)
};

class UTILS_EXPORT RelatedDisplayMode : public QObject
{
    Q_OBJECT
public:
    enum RelatedDisplayModeEnum {
        New = 0,
        Replace
    };
    Q_ENUM(RelatedDisplayModeEnum)
private:
    DISABLE_CREATION(RelatedDisplayMode)
};

class UTILS_EXPORT LabelStyle : public QObject
{
    Q_OBJECT
public:
    enum LabelStyleEnum {
        Frame = 0,
        None,
        Outline,
        Limits,
        Channel
    };
    Q_ENUM(LabelStyleEnum)
private:
    DISABLE_CREATION(LabelStyle)
};

class UTILS_EXPORT TimeUnit : public QObject
{
    Q_OBJECT
public:
    enum TimeUnitEnum {
        MilliSecond = 0,
        Second,
        Minute,
    };
    Q_ENUM(TimeUnitEnum)
private:
    DISABLE_CREATION(TimeUnit)
};

class UTILS_EXPORT PlotStyle : public QObject
{
    Q_OBJECT
public:
    enum PlotStyleEnum {
        Point = 0,
        Line,
        FillUnder
    };
    Q_ENUM(PlotStyleEnum)
private:
    DISABLE_CREATION(PlotStyle)
};

class UTILS_EXPORT Stacking : public QObject
{
    Q_OBJECT
public:
    enum StackingEnum {
        Column = 0,
        Row,
        RowColumn
    };
    Q_ENUM(StackingEnum)
private:
    DISABLE_CREATION(Stacking)
};

class UTILS_EXPORT FillMode : public QObject
{
    Q_OBJECT
public:
    enum FillModeEnum {
        FromEdge = 0,
        FromCenter
    };
    Q_ENUM(FillModeEnum)
private:
    DISABLE_CREATION(FillMode)
};

class UTILS_EXPORT EraseMode : public QObject
{
    Q_OBJECT
public:
    enum EraseModeEnum {
        IfNotZero = 0,
        IfZero
    };
    Q_ENUM(EraseModeEnum)
private:
    DISABLE_CREATION(EraseMode)
};

class UTILS_EXPORT CartesianPlotTimeFormat : public QObject
{
    Q_OBJECT
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
    Q_ENUM(CartesianPlotTimeFormatEnum)
private:
    DISABLE_CREATION(CartesianPlotTimeFormat)
};

class UTILS_EXPORT CartesianPlotRangeStyle : public QObject
{
    Q_OBJECT
public:
    enum CartesianPlotRangeStyleEnum {
        Channel = 0,
        User,
        Auto
    };
    Q_ENUM(CartesianPlotRangeStyleEnum)
private:
    DISABLE_CREATION(CartesianPlotRangeStyle)
};

class UTILS_EXPORT CartesianPlotYAxis : public QObject
{
    Q_OBJECT
public:
    enum CartesianPlotYAxisEnum {
        Y = 0,
        Y2
    };
    Q_ENUM(CartesianPlotYAxisEnum)
private:
    DISABLE_CREATION(CartesianPlotYAxis)
};

class UTILS_EXPORT CartesianPlotAxisStyle : public QObject
{
    Q_OBJECT
public:
    enum CartesianPlotAxisStyleEnum {
        Linear = 0,
        Log10,
        Time
    };
    Q_ENUM(CartesianPlotAxisStyleEnum)
private:
    DISABLE_CREATION(CartesianPlotAxisStyle)
};

class UTILS_EXPORT ArrowPosition : public QObject
{
    Q_OBJECT
public:
    enum ArrowPositionEnum {
        None = 0,
        Start,
        End,
        Both
    };
    Q_ENUM(ArrowPositionEnum)
private:
    DISABLE_CREATION(ArrowPosition)
};

#endif // ENUMS_H
