#include <QtDebug>

#include "parser.h"
#include "ui4.h"

UI::UI()
{
    ui = nullptr;
}

UI::~UI()
{
    delete ui;
}

void UI::parse(QXmlStreamReader& reader)
{
    const QString uiElement = QLatin1String("ui");
    while (!reader.atEnd()) {
        if (reader.readNext() == QXmlStreamReader::StartElement) {
            if (reader.name().compare(uiElement, Qt::CaseInsensitive) == 0
                && !ui) {
                double version = reader.attributes().value("version").toDouble();
                if (version < 4.0) {
                    const QString msg = QString::fromLatin1("UIParser: File generated with too old version of Qt Designer (%1)").arg(version);
                    fprintf(stderr, "%s\n", qPrintable(msg));
                    return;
                }
                ui = new DomUI();
                ui->read(reader);
            } else {
                reader.raiseError(QLatin1String("Unexpected element ") + reader.name().toString());
            }
        }
    }
    if (reader.hasError()) {
        delete ui;
        ui = nullptr;
        fprintf(stderr, "%s\n", qPrintable(QString::fromLatin1("UIParser: Error in line %1, column %2 : %3")
                                    .arg(reader.lineNumber()).arg(reader.columnNumber())
                                    .arg(reader.errorString())));
    }
}

void UI::rectToQML(QTextStream& ostream, DomRect *v, int level)
{
    QString indent(level * 4, ' ');

    ostream << indent << "    x: " << QString::number(v->elementX()) << endl;
    ostream << indent << "    y: " << QString::number(v->elementY()) << endl;
    ostream << indent << "    width: " << QString::number(v->elementWidth()) << endl;
    ostream << indent << "    height: " << QString::number(v->elementHeight()) << endl;
}

QString UI::colorToQML(DomColor *v)
{
    QString color;
    QTextStream ostream(&color);

    ostream << "#" << hex << qSetFieldWidth(2) << qSetPadChar('0');

    if (v->hasAttributeAlpha())
        ostream << v->attributeAlpha();

    ostream << v->elementRed()
        << v->elementGreen()
        << v->elementBlue();

    return color;
}

QString UI::directionToQML(QString direction)
{
    if (direction.endsWith("Left"))
        return "Direction.Left";
    else if (direction.endsWith("Down"))
        return "Direction.Down";
    else if (direction.endsWith("Up"))
        return "Direction.Up";
    else
        return "Direction.Right";
}

QString UI::formatToQML(QString format)
{
    if (format.endsWith("decimal"))
        return "TextFormat.Decimal";
    else if (format.endsWith("exponential"))
        return "TextFormat.Exponential";
    else if (format.endsWith("engr_notation"))
        return "TextFormat.EngNotation";
    else if (format.endsWith("compact"))
        return "TextFormat.Compact";
    else if (format.endsWith("truncated"))
        return "TextFormat.Truncated";
    else if (format.endsWith("hexadecimal"))
        return "TextFormat.Hexadecimal";
    else if (format.endsWith("octal"))
        return "TextFormat.Octal";
    else if (format.endsWith("sexagesimal"))
        return "TextFormat.Sexagesimal";
    else if (format.endsWith("sexagesimal_hms"))
        return "TextFormat.SexagesimalHMS";
    else if (format.endsWith("sexagesimal_dms"))
        return "TextFormat.SexagesimalDMS";
    else
        return "TextFormat.String";
}

QString UI::labelStyleToQML(QString style)
{
    if (style.endsWith("Limits"))
        return "LabelStyle.Limits";
    else if (style.endsWith("Outline"))
        return "LabelStyle.Outline";
    else if (style.endsWith("ChannelV"))
        return "LabelStyle.Channel";
    else
        return "LabelStyle.None";
}

bool UI::dynamicAttributeToQML(QTextStream &ostream, DomProperty *v, int level)
{
    QString indent(level * 4, ' ');

    if (v->attributeName() == "visibility") {
        QString visibilityMode = v->elementEnum();
        if (visibilityMode.endsWith("IfNotZero"))
            ostream << indent << "    dynamicAttribute.visibilityMode: VisibilityMode.IfNotZero" << endl;
        else if (visibilityMode.endsWith("IfZero"))
            ostream << indent << "    dynamicAttribute.visibilityMode: VisibilityMode.IfZero" << endl;
        else if (visibilityMode.endsWith("Calc"))
            ostream << indent << "    dynamicAttribute.visibilityMode: VisibilityMode.Calc" << endl;
    }
    else if (v->attributeName() == "visibilityCalc")
        ostream << indent << "    dynamicAttribute.visibilityCalc: '" << v->elementString()->text() << "'" << endl;
    else if (v->attributeName() == "channel")
        ostream << indent << "    dynamicAttribute.channel: '" << v->elementString()->text() << "'" << endl;
    else if (v->attributeName() == "channelB")
        ostream << indent << "    dynamicAttribute.channelB: '" << v->elementString()->text() << "'" << endl;
    else if (v->attributeName() == "channelC")
        ostream << indent << "    dynamicAttribute.channelC: '" << v->elementString()->text() << "'" << endl;
    else if (v->attributeName() == "channelD")
        ostream << indent << "    dynamicAttribute.channelD: '" << v->elementString()->text() << "'" << endl;
    else
        return false;

    return true;
}

bool UI::limitsToQML(QTextStream &ostream, DomProperty *v, int level)
{
    QString indent(level * 4, ' ');
    if (v->attributeName() == "precisionMode") {
        if (v->elementEnum() == "caLineEdit::User")
            ostream << indent << "    limits.precSrc: LimitsSource.Default" << endl;
    }
    else if (v->attributeName() == "precision")
        ostream << indent << "    limits.precDefault: " << v->elementNumber() << endl;
    else if (v->attributeName() == "limitsMode") {
        if (v->elementEnum() == "caLineEdit::User") {
            ostream << indent << "    limits.loprSrc: LimitsSource.Default" << endl;
            ostream << indent << "    limits.hoprSrc: LimitsSource.Default" << endl;
        }
    }
    else if (v->attributeName() == "minValue")
        ostream << indent << "    limits.loprDefault: " << v->elementNumber() << endl;
    else if (v->attributeName() == "maxValue")
        ostream << indent << "    limits.hoprDefault: " << v->elementNumber() << endl;
    else
        return false;

    return true;
}

