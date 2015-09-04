#ifndef COMMON_H
#define COMMON_H

#ifdef ALLOCATE_STORAGE
#define EXTERN
#else
#define EXTERN extern
#endif

    #define MIN(a,b)  ((a)<(b)?(a):(b))
    #define MAX(a,b)  ((a)>(b)?(a):(b))

    /* Define these to set the number of menu items in the Related Display
     * and Shell Command. */
    #define MAX_RELATED_DISPLAYS    16
    #define MAX_SHELL_COMMANDS      16

    #define MAX_TOKEN_LENGTH        256     /* max size of strings in adl    */
    #define MAX_PENS                8       /* max # of pens on strip chart  */
    #define MAX_TRACES              8       /* max # of traces on cart. plot */
    #define MAX_FILE_CHARS          256     /* max # of chars in filename    */
    #define MAX_CALC_RECORDS        4       /* max # of records for calc     */
    #define MAX_CALC_INPUTS         12      /* max # of inputs for calc      */

    #define DL_MAX_COLORS           65      /* max # of colors for display   */
    /*********************************************************************
     * Resource Types                                                    *
     *********************************************************************/
    #define NUM_TRAVERSAL_MODES     2
    typedef enum {
        DL_EXECUTE  = 0,
        DL_EDIT     = 1
    } DlTraversalMode;
    #ifdef ALLOCATE_STORAGE
    const DlTraversalMode FIRST_TRAVERSAL_MODE = DL_EXECUTE;
    #else
    extern const DlTraversalMode FIRST_TRAVERSAL_MODE;
    #endif

    #define NUM_LABEL_TYPES         5
    typedef enum {
        LABEL_NONE     = 2,
        NO_DECORATIONS = 3,
        OUTLINE        = 4,
        LIMITS         = 5,
        CHANNEL        = 6
    } LabelType;
    #if defined(ALLOCATE_STORAGE) || defined(__cplusplus)
    const LabelType FIRST_LABEL_TYPE = LABEL_NONE;
    #else
    extern const LabelType FIRST_LABEL_TYPE;
    #endif

    #define NUM_COLOR_MODES         3
    typedef enum {
        STATIC   = 7,
        ALARM    = 8,
        DISCRETE = 9
    } ColorMode;
    #if defined(ALLOCATE_STORAGE) || defined(__cplusplus)
    const ColorMode FIRST_COLOR_MODE = STATIC;
    #else
    extern const ColorMode FIRST_COLOR_MODE;
    #endif

    #define NUM_VISIBILITY_MODES    4
    typedef enum {
        V_STATIC    = 10,
        IF_NOT_ZERO = 11,
        IF_ZERO     = 12,
        V_CALC      = 13
    } VisibilityMode;
    #if defined(ALLOCATE_STORAGE) || defined(__cplusplus)
    const VisibilityMode FIRST_VISIBILITY_MODE = V_STATIC;
    #else
    extern const VisibilityMode FIRST_VISIBILITY_MODE;
    #endif

    #define NUM_DIRECTIONS          4
    typedef enum {
        UP    = 14,
        RIGHT = 15,
        DOWN  = 16,
        LEFT  = 17
    } Direction;
    #if defined(ALLOCATE_STORAGE) || defined(__cplusplus)
    const Direction FIRST_DIRECTION = UP;
    #else
    extern const Direction FIRST_DIRECTION;
    #endif

    #define NUM_EDGE_STYLES         2
    typedef enum {
        SOLID = 18,
        DASH  = 19
    } EdgeStyle;
    #if defined(ALLOCATE_STORAGE) || defined(__cplusplus)
    const EdgeStyle FIRST_EDGE_STYLE = SOLID;
    #else
    extern const EdgeStyle FIRST_EDGE_STYLE;
    #endif

    #define NUM_FILL_STYLES         2
    typedef enum {
        F_SOLID   = 20,
        F_OUTLINE = 21
    } FillStyle;
    #if defined(ALLOCATE_STORAGE) || defined(__cplusplus)
    const FillStyle FIRST_FILL_STYLE = F_SOLID;
    #else
    extern const FillStyle FIRST_FILL_STYLE;
    #endif


    #define NUM_TEXT_FORMATS        11
    typedef enum {
        MEDM_DECIMAL  = 22,
        EXPONENTIAL   = 23,
        ENGR_NOTATION = 24,
        COMPACT       = 25,
        TRUNCATED     = 26,
        HEXADECIMAL   = 27,
        OCTAL         = 28,
        STRING        = 29,
        SEXAGESIMAL   = 30,
        SEXAGESIMAL_HMS= 31,
        SEXAGESIMAL_DMS= 32
    } TextFormat;
    #if defined(ALLOCATE_STORAGE) || defined(__cplusplus)
    const TextFormat FIRST_TEXT_FORMAT = MEDM_DECIMAL;
    #else
    extern const TextFormat FIRST_TEXT_FORMAT;
    #endif

    #define NUM_TEXT_ALIGNS         3
    typedef enum {
        HORIZ_LEFT   = 33,
        HORIZ_CENTER = 34,
        HORIZ_RIGHT  = 35
    } TextAlign;
    #if defined(ALLOCATE_STORAGE) || defined(__cplusplus)
    const TextAlign FIRST_TEXT_ALIGN = HORIZ_LEFT;
    #else
    extern const TextAlign FIRST_TEXT_ALIGN;
    #endif

    #define NUM_STACKINGS           3
    typedef enum {
        COLUMN      = 36,
        ROW         = 37,
        ROW_COLUMN  = 38
    } Stacking;
    #if defined(ALLOCATE_STORAGE) || defined(__cplusplus)
    const Stacking FIRST_STACKING = COLUMN;
    #else
    extern const Stacking FIRST_STACKING;
    #endif

    #define NUM_FILL_MODES          2
    typedef enum {
        FROM_EDGE   = 39,
        FROM_CENTER = 40
    } FillMode;
    #if defined(ALLOCATE_STORAGE) || defined(__cplusplus)
    const FillMode FIRST_FILL_MODE = FROM_EDGE;
    #else
    extern const FillMode FIRST_FILL_MODE;
    #endif

    #define NUM_TIME_UNITS          3
    typedef enum {
        MILLISECONDS = 41,
        SECONDS      = 42,
        MINUTES      = 43
    } TimeUnits;
    #if defined(ALLOCATE_STORAGE) || defined(__cplusplus)
    const TimeUnits FIRST_TIME_UNIT = MILLISECONDS;
    #else
    extern const TimeUnits FIRST_TIME_UNIT;
    #endif

    #define NUM_CARTESIAN_PLOT_STYLES       3
    typedef enum {
        POINT_PLOT      = 44,
        LINE_PLOT       = 45,
        FILL_UNDER_PLOT = 46
    } CartesianPlotStyle;
    #if defined(ALLOCATE_STORAGE) || defined(__cplusplus)
    const CartesianPlotStyle FIRST_CARTESIAN_PLOT_STYLE = POINT_PLOT;
    #else
    extern const CartesianPlotStyle FIRST_CARTESIAN_PLOT_STYLE;
    #endif

    #define NUM_ERASE_OLDESTS       2
    typedef enum {
        ERASE_OLDEST_OFF = 47,
        ERASE_OLDEST_ON  = 48
    } EraseOldest;
    #if defined(ALLOCATE_STORAGE) || defined(__cplusplus)
    const EraseOldest FIRST_ERASE_OLDEST = ERASE_OLDEST_OFF;
    #else
    extern const EraseOldest FIRST_ERASE_OLDEST;
    #endif

    #define NUM_IMAGE_TYPES 3
    typedef enum {
        NO_IMAGE   = 49,
        GIF_IMAGE  = 50,
        TIFF_IMAGE = 51
    } ImageType;
    #if defined(ALLOCATE_STORAGE) || defined(__cplusplus)
    const ImageType FIRST_IMAGE_TYPE = NO_IMAGE;
    #else
    extern const ImageType FIRST_IMAGE_TYPE;
    #endif

    #define NUM_CARTESIAN_PLOT_AXIS_STYLES 3
    typedef enum {
        LINEAR_AXIS = 52,
        LOG10_AXIS  = 53,
        TIME_AXIS   = 54
    } CartesianPlotAxisStyle;
    #if defined(ALLOCATE_STORAGE) || defined(__cplusplus)
    const CartesianPlotAxisStyle FIRST_CARTESIAN_PLOT_AXIS_STYLE = LINEAR_AXIS;
    #else
    extern const CartesianPlotAxisStyle FIRST_CARTESIAN_PLOT_AXIS_STYLE;
    #endif

    #define NUM_CARTESIAN_PLOT_RANGE_STYLES 3
    typedef enum {
        CHANNEL_RANGE        = 55,
        USER_SPECIFIED_RANGE = 56,
        AUTO_SCALE_RANGE     = 57
    } CartesianPlotRangeStyle;

    #if defined(ALLOCATE_STORAGE) || defined(__cplusplus)
    const CartesianPlotRangeStyle FIRST_CARTESIAN_PLOT_RANGE_STYLE = CHANNEL_RANGE;
    #else
    extern const CartesianPlotRangeStyle FIRST_CARTESIAN_PLOT_RANGE_STYLE;
    #endif

    #define NUM_ERASE_MODES 2
    typedef enum {
        ERASE_IF_NOT_ZERO = 58,
        ERASE_IF_ZERO     = 59
    } eraseMode_t;
    #if defined(ALLOCATE_STORAGE) || defined(__cplusplus)
    const eraseMode_t FIRST_ERASE_MODE = ERASE_IF_NOT_ZERO;
    #else
    extern const eraseMode_t FIRST_ERASE_MODE;
    #endif

    #define NUM_RD_MODES 2
    typedef enum {
        ADD_NEW_DISPLAY = 60,
        REPLACE_DISPLAY = 61
    } relatedDisplayMode_t;
    #if defined(ALLOCATE_STORAGE) || defined(__cplusplus)
    const relatedDisplayMode_t FIRST_RD_MODE = ADD_NEW_DISPLAY;
    #else
    extern const relatedDisplayMode_t FIRST_RD_MODE;
    #endif

    #define NUM_RD_VISUAL 4
    typedef enum {
        RD_MENU       = 62,
        RD_ROW_OF_BTN = 63,
        RD_COL_OF_BTN = 64,
        RD_HIDDEN_BTN = 65
    } relatedDisplayVisual_t;
    #if defined(ALLOCATE_STORAGE) || defined(__cplusplus)
    const relatedDisplayVisual_t FIRST_RD_VISUAL = RD_MENU;
    #else
    extern const relatedDisplayVisual_t FIRST_RD_VISUAL;
    #endif

    #define NUM_CP_TIME_FORMAT 7
    typedef enum {
        HHMMSS    = 66,
        HHMM      = 67,
        HH00      = 68,
        MMMDDYYYY = 69,
        MMMDD     = 70,
        MMDDHH00  = 71,
        WDHH00    = 72
    } CartesianPlotTimeFormat_t;
    #if defined(ALLOCATE_STORAGE) || defined(__cplusplus)
    const CartesianPlotTimeFormat_t FIRST_CP_TIME_FORMAT = HHMMSS;
    #else
    extern const CartesianPlotTimeFormat_t FIRST_CP_TIME_FORMAT;
    #endif

    #define NUM_Boolean_t 2
    typedef enum {
        BOOLEAN_FALSE = 73,
        BOOLEAN_TRUE  = 74
    } Boolean_t;
    #if defined(ALLOCATE_STORAGE) || defined(__cplusplus)
    const Boolean_t FIRST_Boolean_t = BOOLEAN_FALSE;
    #else
    extern const Boolean_t FIRST_BOOLEAN;
    #endif

    #define NUM_PV_LIMITS_SRC 4
    typedef enum {
        PV_LIMITS_CHANNEL = 75,
        PV_LIMITS_DEFAULT = 76,
        PV_LIMITS_USER    = 77,
        PV_LIMITS_UNUSED  = 78
    } PvLimitsSrc_t;
    #if defined(ALLOCATE_STORAGE) || defined(__cplusplus)
    const PvLimitsSrc_t FIRST_PV_LIMITS_SRC = PV_LIMITS_CHANNEL;
    #else
    extern const PvLimitsSrc_t FIRST_PV_LIMITS_SRC;
    #endif

    #define NUM_CARTESIAN_Y_AXIS        2
    typedef enum {
        Y      = 79,
        Y2     = 80
    } CartesianYAxis;
    #if defined(ALLOCATE_STORAGE) || defined(__cplusplus)
    const CartesianYAxis FIRST_CARTESIAN_Y_AXIS = Y;
    #else
    extern const CartesianYAxis FIRST_CARTESIAN_Y_AXIS;
    #endif


    #define MAX_OPTIONS             11    /* NUM_TEXT_FORMATS */
    #define NUMBER_STRING_VALUES    81    /* Y2+1 */

    /*********************************************************************
     * stringValueTable for string-valued tokens - position sensitive!   *
     * any changes of types or ordering of above must have matching      *
     * changes in this table!                                            *
     *********************************************************************/
    #ifndef ALLOCATE_STORAGE
    extern char *stringValueTable[NUMBER_STRING_VALUES];
    extern char *qmlValueTable[NUMBER_STRING_VALUES];
    #else
    const char *stringValueTable[NUMBER_STRING_VALUES] = {
        "execute", "edit",
        "none", "no decorations", "outline", "limits", "channel",
        "static", "alarm", "discrete",
        "static", "if not zero", "if zero", "calc",
        "up", "right", "down", "left",
        "solid", "dash",
        "solid", "outline",
        "decimal", "exponential", "engr. notation", "compact", "truncated",
        "hexadecimal", "octal", "string",
        "sexagesimal", "sexagesimal-hms", "sexagesimal-dms",
        "horiz. left", "horiz. centered", "horiz. right",
        "column", "row", "row column",
        "from edge", "from center",
        "milli-second", "second", "minute",
        "point", "line", "fill-under",
        "plot n pts & stop", "plot last n pts",
        "no image", "gif", "tiff",
        "linear", "log10", "time",
        "from channel", "user-specified", "auto-scale",
        "if not zero", "if zero",
        "create new display", "replace display",
        "menu", "a row of buttons", "a column of buttons", "invisible",
        "hh:mm:ss", "hh:mm", "hh:00", "MMM DD YYYY", "MMM DD", "MMM DD hh:00",
        "wd hh:00",
        "false", "true",
        "channel", "default", "user", "unused",
        "Y", "Y2",
    };

    const char *qmlValueTable[NUMBER_STRING_VALUES] = {
        "execute", "edit",
        "LabelStyle.Frame", "LabelStyle.None", "LabelStyle.Outline", "LabelStyle.Limits", "LabelStyle.Channel",
        "ColorMode.Static", "ColorMode.Alarm", "ColorMode.Discrete",
        "VisibilityMode.Static", "VisibilityMode.IfNotZero", "VisibilityMode.IfZero", "VisibilityMode.Calc",
        "Direction.Up", "Direction.Right", "Direction.Down", "Direction.Left",
        "EdgeStyle.Solid", "EdgeStyle.Dash",
        "FillStyle.Solid", "FillStyle.Outline",
        "TextFormat.Decimal", "TextFormat.Exponential", "TextFormat.EngNotation", "TextFormat.Compact", "TextFormat.Truncated",
        "TextFormat.Hexadecimal", "TextFormat.Octal", "TextFormat.String",
        "TextFormat.Sexagesimal", "TextFormat.SexagesimalHMS", "TextFormat.SexagesimalDMS",
        "Text.AlignLeft", "Text.AlignHCenter", "Text.AlignRight",
        "Stacking.Row", "Stacking.Column", "Stacking.RowColumn",
        "FillMode.FromEdge", "FillMode.FromCenter",
        "TimeUnit.MilliSecond", "TimeUnit.Second", "TimeUnit.Minute",
        "PlotStyle.Point", "PlotStyle.Line", "PlotStyle.FillUnder",
        "false", "true",
        "no image", "gif", "tiff",
        "CartesianPlotAxisStyle.Linear", "CartesianPlotAxisStyle.Log10", "CartesianPlotAxisStyle.Time",
        "CartesianPlotRangeStyle.Channel", "CartesianPlotRangeStyle.User", "CartesianPlotRangeStyle.Auto",
        "EraseMode.IfNotZero", "EraseMode.IfZero",
        "RelatedDisplayMode.New", "RelatedDisplayMode.Replace",
        "RelatedDisplayVisual.Menu", "RelatedDisplayVisual.Row", "RelatedDisplayVisual.Column", "RelatedDisplayVisual.Hidden",
        "CartesianPlotTimeFormat.HHMMSS", "CartesianPlotTimeFormat.HHMM", "CartesianPlotTimeFormat.HH00", "CartesianPlotTimeFormat.MMMDDYYYY", "CartesianPlotTimeFormat.MMMDD", "CartesianPlotTimeFormat.MMMDDHH00",
        "CartesianPlotTimeFormat.WDHH00",
        "false", "true",
        "LimitsSource.Channel", "LimitsSource.Default", "LimitsSource.User", "LimitsSource.Unused",
        "CartesianPlotYAxis.Y", "CartesianPlotYAxis.Y2",
    };
    #endif

    /**********************************************************************
     * Controllers are also monitors (controllers are a sub-class of      *
     *  monitors) -> order must be consistent with all of the following:  *
     * Controllers:                                                       *
     *    DL_Valuator   DL_ChoiceButton   DL_MessageButton DL_TextEntry   *
     *    DL_Menu       DL_RelatedDisplay DL_ShellCommand  DL_WheelSwitch *
     * Monitors:                                                          *
     *    DL_Meter      DL_TextUpdate     DL_Bar           DL_Indicator   *
     *    DL_StripChart DL_CartesianPlot  DL_SurfacePlot                  *
     * Statics acting as monitors (dynamics):                             *
     *    DL_Rectangle  DL_Oval           DL_Arc           DL_Text        *
     *    DL_Polyline   DL_Polygon                                        *
     **********************************************************************/

    typedef enum {
      /* Self */
        DL_Element        =100,
      /* Basics */
        DL_Composite      =101,
        DL_Display        =102,
      /* Controllers */
        DL_ChoiceButton   =103,
        DL_Menu           =104,
        DL_MessageButton  =105,
        DL_RelatedDisplay =106,
        DL_ShellCommand   =107,
        DL_TextEntry      =108,
        DL_Valuator       =109,
        DL_WheelSwitch    =110,
      /* Monitors */
        DL_Bar            =111,
        DL_Byte           =112,
        DL_CartesianPlot  =113,
        DL_Indicator      =114,
        DL_Meter          =115,
        DL_StripChart     =116,
        DL_TextUpdate     =117,
      /* Graphics */
        DL_Arc            =118,
        DL_Image          =119,
        DL_Line           =120,
        DL_Oval           =121,
        DL_Polygon        =122,
        DL_Polyline       =123,
        DL_Rectangle      =124,
        DL_Text           =125
    } DlElementType;

    /*
     * types for parsing
     */
    #include <iostream>
    typedef enum
    { T_WORD, T_EQUAL, T_QUOTE, T_LEFT_BRACE, T_RIGHT_BRACE, T_EOF} TOKEN;

    EXTERN TOKEN getToken(std::istream &fstream, char *word);
    EXTERN void parseAndSkip(std::istream &fstream);
#endif // COMMON_H