void UI::fontToQML(QTextStream& ostream, DomFont *v, int level)
{
    QString indent(level * 4, ' ');

    if (v->hasElementFamily())
        ostream << indent << "    font.family: '" << v->elementFamily() << "'" << endl;
    if (v->hasElementPointSize())
        ostream << indent << "    font.pixelSize: " << v->elementPointSize() << endl;
    if (v->hasElementWeight()) {
        int weight = v->elementWeight();
        if (weight >= 87)
            ostream << indent << "    font.weight: Font.Black" << endl;
        else if (weight >= 81)
            ostream << indent << "    font.weight: Font.ExtraBold" << endl;
        else if (weight >= 75)
            ostream << indent << "    font.weight: Font.Bold" << endl;
        else if (weight >= 63)
            ostream << indent << "    font.weight: Font.DemiBold" << endl;
        else if (weight >= 57)
            ostream << indent << "    font.weight: Font.Medium" << endl;
        else if (weight >= 50)
            ostream << indent << "    font.weight: Font.Normal" << endl;
        else if (weight >= 25)
            ostream << indent << "    font.weight: Font.Light" << endl;
        else if (weight >= 12)
            ostream << indent << "    font.weight: Font.ExtraLight" << endl;
        else if (weight >= 0)
            ostream << indent << "    font.weight: Font.Light" << endl;
    }
    if (v->hasElementItalic())
        ostream << indent << "    font.italic: " << (v->elementItalic() ? "true" : "false") << endl;
    if (v->hasElementBold())
        ostream << indent << "    font.bold: " << (v->elementBold() ? "true" : "false") << endl;
    if (v->hasElementUnderline())
        ostream << indent << "    font.underline" << (v->elementUnderline() ? "true" : "false") << endl;
    if (v->hasElementStrikeOut())
        ostream << indent << "    font.strikeout" << (v->elementStrikeOut() ? "true" : "false") << endl;
}

void UI::layoutToQML(QTextStream& ostream, DomLayout *l, int level)
{
    QString indent(level * 4, ' ');
    if (l->attributeClass() == "QHBoxLayout")
        ostream << indent << "RowLayout {" << endl;
    else if (l->attributeClass() == "QVBoxLayout")
        ostream << indent << "ColumnLayout {" << endl;
    else if (l->attributeClass() == "QGridLayout")
        ostream << indent << "GridLayout {" << endl;
    else if (l->attributeClass() == "QFormLayout") {
        ostream << indent << "GridLayout {" << endl;
        ostream << indent << "    columns: 2" << endl;
    }
    else {
        qCritical() << "Layout " << l->attributeClass() << "is not supported";
        return;
    }

    ostream << indent << "    anchors.fill: parent" << endl;

    for (DomLayoutItem *child : l->elementItem()) {
        widgetToQML(ostream, child->elementWidget(), level+1, child);
    }

    ostream << indent << "}" << endl;
}

void UI::tabWidgetToQML(QTextStream& ostream, DomWidget *w, int level, DomLayoutItem*i)
{
    QString indent(level * 4, ' ');

    ostream << indent << "TabView {" << endl;
    if (i && i->hasAttributeRow())
        ostream << indent << "    Layout.row: " << QString::number(i->attributeRow()) << endl;
    if (i && i->hasAttributeColumn())
        ostream << indent << "    Layout.column: " << QString::number(i->attributeColumn()) << endl;
    if (i && i->hasAttributeColSpan())
        ostream << indent << "    Layout.columnSpan: " << QString::number(i->attributeColSpan()) << endl;
    if (i && i->hasAttributeRowSpan())
        ostream << indent << "    Layout.rowSpan: " << QString::number(i->attributeRowSpan()) << endl;

    for (DomProperty *v : w->elementProperty()) {
        if (v->attributeName() == "geometry") {
            rectToQML(ostream, v->elementRect(), level);
        } else if (v->attributeName() == "tabPosition") {
            QString d = v->elementEnum();
            if (d == "QTabWidget::West")
                ostream << indent << "    tabPosition: Qt.LeftEdge" << endl;
            else if (d == "QTabWidget::South")
                ostream << indent << "    tabPosition: Qt.BottomEdge" << endl;
            else if (d == "QTabWidget::East")
                ostream << indent << "    tabPosition: Qt.RightEdge" << endl;
        } else if (v->attributeName() == "currentIndex") {
            ostream << indent << "    currentIndex: " << v->elementNumber() << endl;
        }
    }

    for (DomWidget *child : w->elementWidget()) {
        ostream << indent << "    Tab {" << endl;
        for (DomProperty *v : child->elementAttribute()) {
            if (v->attributeName() == "title") {
                ostream << indent << "        title: '" << v->elementString()->text() << "'" << endl;
                break;
            }
        }
        widgetToQML(ostream, child, level+1, i);
        ostream << indent << "    }" << endl;
    }

    ostream << indent << "}" << endl;
}

void UI::compositeToQML(QTextStream& ostream, DomWidget *w, int level, DomLayoutItem*i)
{
    QString indent(level * 4, ' ');

    ostream << indent << "CSComposite {" << endl;
    for (DomProperty *v : w->elementProperty()) {
        if (v->attributeName() == "geometry") {
            rectToQML(ostream, v->elementRect(), level);
        }
        else if (v->attributeName() == "filename") {
            ostream << indent << "    source: '" << v->elementString()->text() << "'" << endl;
        }
        else if (v->attributeName() == "macro") {
            ostream << indent << "    macro: '" << v->elementString()->text() << "'" << endl;
        }
        else if (dynamicAttributeToQML(ostream, v, level))
            ;
    }

    for (DomWidget *child : w->elementWidget()) {
        widgetToQML(ostream, child, level+1);
    }

    ostream << indent << "}" << endl;
}

void UI::frameToQML(QTextStream& ostream, DomWidget *w, int level, DomLayoutItem*i)
{
    QString indent(level * 4, ' ');

    ostream << indent << "CSComposite {" << endl;
    for (DomProperty *v : w->elementProperty()) {
        if (v->attributeName() == "geometry") {
            rectToQML(ostream, v->elementRect(), level);
        }
        else if (dynamicAttributeToQML(ostream, v, level))
            ;
    }
    for (DomLayout *child : w->elementLayout())
        layoutToQML(ostream, child, level+1);

    for (DomWidget *child : orderedChildWidgets(w)) {
        widgetToQML(ostream, child, level+1);
    }

    ostream << indent << "}" << endl;
}

void UI::graphicsToQML(QTextStream& ostream, DomWidget *w, int level, DomLayoutItem*i)
{
    QString indent(level * 4, ' ');

    QString form = "caGraphics::Rectangle";
    DomRect *rect = nullptr;
    QString arrowMode = "caGraphics::Single";
    for (DomProperty *v : w->elementProperty()) {
        if (v->attributeName() == "form") {
            form = v->elementEnum();
        } else if (v->attributeName() == "geometry") {
            rect = v->elementRect(); 
        } else if (v->attributeName() == "arrowMode") {
            arrowMode = v->elementEnum();
        }
    }

    if (form == "caGraphics::Rectangle")
        ostream << indent << "CSRect {" << endl;
    else if (form == "caGraphics::Circle")
        ostream << indent << "CSOval {" << endl;
    else if (form == "caGraphics::Arc")
        ostream << indent << "CSArc {" << endl;
    else if (form == "caGraphics::Line" || form == "caGraphics::Arrow") {
        ostream << indent << "CSPolyline {" << endl;
        ostream << indent << "    points: [" 
            << "Qt.point(0," << rect->elementHeight()/2 << ")," 
            << "Qt.point(" << rect->elementWidth() << "," << rect->elementHeight()/2 << ")]" << endl;
        
        if (form == "caGraphics::Arrow") {
            if (arrowMode == "caGraphics::Double")
                ostream << indent << "    arrowPosition: ArrowPosition.Both" << endl;
            else
                ostream << indent << "    arrowPosition: ArrowPosition.Start" << endl;
        }
    }
    else if (form == "caGraphics::Triangle") {
        ostream << indent << "CSPolygon {" << endl;
        ostream << indent << "    points: [" 
            << "Qt.point(" << rect->elementWidth()/2 << ",0)," 
            << "Qt.point(0," << rect->elementHeight() << "),"
            << "Qt.point(" << rect->elementWidth() << "," << rect->elementHeight() << ")]" << endl;
    }

    rectToQML(ostream, rect, level);

    bool fill = false;
    for (DomProperty *v : w->elementProperty()) {
        if (v->attributeName() == "foreground")
            ostream << indent << "    foreground: '" << colorToQML(v->elementColor()) << "'" << endl;
        else if (v->attributeName() == "background")
            ostream << indent << "    background: '" << colorToQML(v->elementColor()) << "'" << endl;
        else if (v->attributeName() == "tiltAngle")
            ostream << indent << "    rotation: " << v->elementNumber() << endl;
        else if (v->attributeName() == "linestyle") {
            if (v->elementEnum() == "caGraphics::Dash")
                ostream << indent << "    edgeStyle: EdgeStyle.Dash" << endl;
        }
        else if (v->attributeName() == "fillstyle") {
            if (v->elementEnum() == "caGraphics::Filled")
                fill = true;
        }
        else if  (v->attributeName() == "colorMode") {
            if (v->elementEnum() == "caGraphics::Alarm")
                ostream << indent << "    colorMode: ColorMode.Alarm" << endl;
        }
        else if (v->attributeName() == "lineSize") {
            ostream << indent << "    lineWidth: " << v->elementNumber() << endl; 
        }
        else if (v->attributeName() == "startAngle" && form == "caGraphics::Arc") {
            ostream << indent << "    begin: " << v->elementNumber() << endl; 
        }
        else if (v->attributeName() == "spanAngle" && form == "caGraphics::Arc") {
            ostream << indent << "    span: " << v->elementNumber() << endl; 
        }
        else if (dynamicAttributeToQML(ostream, v, level))
            ;
    }
    if (!fill)
        ostream << indent << "    fillStyle: FillStyle.Outline" << endl;
    ostream << indent << "}" << endl;
}

void UI::imageToQML(QTextStream& ostream, DomWidget *w, int level, DomLayoutItem*i)
{
    QString indent(level * 4, ' ');

    ostream << indent << "CSImage {" << endl;

    for (DomProperty *v : w->elementProperty()) {
        if (v->attributeName() == "geometry") {
            rectToQML(ostream, v->elementRect(), level);
        }
        else if (v->attributeName() == "filename") {
            ostream << indent << "    source: '" << v->elementString()->text() << "'" << endl;
        }
        else if (v->attributeName() == "imageCalc") {
            ostream << indent << "    imageCalc: '" << v->elementString()->text() << "'" << endl;
        }
        else if (dynamicAttributeToQML(ostream, v ,level))
            ;
    }

    ostream << indent << "}" << endl;
}

void UI::labelToQML(QTextStream& ostream, DomWidget *w, int level, DomLayoutItem*i)
{
    QString indent(level * 4, ' ');

    ostream << indent << "CSText {" << endl;

    bool up = false;

    for (DomProperty *v : w->elementProperty()) {
        if (v->attributeName() == "geometry") {
            rectToQML(ostream, v->elementRect(), level);
        }
        else if (v->attributeName() == "foreground") {
            ostream << indent << "    foreground: '" << colorToQML(v->elementColor()) << "'" << endl;
        }
        else if (v->attributeName() == "background") {
            ostream << indent << "    background: '" << colorToQML(v->elementColor()) << "'" << endl;
        }
        else if  (v->attributeName() == "colorMode") {
            if (v->elementEnum().endsWith("Alarm"))
                ostream << indent << "    colorMode: ColorMode.Alarm" << endl;
        }
        else if (v->attributeName() == "font") {
            fontToQML(ostream, v->elementFont(), level);
        }
        else if (v->attributeName() == "text") {
            ostream << indent << "    text: \"" << v->elementString()->text() << "\"" << endl;
        }
        else if (v->attributeName() == "alignment") {
            QString align = v->elementSet();
            if (align.contains("Qt::AlignRight") || align == "caLabelVertical::AlignRight")
                ostream << indent << "    align: Text.AlignRight" << endl;
            else if (align.contains("Qt::AlignHCenter") || align == "caLabelVertical::AlignCenter")
                ostream << indent << "    align: Text.AlignHCenter" << endl;
            else if (align.contains("Qt::AlignJustify"))
                ostream << indent << "    align: Text.AlignJustify" << endl;
        }
        else if (v->attributeName() == "direction") {
            if (v->elementEnum() == "caLabelVertical::Up")
                up = true;
        }
        else if (dynamicAttributeToQML(ostream, v, level))
            ;
    }
    
    if (w->attributeClass() == "caLabelVertical") {
        ostream << indent << "    // caLabelVertical" << endl;
        ostream << indent << "    rotation: " << (up ? -90 : 90) << endl;
    }

    ostream << indent << "}" << endl;
}

void UI::polylineToQML(QTextStream& ostream, DomWidget *w, int level, DomLayoutItem*i)
{
    QString indent(level * 4, ' ');

    bool polygon = false;
    for (DomProperty *v : w->elementProperty()) {
        if (v->attributeName() == "polystyle") {
            if (v->elementEnum() == "caPolyLine::Polygon")
                polygon = true;
        }
    }

    if (polygon)
        ostream << indent << "CSPolygon {" << endl;
    else
        ostream << indent << "CSPolyline {" << endl;

    bool fill = false;
    for (DomProperty *v : w->elementProperty()) {
        if (v->attributeName() == "geometry") {
            rectToQML(ostream, v->elementRect(), level);
        }
        else if (v->attributeName() == "foreground")
            ostream << indent << "    foreground: '" << colorToQML(v->elementColor()) << "'" << endl;
        else if (v->attributeName() == "background")
            ostream << indent << "    background: '" << colorToQML(v->elementColor()) << "'" << endl;
        else if (v->attributeName() == "tiltAngle")
            ostream << indent << "    rotation: " << v->elementNumber() << endl;
        else if (v->attributeName() == "linestyle") {
            if (v->elementEnum() == "caPolyLine::Dash")
                ostream << indent << "    edgeStyle: EdgeStyle.Dash" << endl;
        }
        else if (v->attributeName() == "fillstyle") {
            if (v->elementEnum() == "caPolyLine::Filled")
                fill = true;
        }
        else if  (v->attributeName() == "colorMode") {
            if (v->elementEnum() == "caPolyLine::Alarm")
                ostream << indent << "    colorMode: ColorMode.Alarm" << endl;
        }
        else if (v->attributeName() == "lineSize") {
            ostream << indent << "    lineWidth: " << v->elementNumber() << endl; 
        }
        else if (v->attributeName() == "xyPairs") {
            QString xyPairs = v->elementString()->text();
            ostream << indent << "    points: [";
            for (QString xyPair : xyPairs.split(';', QString::SkipEmptyParts)) {
                ostream << "Qt.point(" << xyPair << "),";
            }
            ostream << "]" << endl;
        }
        else if (dynamicAttributeToQML(ostream, v, level))
            ;
    }
    if (!fill)
        ostream << indent << "    fillStyle: FillStyle.Outline" << endl;

    ostream << indent << "}" << endl;
}

void UI::byteToQML(QTextStream& ostream, DomWidget *w, int level, DomLayoutItem*i)
{
    QString indent(level * 4, ' ');

    ostream << indent << "CSByte {" << endl;

    for (DomProperty *v : w->elementProperty()) {
        if (v->attributeName() == "geometry") {
            rectToQML(ostream, v->elementRect(), level);
        }
        else if (v->attributeName() == "channel") {
            ostream << indent << "    source: '" << v->elementString()->text() << "'" << endl;
        }
        else if (v->attributeName() == "foreground") {
            ostream << indent << "    foreground: '" << colorToQML(v->elementColor()) << "'" << endl;
        }
        else if (v->attributeName() == "background") {
            ostream << indent << "    background: '" << colorToQML(v->elementColor()) << "'" << endl;
        }
        else if  (v->attributeName() == "colorMode") {
            if (v->elementEnum().contains("Alarm"))
                ostream << indent << "    colorMode: ColorMode.Alarm" << endl;
        }
        else if  (v->attributeName() == "direction") {
            QString direction = v->elementEnum();
            if (direction.endsWith("Up") || direction.endsWith("Down"))
                ostream << indent << "    orientation: Qt.Vertical" << endl;
            else
                ostream << indent << "    orientation: Qt.Horizontal" << endl;
        }
        else if  (v->attributeName() == "startBit") {
            ostream << indent << "    start: " << v->elementNumber() << endl;
        }
        else if  (v->attributeName() == "endBit") {
            ostream << indent << "    end: " << v->elementNumber() << endl;
        }
    }

    ostream << indent << "}" << endl;
}
 
void UI::barToQML(QTextStream& ostream, DomWidget *w, int level, DomLayoutItem*i)
{
    QString indent(level * 4, ' ');

    ostream << indent << "CSBar {" << endl;

    for (DomProperty *v : w->elementProperty()) {
        if (v->attributeName() == "geometry") {
            rectToQML(ostream, v->elementRect(), level);
        }
        else if (v->attributeName() == "channel") {
            ostream << indent << "    source: '" << v->elementString()->text() << "'" << endl;
        }
        else if (v->attributeName() == "foreground") {
            ostream << indent << "    foreground: '" << colorToQML(v->elementColor()) << "'" << endl;
        }
        else if (v->attributeName() == "background") {
            ostream << indent << "    background: '" << colorToQML(v->elementColor()) << "'" << endl;
        }
        else if  (v->attributeName() == "colorMode") {
            if (v->elementEnum().contains("Alarm"))
                ostream << indent << "    colorMode: ColorMode.Alarm" << endl;
        }
        else if  (v->attributeName() == "direction") {
            ostream << indent << "    direction: " << directionToQML(v->elementEnum()) << endl;
        }
        else if  (v->attributeName() == "look") {
            ostream << indent << "    labelStyle: " << labelStyleToQML(v->elementEnum()) << endl;
        }
        else if (limitsToQML(ostream, v, level))
            ;
    }

    ostream << indent << "}" << endl;
}

void UI::indicatorToQML(QTextStream &ostream, DomWidget*w, int level, DomLayoutItem*i)
{
    QString indent(level * 4, ' ');

    ostream << indent << "CSIndicator {" << endl;

    for (DomProperty *v : w->elementProperty()) {
        if (v->attributeName() == "geometry") {
            rectToQML(ostream, v->elementRect(), level);
        }
        else if (v->attributeName() == "channel") {
            ostream << indent << "    source: '" << v->elementString()->text() << "'" << endl;
        }
        else if (v->attributeName() == "foreground") {
            ostream << indent << "    foreground: '" << colorToQML(v->elementColor()) << "'" << endl;
        }
        else if (v->attributeName() == "background") {
            ostream << indent << "    background: '" << colorToQML(v->elementColor()) << "'" << endl;
        }
        else if  (v->attributeName() == "colorMode") {
            if (v->elementEnum().contains("Alarm"))
                ostream << indent << "    colorMode: ColorMode.Alarm" << endl;
        }
        else if  (v->attributeName() == "direction") {
            ostream << indent << "    direction: " << directionToQML(v->elementEnum()) << endl;
        }
        else if  (v->attributeName() == "look") {
            ostream << indent << "    labelStyle: " << labelStyleToQML(v->elementEnum()) << endl;
        }
        else if (limitsToQML(ostream, v, level))
            ;
    }

    ostream << indent << "}" << endl;
}

void UI::meterToQML(QTextStream &ostream, DomWidget*w, int level, DomLayoutItem*i)
{
    QString indent(level * 4, ' ');

    ostream << indent << "CSMeter {" << endl;

    for (DomProperty *v : w->elementProperty()) {
        if (v->attributeName() == "geometry") {
            rectToQML(ostream, v->elementRect(), level);
        }
        else if (v->attributeName() == "channel") {
            ostream << indent << "    source: '" << v->elementString()->text() << "'" << endl;
        }
        else if (v->attributeName() == "foreground") {
            ostream << indent << "    foreground: '" << colorToQML(v->elementColor()) << "'" << endl;
        }
        else if (v->attributeName() == "background") {
            ostream << indent << "    background: '" << colorToQML(v->elementColor()) << "'" << endl;
        }
        else if  (v->attributeName() == "colorMode") {
            if (v->elementEnum().contains("Alarm"))
                ostream << indent << "    colorMode: ColorMode.Alarm" << endl;
        }
    }

    ostream << indent << "}" << endl;
}

void UI::stripChartToQML(QTextStream& ostream, DomWidget *w, int level, DomLayoutItem*i)
{
    QString indent(level * 4, ' ');

    ostream << indent << "CSStripChart {" << endl;

    class Trace {
        public:
        Trace () {
            source = "";
            color = "#000000";
            loprDefault = 0; hoprDefault = 1;
            loprSrc = "LimitsSource.Channel";
            hoprSrc = "LimitsSource.Channel";
        };
        static QString scalingToQML(QString scaling) {
            if (scaling == "caStripPlot::Channel")
                return "LimitsSource.Channel";
            else
                return "LimitsSource.Default";
        };
        QString source;
        QString color;
        double loprDefault;
        double hoprDefault;
        QString loprSrc;
        QString hoprSrc;
    };

    Trace traces[8];
    for (DomProperty *v : w->elementProperty()) {
        if (v->attributeName() == "geometry")
            rectToQML(ostream, v->elementRect(), level);
        else if (v->attributeName() == "foreground")
            ostream << indent << "    foreground: '" << colorToQML(v->elementColor()) << "'" << endl;
        else if (v->attributeName() == "background")
            ostream << indent << "    background: '" << colorToQML(v->elementColor()) << "'" << endl;
        else if (v->attributeName() == "Title")
            ostream << indent << "    title: '" << v->elementString()->text() << "'" << endl;
        else if (v->attributeName() == "TitleX")
            ostream << indent << "    xLabel: '" << v->elementString()->text() << "'" << endl;
        else if (v->attributeName() == "TitleY")
            ostream << indent << "    yLabel: '" << v->elementString()->text() << "'" << endl;
        else if (v->attributeName() == "units") {
            QString units = v->elementEnum();
            if (units.endsWith("Second"))
                ostream << indent << "    units: TimeUnit.Second" << endl;
            else if (units.endsWith("Minute"))
                ostream << indent << "    units: TimeUnit.Minute" << endl;
        }
        else if (v->attributeName() == "period")
            ostream << indent << "    period: " << v->elementNumber() << endl;
        else if (v->attributeName() == "channels") {
            QStringList channels = v->elementString()->text().split(';');
            for (int i=0; i<channels.size(); i++) {
                traces[i].source = channels[i]; 
            }
        }
        else if (v->attributeName().startsWith("color_")) {
            int index = v->attributeName().mid(6).toInt();
            traces[index].color = colorToQML(v->elementColor());
        }
        else if (v->attributeName().startsWith("YaxisScalingMin_")) {
            int index = v->attributeName().mid(16).toInt();
            traces[index].loprSrc = Trace::scalingToQML(v->elementEnum());
        }
        else if (v->attributeName().startsWith("YaxisScalingMax_")) {
            int index = v->attributeName().mid(16).toInt();
            traces[index].hoprSrc = Trace::scalingToQML(v->elementEnum());
        }
        else if (v->attributeName().startsWith("YaxisLimitsMin_")) {
            int index = v->attributeName().mid(15).toInt();
            traces[index].loprDefault = v->elementDouble();
        }
        else if (v->attributeName().startsWith("YaxisLimitsMax_")) {
            int index = v->attributeName().mid(15).toInt();
            traces[index].hoprDefault = v->elementDouble();
        }
    }

    ostream << indent << "    model: ListModel {" << endl;
    for (int i=0; i<8; i++) {
        if (traces[i].source.isEmpty())
            continue;
        ostream << indent << "        ListElement {" << endl;
        ostream << indent << "            source: '" << traces[i].source << "'" << endl;
        ostream << indent << "            color: '" << traces[i].color << "'" << endl;
        ostream << indent << "            loprSrc: " << traces[i].loprSrc << endl;
        ostream << indent << "            hoprSrc: " << traces[i].hoprSrc << endl;
        ostream << indent << "            loprDefault: " << traces[i].loprDefault << endl;
        ostream << indent << "            hoprDefault: " << traces[i].hoprDefault << endl;
        ostream << indent << "        }" << endl;
    }
    ostream << indent << "    }" << endl;
    ostream << indent << "}" << endl;
}

void UI::textUpdateToQML(QTextStream& ostream, DomWidget *w, int level, DomLayoutItem*i)
{
    QString indent(level * 4, ' ');

    ostream << indent << "CSTextUpdate {" << endl;

    for (DomProperty *v : w->elementProperty()) {
        if (v->attributeName() == "geometry") {
            rectToQML(ostream, v->elementRect(), level);
        }
        else if (v->attributeName() == "channel") {
            ostream << indent << "    source: '" << v->elementString()->text() << "'" << endl;
        }
        else if (v->attributeName() == "foreground") {
            ostream << indent << "    foreground: '" << colorToQML(v->elementColor()) << "'" << endl;
        }
        else if (v->attributeName() == "background") {
            ostream << indent << "    background: '" << colorToQML(v->elementColor()) << "'" << endl;
        }
        else if  (v->attributeName() == "colorMode") {
            if (v->elementEnum().contains("Alarm"))
                ostream << indent << "    colorMode: ColorMode.Alarm" << endl;
        }
        else if (v->attributeName() == "font") {
            fontToQML(ostream, v->elementFont(), level);
        }
        else if (v->attributeName() == "fontScaleMode") {
            QString fontSizeMode = v->elementEnum();
            if (fontSizeMode == "ESimpleLabel::None")
                ostream << indent << "    fontSizeMode: Text.FixedSize" << endl;
            else if (fontSizeMode == "ESimpleLabel::Height")
                ostream << indent << "    fontSizeMode: Text.VerticalFit" << endl;
            else if (fontSizeMode == "ESimpleLabel::WidthAndHeight")
                ostream << indent << "    fontSizeMode: Text.Fit" << endl;
        }
        else if (v->attributeName() == "unitsEnabled") {
            ostream << indent << "    unitsVisible: " << v->elementBool() << endl;
        }
        else if (v->attributeName() == "alignment") {
            QString align = v->elementSet();
            if (align.contains("Qt::AlignRight") || align == "caLabelVertical::AlignRight")
                ostream << indent << "    align: Text.AlignRight" << endl;
            else if (align.contains("Qt::AlignHCenter") || align == "caLabelVertical::AlignCenter")
                ostream << indent << "    align: Text.AlignHCenter" << endl;
            else if (align.contains("Qt::AlignJustify"))
                ostream << indent << "    align: Text.AlignJustify" << endl;
        }
        else if (v->attributeName() == "formatType") {
            ostream << indent << "    format: " << formatToQML(v->elementEnum()) << endl;
        }
        else if (limitsToQML(ostream, v, level))
            ;
        else if (dynamicAttributeToQML(ostream, v, level))
            ;
    }
 
    ostream << indent << "}" << endl;
}

void UI::choiceButtonToQML(QTextStream& ostream, DomWidget *w, int level, DomLayoutItem*i)
{
    QString indent(level * 4, ' ');

    ostream << indent << "CSChoiceButton {" << endl;

    for (DomProperty *v : w->elementProperty()) {
        if (v->attributeName() == "geometry") {
            rectToQML(ostream, v->elementRect(), level);
        }
        else if (v->attributeName() == "channel") {
            ostream << indent << "    source: '" << v->elementString()->text() << "'" << endl;
        }
        else if (v->attributeName() == "foreground") {
            ostream << indent << "    foreground: '" << colorToQML(v->elementColor()) << "'" << endl;
        }
        else if (v->attributeName() == "background") {
            ostream << indent << "    background: '" << colorToQML(v->elementColor()) << "'" << endl;
        }
        else if  (v->attributeName() == "colorMode") {
            if (v->elementEnum().contains("Alarm"))
                ostream << indent << "    colorMode: ColorMode.Alarm" << endl;
        }
        else if  (v->attributeName() == "stackingMode") {
            QString stacking = v->elementEnum();
            if (stacking == "caChoice::Column")
                ostream << indent << "    stacking: Stacking.Row" << endl;
            else if (stacking == "caChoice::Row" || stacking == "caChoice::RowInverse")
                ostream << indent << "    stacking: Stacking.Column" << endl;
            else if (stacking == "caChoice::RowColumn")
                ostream << indent << "    stacking: Stacking.RowColumn" << endl;
        }
    }

    ostream << indent << "}" << endl;
}

void UI::menuToQML(QTextStream& ostream, DomWidget *w, int level, DomLayoutItem*i)
{
    QString indent(level * 4, ' ');

    ostream << indent << "CSMenu {" << endl;

    for (DomProperty *v : w->elementProperty()) {
        if (v->attributeName() == "geometry") {
            rectToQML(ostream, v->elementRect(), level);
        }
        else if (v->attributeName() == "channel") {
            ostream << indent << "    source: '" << v->elementString()->text() << "'" << endl;
        }
        else if (v->attributeName() == "foreground") {
            ostream << indent << "    foreground: '" << colorToQML(v->elementColor()) << "'" << endl;
        }
        else if (v->attributeName() == "background") {
            ostream << indent << "    background: '" << colorToQML(v->elementColor()) << "'" << endl;
        }
        else if  (v->attributeName() == "colorMode") {
            if (v->elementEnum().contains("Alarm"))
                ostream << indent << "    colorMode: ColorMode.Alarm" << endl;
        }
    }

    ostream << indent << "}" << endl;
}

void UI::messageButtonToQML(QTextStream& ostream, DomWidget *w, int level, DomLayoutItem*i)
{
    QString indent(level * 4, ' ');

    ostream << indent << "CSMessageButton {" << endl;

    for (DomProperty *v : w->elementProperty()) {
        if (v->attributeName() == "geometry") {
            rectToQML(ostream, v->elementRect(), level);
        }
        else if (v->attributeName() == "channel") {
            ostream << indent << "    source: '" << v->elementString()->text() << "'" << endl;
        }
        else if (v->attributeName() == "foreground") {
            ostream << indent << "    foreground: '" << colorToQML(v->elementColor()) << "'" << endl;
        }
        else if (v->attributeName() == "background") {
            ostream << indent << "    background: '" << colorToQML(v->elementColor()) << "'" << endl;
        }
        else if  (v->attributeName() == "colorMode") {
            if (v->elementEnum().contains("Alarm"))
                ostream << indent << "    colorMode: ColorMode.Alarm" << endl;
        }
        else if (v->attributeName() == "label") {
            ostream << indent << "    text: '" << v->elementString()->text() << "'" << endl;
        }
        else if (v->attributeName() == "pressMessage") {
            ostream << indent << "    onMessage: '" << v->elementString()->text() << "'" << endl;
        }
        else if (v->attributeName() == "releaseMessage") {
            ostream << indent << "    offMessage: '" << v->elementString()->text() << "'" << endl;
        }
     }

    ostream << indent << "}" << endl;
}

void UI::relatedDisplayToQML(QTextStream& ostream, DomWidget *w, int level, DomLayoutItem*i)
{
    QString indent(level * 4, ' ');

    ostream << indent << "CSRelatedDisplay {" << endl;

    class Entry {
        public:
        Entry () {
            label = "";
            file = "";
            arg = "";
            remove = "false";
        };
        QString label;
        QString file;
        QString arg;
        QString remove;
    };

    QVector<Entry> entries;
    for (DomProperty *v : w->elementProperty()) {
        if (v->attributeName() == "geometry") {
            rectToQML(ostream, v->elementRect(), level);
        }
        else if (v->attributeName() == "foreground") {
            ostream << indent << "    foreground: '" << colorToQML(v->elementColor()) << "'" << endl;
        }
        else if (v->attributeName() == "background") {
            ostream << indent << "    background: '" << colorToQML(v->elementColor()) << "'" << endl;
        }
        else if (v->attributeName() == "label") {
            ostream << indent << "    label: '" << v->elementString()->text() << "'" << endl;
        }
        else if (v->attributeName() == "stackingMode") {
            QString stacking = v->elementEnum();
            if (stacking.endsWith("Column"))
                ostream << indent << "    visual: RelatedDisplayVisual.Column" << endl;
            else if (stacking.endsWith("Row"))
                ostream << indent << "    visual: RelatedDisplayVisual.Row" << endl;
            else
                ostream << indent << "    visual: RelatedDisplayVisual.Menu" << endl;
        }
        else if (v->attributeName() == "labels") {
            QStringList labels = v->elementString()->text().split(';');
            for (int i=0; i<labels.size(); i++) {
                if (i >= entries.size())
                    entries.append(Entry());
                entries[i].label = labels[i];
            }
        }
        else if (v->attributeName() == "files") {
            QStringList files = v->elementString()->text().split(';');
            for (int i=0; i<files.size(); i++) {
                if (i >= entries.size())
                    entries.append(Entry());
                // replace .adl suffix with .ui
                if (files[i].endsWith(".adl")) {
                    files[i].replace(files[i].size() - 4, 4, ".ui");
                }
                entries[i].file = files[i];
            }
         }
        else if (v->attributeName() == "args") {
            QStringList args = v->elementString()->text().split(';');
            for (int i=0; i<args.size(); i++) {
                if (i >= entries.size())
                    entries.append(Entry());
                entries[i].arg = args[i];
            }
        }
        else if (v->attributeName() == "removeParent") {
            QStringList remove = v->elementString()->text().split(';');
            for (int i=0; i<remove.size(); i++) {
                if (i >= entries.size())
                    entries.append(Entry());
                entries[i].remove = remove[i];
            }
         }
     }

    ostream << indent << "    model: ListModel {" << endl;
    for (Entry entry : entries) {
        if (entry.file.isEmpty())
            continue;
        ostream << indent << "        ListElement {" << endl;
        ostream << indent << "            file: '" << entry.file << "'" << endl;
        ostream << indent << "            macro: '" << entry.arg << "'" << endl;
        ostream << indent << "            label: '" << entry.label << "'" << endl;
        ostream << indent << "            remove: " << entry.remove << endl;
        ostream << indent << "        }" << endl;
    }
    ostream << indent << "    }" << endl;
 
    ostream << indent << "}" << endl;
}

void UI::shellCommandToQML(QTextStream& ostream, DomWidget *w, int level, DomLayoutItem*i)
{
    QString indent(level * 4, ' ');

    ostream << indent << "CSShellCommand {" << endl;

    class Entry {
        public:
        Entry () {
            label = "";
            file = "";
            arg = "";
        };
        QString label;
        QString file;
        QString arg;
    };

    QVector<Entry> entries;
    for (DomProperty *v : w->elementProperty()) {
        if (v->attributeName() == "geometry") {
            rectToQML(ostream, v->elementRect(), level);
        }
        else if (v->attributeName() == "foreground") {
            ostream << indent << "    foreground: '" << colorToQML(v->elementColor()) << "'" << endl;
        }
        else if (v->attributeName() == "background") {
            ostream << indent << "    background: '" << colorToQML(v->elementColor()) << "'" << endl;
        }
        else if (v->attributeName() == "label") {
            ostream << indent << "    label: '" << v->elementString()->text() << "'" << endl;
        }
        else if (v->attributeName() == "labels") {
            QStringList labels = v->elementString()->text().split(';');
            for (int i=0; i<labels.size(); i++) {
                if (i >= entries.size())
                    entries.append(Entry());
                entries[i].label = labels[i];
            }
        }
        else if (v->attributeName() == "files") {
            QStringList files = v->elementString()->text().split(';');
            for (int i=0; i<files.size(); i++) {
                if (i >= entries.size())
                    entries.append(Entry());
                entries[i].file = files[i];
            }
         }
        else if (v->attributeName() == "args") {
            QStringList args = v->elementString()->text().split(';');
            for (int i=0; i<args.size(); i++) {
                if (i >= entries.size())
                    entries.append(Entry());
                entries[i].arg = args[i];
            }
        }
     }

    ostream << indent << "    model: ListModel {" << endl;
    for (Entry entry : entries) {
        if (entry.file.isEmpty())
            continue;
        ostream << indent << "        ListElement {" << endl;
        ostream << indent << "            label: '" << entry.label << "'" << endl;
        ostream << indent << "            command: '" << entry.file << " " << entry.arg << "'" << endl;
        ostream << indent << "        }" << endl;
    }
    ostream << indent << "    }" << endl;
 
    ostream << indent << "}" << endl;
}

void UI::sliderToQML(QTextStream& ostream, DomWidget *w, int level, DomLayoutItem*i)
{
    QString indent(level * 4, ' ');

    ostream << indent << "CSSlider {" << endl;

    for (DomProperty *v : w->elementProperty()) {
        if (v->attributeName() == "geometry") {
            rectToQML(ostream, v->elementRect(), level);
        }
        else if (v->attributeName() == "channel") {
            ostream << indent << "    source: '" << v->elementString()->text() << "'" << endl;
        }
        else if (v->attributeName() == "foreground") {
            ostream << indent << "    foreground: '" << colorToQML(v->elementColor()) << "'" << endl;
        }
        else if (v->attributeName() == "background") {
            ostream << indent << "    background: '" << colorToQML(v->elementColor()) << "'" << endl;
        }
        else if  (v->attributeName() == "colorMode") {
            if (v->elementEnum().contains("Alarm"))
                ostream << indent << "    colorMode: ColorMode.Alarm" << endl;
        }
        else if  (v->attributeName() == "direction") {
            ostream << indent << "    direction: " << directionToQML(v->elementEnum()) << endl;
        }
        else if  (v->attributeName() == "look") {
            ostream << indent << "    labelStyle: " << labelStyleToQML(v->elementEnum()) << endl;
        }
        else if (limitsToQML(ostream, v, level))
            ;
    }

    ostream << indent << "}" << endl;
}

void UI::spinBoxToQML(QTextStream& ostream, DomWidget *w, int level, DomLayoutItem*i)
{
    QString indent(level * 4, ' ');

    ostream << indent << "CSSpinBox {" << endl;

    for (DomProperty *v : w->elementProperty()) {
        if (v->attributeName() == "geometry") {
            rectToQML(ostream, v->elementRect(), level);
        }
        else if (v->attributeName() == "channel") {
            ostream << indent << "    source: '" << v->elementString()->text() << "'" << endl;
        }
        else if (v->attributeName() == "foreground") {
            ostream << indent << "    foreground: '" << colorToQML(v->elementColor()) << "'" << endl;
        }
        else if (v->attributeName() == "background") {
            ostream << indent << "    background: '" << colorToQML(v->elementColor()) << "'" << endl;
        }
        else if  (v->attributeName() == "colorMode") {
            if (v->elementEnum().contains("Alarm"))
                ostream << indent << "    colorMode: ColorMode.Alarm" << endl;
        }
        else if (limitsToQML(ostream, v, level))
            ;
    }

    ostream << indent << "}" << endl;
}

void UI::textEntryToQML(QTextStream& ostream, DomWidget *w, int level, DomLayoutItem*i)
{
    QString indent(level * 4, ' ');

    ostream << indent << "CSTextEntry {" << endl;

    for (DomProperty *v : w->elementProperty()) {
        if (v->attributeName() == "geometry") {
            rectToQML(ostream, v->elementRect(), level);
        }
        else if (v->attributeName() == "channel") {
            ostream << indent << "    source: '" << v->elementString()->text() << "'" << endl;
        }
        else if (v->attributeName() == "foreground") {
            ostream << indent << "    foreground: '" << colorToQML(v->elementColor()) << "'" << endl;
        }
        else if (v->attributeName() == "background") {
            ostream << indent << "    background: '" << colorToQML(v->elementColor()) << "'" << endl;
        }
        else if  (v->attributeName() == "colorMode") {
            if (v->elementEnum().contains("Alarm"))
                ostream << indent << "    colorMode: ColorMode.Alarm" << endl;
        }
        else if (v->attributeName() == "alignment") {
            QString align = v->elementSet();
            if (align.contains("Qt::AlignRight") || align == "caLabelVertical::AlignRight")
                ostream << indent << "    align: Text.AlignRight" << endl;
            else if (align.contains("Qt::AlignHCenter") || align == "caLabelVertical::AlignCenter")
                ostream << indent << "    align: Text.AlignHCenter" << endl;
            else if (align.contains("Qt::AlignJustify"))
                ostream << indent << "    align: Text.AlignJustify" << endl;
        }
        else if (v->attributeName() == "formatType") {
            ostream << indent << "    format: " << formatToQML(v->elementEnum()) << endl;
        }
        else if (limitsToQML(ostream, v, level))
            ;
    }

    ostream << indent << "}" << endl;
}

void UI::wheelSwitchToQML(QTextStream& ostream, DomWidget *w, int level, DomLayoutItem*i)
{
    QString indent(level * 4, ' ');

    ostream << indent << "CSWheelSwitch {" << endl;

    for (DomProperty *v : w->elementProperty()) {
        if (v->attributeName() == "geometry") {
            rectToQML(ostream, v->elementRect(), level);
        }
        else if (v->attributeName() == "channel") {
            ostream << indent << "    source: '" << v->elementString()->text() << "'" << endl;
        }
        else if (v->attributeName() == "foreground") {
            ostream << indent << "    foreground: '" << colorToQML(v->elementColor()) << "'" << endl;
        }
        else if (v->attributeName() == "background") {
            ostream << indent << "    background: '" << colorToQML(v->elementColor()) << "'" << endl;
        }
        else if  (v->attributeName() == "colorMode") {
            if (v->elementEnum().contains("Alarm"))
                ostream << indent << "    colorMode: ColorMode.Alarm" << endl;
        }
        else if (limitsToQML(ostream, v, level))
            ;
  
    }

    ostream << indent << "}" << endl;
}

QVector<DomWidget*> UI::orderedChildWidgets(DomWidget *w)
{
    if (w->elementZOrder().size() == 0)
        return w->elementWidget();

    QVector<DomWidget*>  ordered;
    for (QString widgetName : w->elementZOrder()) {
        for (DomWidget *child : w->elementWidget()) {
            if (child->attributeName() == widgetName) {
                ordered.push_back(child);
                break;
            }
        }
    }
    return ordered;
}

void UI::widgetToQML(QTextStream& ostream, DomWidget *w, int level, DomLayoutItem*i)
{
    QString indent(level * 4, ' ');
    
    QString widgetClass = w->attributeClass();
    if (widgetClass == "QTabWidget")
        tabWidgetToQML(ostream, w, level, i);
    else if (widgetClass == "caInclude")
        compositeToQML(ostream, w, level, i);
    else if (widgetClass == "caLabel" || widgetClass == "QLabel" || widgetClass == "caLabelVertical")
        labelToQML(ostream, w, level, i);
    else if (widgetClass == "caFrame" || widgetClass == "QFrame")
        frameToQML(ostream, w, level, i);
    else if (widgetClass == "caGraphics")
        graphicsToQML(ostream, w, level, i);
    else if (widgetClass == "caImage")
        imageToQML(ostream, w, level, i);
    else if (widgetClass == "caPolyLine")
        polylineToQML(ostream, w, level, i);
    else if (widgetClass == "caByte")
        byteToQML(ostream, w, level, i);
    else if (widgetClass == "caThermo")
        barToQML(ostream, w, level, i);
    else if (widgetClass == "caLinearGauge")
        indicatorToQML(ostream, w, level, i);
    else if (widgetClass == "caCircularGauge")
        meterToQML(ostream, w, level, i);
    else if (widgetClass == "caStripPlot")
        stripChartToQML(ostream, w, level, i);
    else if (widgetClass == "caLineEdit" || widgetClass == "caMultiLineString")
        textUpdateToQML(ostream, w, level, i);
    else if (widgetClass == "caChoice")
        choiceButtonToQML(ostream, w, level, i);
    else if (widgetClass == "caMenu")
        menuToQML(ostream, w, level, i);
    else if (widgetClass == "caMessageButton")
        messageButtonToQML(ostream, w, level, i);
    else if (widgetClass == "caRelatedDisplay")
        relatedDisplayToQML(ostream, w, level, i);
    else if (widgetClass == "caShellCommand")
        shellCommandToQML(ostream, w, level, i);
    else if (widgetClass == "caSlider")
        sliderToQML(ostream, w, level, i);
    else if (widgetClass == "caSpinbox")
        spinBoxToQML(ostream, w, level, i);
    else if (widgetClass == "caTextEntry")
        textEntryToQML(ostream, w, level, i);
    else if (widgetClass == "caNumeric" || widgetClass == "caApplyNumeric")
        wheelSwitchToQML(ostream, w, level, i);
    else if (widgetClass == "QMenuBar" || widgetClass == "QStatusBar")
        qCritical() << "widget " << widgetClass << "not supported";
    else if (widgetClass == "QWidget") {
        ostream << indent << "Item {" << endl;
        for (DomLayout *child : w->elementLayout())
            layoutToQML(ostream, child, level+2);
        for (DomWidget *child : orderedChildWidgets(w))
            widgetToQML(ostream, child, level+2);
        ostream << indent << "}" << endl;
     }
    else {
        qDebug() << "Use CSRect for widget class " << widgetClass << endl;
        ostream << indent << "CSRect {" << endl;
        ostream << indent << "    implicitHeight: 20" << endl;
        ostream << indent << "    fillStyle: FillStyle.Outline" << endl;

        if (i && i->hasAttributeRow())
            ostream << indent << "    Layout.row: " << QString::number(i->attributeRow()) << endl;
        if (i && i->hasAttributeColumn())
            ostream << indent << "    Layout.column: " << QString::number(i->attributeColumn()) << endl;
        if (i && i->hasAttributeColSpan())
            ostream << indent << "    Layout.columnSpan: " << QString::number(i->attributeColSpan()) << endl;
        if (i && i->hasAttributeRowSpan())
            ostream << indent << "    Layout.rowSpan: " << QString::number(i->attributeRowSpan()) << endl;

        for (DomProperty *v : w->elementProperty()) {
            if (v->attributeName() == "geometry") {
                rectToQML(ostream, v->elementRect(), level);
            }
        }

        for (DomLayout *child : w->elementLayout())
            layoutToQML(ostream, child, level+1);

        for (DomWidget *child : orderedChildWidgets(w))
            widgetToQML(ostream, child, level+1);

        ostream << indent << "}" << endl;
    }
}

void UI::toQML(QTextStream& ostream)
{
    ostream << "import QtQuick 2.0\n";
    ostream << "import QtQuick.Layouts 1.0\n";
    ostream << "import QtQuick.Controls 1.0\n";
    ostream << "import CSDataQuick.Components 1.0\n";
    ostream << "import CSDataQuick.Components.Private 1.0\n";
    ostream << "BaseWindow {\n";

    DomWidget *mainWidget = ui->elementWidget();
    if (!mainWidget)
        return;

    for (DomProperty *v : mainWidget->elementProperty()) {
        if (v->attributeName() == "geometry") {
            rectToQML(ostream, v->elementRect());
        }
    }

    for (DomWidget *child : orderedChildWidgets(mainWidget))
        widgetToQML(ostream, child, 1);

    ostream << "}" << endl;
}

void UI::toPartialQML(QTextStream& ostream)
{
    DomWidget *mainWidget = ui->elementWidget();
    if (!mainWidget)
        return;

    for (DomWidget *child : orderedChildWidgets(mainWidget))
        widgetToQML(ostream, child);
}
