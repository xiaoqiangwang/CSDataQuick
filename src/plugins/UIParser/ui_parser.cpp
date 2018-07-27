#include <QRegularExpression>
#include <QSet>
#include <QtDebug>

#include "ui_parser.h"
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

void UI::layoutItemToQML(QTextStream &ostream, DomLayoutItem *i, int level)
{
    if (i == nullptr)
        return;

    QString indent(level * 4, ' ');
    if (i->hasAttributeRow())
        ostream << indent << "    Layout.row: " << QString::number(i->attributeRow()) << endl;
    if (i->hasAttributeColumn())
        ostream << indent << "    Layout.column: " << QString::number(i->attributeColumn()) << endl;
    if (i->hasAttributeColSpan())
        ostream << indent << "    Layout.columnSpan: " << QString::number(i->attributeColSpan()) << endl;
    if (i->hasAttributeRowSpan())
        ostream << indent << "    Layout.rowSpan: " << QString::number(i->attributeRowSpan()) << endl;
}

bool UI::limitsToQML(QTextStream &ostream, DomProperty *v, int level)
{
    QString indent(level * 4, ' ');
    if (v->attributeName() == "precisionMode") {
        if (v->elementEnum().endsWith("User"))
            ostream << indent << "    limits.precSrc: LimitsSource.Default" << endl;
    }
    else if (v->attributeName() == "precision")
        ostream << indent << "    limits.precDefault: " << v->elementNumber() << endl;
    else if (v->attributeName() == "limitsMode") {
        if (v->elementEnum().endsWith("User")) {
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
        DomWidget *w = child->elementWidget();
        if (w)
            widgetToQML(ostream, w, level+1, child);
    }

    ostream << indent << "}" << endl;
}

void UI::groupBoxToQML(QTextStream& ostream, DomWidget *w, int level, DomLayoutItem*i)
{
    QString indent(level * 4, ' ');

    ostream << indent << "GroupBox {" << endl;
    layoutItemToQML(ostream, i, level);

    for (DomProperty *v : uniqueProperties(w->elementProperty())) {
        if (v->attributeName() == "geometry") {
            rectToQML(ostream, v->elementRect(), level);
        }
        else if (v->attributeName() == "title") {
            ostream << indent << "    title: '" << v->elementString()->text() << "'" << endl;
        }
    }

    for (DomLayout *child : w->elementLayout())
        layoutToQML(ostream, child, level+1);

    for (DomWidget *child : orderedChildWidgets(w)) {
        widgetToQML(ostream, child, level+1);
    }

    ostream << indent << "}" << endl;
}

void UI::textEditToQML(QTextStream& ostream, DomWidget *w, int level, DomLayoutItem*i)
{
    QString indent(level * 4, ' ');

    ostream << indent << "TextEdit {" << endl;
    layoutItemToQML(ostream, i, level);

    for (DomProperty *v : uniqueProperties(w->elementProperty())) {
        if (v->attributeName() == "geometry") {
            rectToQML(ostream, v->elementRect(), level);
        }
        else if (v->attributeName() == "font") {
            fontToQML(ostream, v->elementFont(), level);
        }
        else if (v->attributeName() == "readOnly") {
            ostream << "    readOnly: " << v->elementBool() << endl;
        }
        else if (v->attributeName() == "plainText") {
            QString text = v->elementString()->text();
            // escape single quote if not escaped yet
            text.replace(QRegularExpression("(?<!\\\\)(')"), "\\'");
            ostream << "    text: '" << text << "'" << endl;
        }
        else if (v->attributeName() == "html") {
            QString text = v->elementString()->text();
            // escape single quote if not escaped yet
            text.replace(QRegularExpression("(?<!\\\\)(')"), "\\'");
            ostream << "    textFormat: TextEdit.RichText" << endl;
            ostream << "    text: '" << text << "'" << endl;
        }
    }

    ostream << indent << "}" << endl;
}

void UI::tabWidgetToQML(QTextStream& ostream, DomWidget *w, int level, DomLayoutItem*i)
{
    QString indent(level * 4, ' ');

    ostream << indent << "TabView {" << endl;
    layoutItemToQML(ostream, i, level);

    for (DomProperty *v : uniqueProperties(w->elementProperty())) {
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
    layoutItemToQML(ostream, i, level);

    for (DomProperty *v : uniqueProperties(w->elementProperty())) {
        if (v->attributeName() == "geometry") {
            rectToQML(ostream, v->elementRect(), level);
        }
        else if (v->attributeName() == "filename") {
            QString file = v->elementString()->text();
            if (file.endsWith(".adl")) {
                file.replace(file.size() - 4, 4, ".ui");
            }
            ostream << indent << "    source: '" << file << "'" << endl;
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
    layoutItemToQML(ostream, i, level);
    for (DomProperty *v : uniqueProperties(w->elementProperty())) {
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
    for (DomProperty *v : uniqueProperties(w->elementProperty())) {
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

    layoutItemToQML(ostream, i, level);
    rectToQML(ostream, rect, level);

    bool fill = false;
    for (DomProperty *v : uniqueProperties(w->elementProperty())) {
        if (v->attributeName() == "foreground")
            ostream << indent << "    foreground: '" << colorToQML(v->elementColor()) << "'" << endl;
        else if (v->attributeName() == "background")
            ostream << indent << "    background: '" << colorToQML(v->elementColor()) << "'" << endl;
        else if (v->attributeName() == "tiltAngle")
            ostream << indent << "    rotation: " << v->elementNumber() << endl;
        else if (v->attributeName() == "linestyle") {
            if (v->elementEnum().endsWith("Dash"))
                ostream << indent << "    edgeStyle: EdgeStyle.Dash" << endl;
        }
        else if (v->attributeName() == "fillstyle") {
            if (v->elementEnum().endsWith("Filled"))
                fill = true;
        }
        else if  (v->attributeName() == "colorMode") {
            if (v->elementEnum().endsWith("Alarm"))
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
    layoutItemToQML(ostream, i, level);

    for (DomProperty *v : uniqueProperties(w->elementProperty())) {
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
    layoutItemToQML(ostream, i, level);

    bool up = false;

    for (DomProperty *v : uniqueProperties(w->elementProperty())) {
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
        else if (v->attributeName() == "fontScaleMode") {
            QString fontSizeMode = v->elementEnum();
            if (fontSizeMode == "ESimpleLabel::None")
                ostream << indent << "    fontSizeMode: Text.FixedSize" << endl;
            else if (fontSizeMode == "ESimpleLabel::Height")
                ostream << indent << "    fontSizeMode: Text.VerticalFit" << endl;
            else if (fontSizeMode == "ESimpleLabel::WidthAndHeight")
                ostream << indent << "    fontSizeMode: Text.Fit" << endl;
        }
        else if (v->attributeName() == "text") {
            QString text = v->elementString()->text();
            // escape single quote if not escaped yet
            text.replace(QRegularExpression("(?<!\\\\)(')"), "\\'");
            ostream << indent << "    text: '" <<  text << "'" << endl;
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
    for (DomProperty *v : uniqueProperties(w->elementProperty())) {
        if (v->attributeName() == "polystyle") {
            if (v->elementEnum().endsWith("Polygon"))
                polygon = true;
        }
    }

    if (polygon)
        ostream << indent << "CSPolygon {" << endl;
    else
        ostream << indent << "CSPolyline {" << endl;

    layoutItemToQML(ostream, i, level);

    bool fill = false;
    for (DomProperty *v : uniqueProperties(w->elementProperty())) {
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
            if (v->elementEnum().endsWith("Dash"))
                ostream << indent << "    edgeStyle: EdgeStyle.Dash" << endl;
        }
        else if (v->attributeName() == "fillstyle") {
            if (v->elementEnum().endsWith("Filled"))
                fill = true;
        }
        else if  (v->attributeName() == "colorMode") {
            if (v->elementEnum().endsWith("Alarm"))
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
    layoutItemToQML(ostream, i, level);

    for (DomProperty *v : uniqueProperties(w->elementProperty())) {
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

void UI::cartesianPlotToQML(QTextStream& ostream, DomWidget *w, int level, DomLayoutItem*i)
{
    QString indent(level * 4, ' ');

    class Trace {
        public:
        Trace () {
            xchannel = "";
            ychannel = "";
            color = "#000000";
        };
        QString xchannel;
        QString ychannel;
        QString color;
    };

    ostream << indent << "CSCartesianPlot {" << endl;
    layoutItemToQML(ostream, i, level);

    Trace traces[8];
    for (DomProperty *v : uniqueProperties(w->elementProperty())) {
        if (v->attributeName() == "geometry") {
            rectToQML(ostream, v->elementRect(), level);
        }
        else if (v->attributeName() == "foreground") {
            ostream << indent << "    foreground: '" << colorToQML(v->elementColor()) << "'" << endl;
        }
        else if (v->attributeName() == "background") {
            ostream << indent << "    background: '" << colorToQML(v->elementColor()) << "'" << endl;
        }
        else if (v->attributeName() == "Title") {
            ostream << indent << "    title: '" << v->elementString()->text() << "'" << endl;
        }
        else if (v->attributeName() == "TitleX") {
            ostream << indent << "    xLabel: '" << v->elementString()->text() << "'" << endl;
        }
        else if (v->attributeName() == "TitleY") {
            ostream << indent << "    yLabel: '" << v->elementString()->text() << "'" << endl;
        }
        else if (v->attributeName() == "countNumOrChannel") {
            ostream << indent << "    countSource: '" << v->elementString()->text() << "'" << endl;
        }
        else if (v->attributeName() == "XaxisScaling") {
            if (v->elementEnum().endsWith("User"))
                ostream << indent << "    xRangeStyle: CartesianPlotRangeStyle.User" << endl;
        }
        else if (v->attributeName() == "YaxisScaling") {
            if (v->elementEnum().endsWith("User"))
                ostream << indent << "    yRangeStyle: CartesianPlotRangeStyle.User" << endl;
        }
        else if (v->attributeName() == "XaxisLimits") {
            QStringList limits = v->elementString()->text().split(';');
            ostream << indent << "    xRangeLower: " << limits[0] << endl;
            ostream << indent << "    xRangeUpper: " << limits[1] << endl;
        }
        else if (v->attributeName() == "YaxisLimits") {
            QStringList limits = v->elementString()->text().split(';');
            ostream << indent << "    yRangeLower: " << limits[0] << endl;
            ostream << indent << "    yRangeUpper: " << limits[1] << endl;
        }
        else if (v->attributeName().startsWith("channels_")) {
            int index = v->attributeName().mid(9).toInt();
            QStringList channels = v->elementString()->text().split(';');
            if (channels.size() == 2) {
                traces[index].xchannel = channels[0];
                traces[index].ychannel = channels[1];
            }
        }
        else if (v->attributeName().startsWith("color_")) {
            int index = v->attributeName().mid(6).toInt();
            traces[index].color = colorToQML(v->elementColor());
        }
    }

    ostream << indent << "    model: ListModel {" << endl;
    for (int i=0; i<8; i++) {
        if (traces[i].xchannel.isEmpty() && traces[i].ychannel.isEmpty())
            continue;
        ostream << indent << "        ListElement {" << endl;
        ostream << indent << "            xchannel: '" << traces[i].xchannel << "'" << endl;
        ostream << indent << "            ychannel: '" << traces[i].ychannel << "'" << endl;
        ostream << indent << "            color: '" << traces[i].color << "'" << endl;
        ostream << indent << "        }" << endl;
    }
    ostream << indent << "    }" << endl;

    ostream << indent << "}" << endl;
}

void UI::barToQML(QTextStream& ostream, DomWidget *w, int level, DomLayoutItem*i)
{
    QString indent(level * 4, ' ');

    ostream << indent << "CSBar {" << endl;
    layoutItemToQML(ostream, i, level);

    for (DomProperty *v : uniqueProperties(w->elementProperty())) {
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
    layoutItemToQML(ostream, i, level);

    for (DomProperty *v : uniqueProperties(w->elementProperty())) {
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

void UI::ledToQML(QTextStream &ostream, DomWidget*w, int level, DomLayoutItem*i)
{
    QString indent(level * 4, ' ');

    bool rect = false;
    for (DomProperty *v : uniqueProperties(w->elementProperty())) {
        if (v->attributeName() == "rectangular") {
            rect = v->elementBool() == "true";
            break;
        }
    }

    if (rect)
        ostream << indent << "CSRect {" << endl;
    else
        ostream << indent << "CSOval {" << endl;

    layoutItemToQML(ostream, i, level);
    ostream << indent << "    implicitWidth: 18" << endl;
    ostream << indent << "    implicitHeight: 18" << endl; 

    bool scaleContents = false;
    int x = 0, y = 0, width = 0, height = 0;
    int ledWidth = 18, ledHeight = 18;
    for (DomProperty *v : uniqueProperties(w->elementProperty())) {
        if (v->attributeName() == "geometry") {
            x = v->elementRect()->elementX();
            y = v->elementRect()->elementY();
            width = v->elementRect()->elementWidth();
            height = v->elementRect()->elementHeight();
        }
        else if (v->attributeName() == "scaleContents") {
            scaleContents = (v->elementBool() == "true");
        }
        else if (v->attributeName() == "ledWidth") {
            ledWidth = v->elementNumber();
        }
        else if (v->attributeName() == "ledHeight") {
            ledHeight = v->elementNumber();
        }
        else if (v->attributeName() == "channel") {
            ostream << indent << "    dynamicAttribute.channel: '" << v->elementString()->text() << "'" << endl;
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

    if (scaleContents) {
        ostream << indent << "    x: " << x << endl;
        ostream << indent << "    y: " << y << endl;
        ostream << indent << "    width: " << width << endl;
        ostream << indent << "    height: " << height << endl;
    } else {
        ostream << indent << "    x: " << x + (width - ledWidth) / 2 << endl;
        ostream << indent << "    y: " << y + (height - ledHeight) / 2 << endl;
        ostream << indent << "    width: " << ledWidth << endl;
        ostream << indent << "    height: " << ledHeight << endl;
    }

    ostream << indent << "}" << endl;
}

void UI::meterToQML(QTextStream &ostream, DomWidget*w, int level, DomLayoutItem*i)
{
    QString indent(level * 4, ' ');

    ostream << indent << "CSMeter {" << endl;
    layoutItemToQML(ostream, i, level);

    for (DomProperty *v : uniqueProperties(w->elementProperty())) {
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
    layoutItemToQML(ostream, i, level);

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
    for (DomProperty *v : uniqueProperties(w->elementProperty())) {
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
            ostream << indent << "    period: " << v->elementDouble() << endl;
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
        ostream << indent << "            channel: '" << traces[i].source << "'" << endl;
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
    layoutItemToQML(ostream, i, level);

    for (DomProperty *v : uniqueProperties(w->elementProperty())) {
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
    layoutItemToQML(ostream, i, level);

    for (DomProperty *v : uniqueProperties(w->elementProperty())) {
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
            if (stacking.endsWith("Column"))
                ostream << indent << "    stacking: Stacking.Row" << endl;
            else if (stacking.endsWith("Row") || stacking.endsWith("RowInverse"))
                ostream << indent << "    stacking: Stacking.Column" << endl;
            else if (stacking.endsWith("RowColumn"))
                ostream << indent << "    stacking: Stacking.RowColumn" << endl;
        }
    }

    ostream << indent << "}" << endl;
}

void UI::menuToQML(QTextStream& ostream, DomWidget *w, int level, DomLayoutItem*i)
{
    QString indent(level * 4, ' ');

    ostream << indent << "CSMenu {" << endl;
    layoutItemToQML(ostream, i, level);

    for (DomProperty *v : uniqueProperties(w->elementProperty())) {
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
    layoutItemToQML(ostream, i, level);

    for (DomProperty *v : uniqueProperties(w->elementProperty())) {
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
    layoutItemToQML(ostream, i, level);

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
    for (DomProperty *v : uniqueProperties(w->elementProperty())) {
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

void UI::scriptButtonToQML(QTextStream &ostream, DomWidget*w, int level, DomLayoutItem*i)
{
    QString indent(level * 4, ' ');

    ostream << indent << "CSShellCommand {" << endl;
    layoutItemToQML(ostream, i, level);

    QString command;
    for (DomProperty *v : uniqueProperties(w->elementProperty())) {
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
            ostream << indent << "    label: '-" << v->elementString()->text() << "'" << endl;
        }
        else if (v->attributeName() == "scriptCommand") {
            command += v->elementString()->text();
        }
        else if (v->attributeName() == "scriptParameter") {
            command += "\n\"";
            command += v->elementString()->text();
            command += " \"&";
        }
    }

    if (!command.isEmpty()) {
        ostream << indent << "    model: ListModel {" << endl;
        ostream << indent << "        ListElement {" << endl;
        ostream << indent << "            label: ''" << endl;
        ostream << indent << "            command: '" << command << "'" << endl;
        ostream << indent << "        }" << endl;
        ostream << indent << "    }" << endl;
    }

    ostream << indent << "}" << endl;
}

void UI::shellCommandToQML(QTextStream& ostream, DomWidget *w, int level, DomLayoutItem*i)
{
    QString indent(level * 4, ' ');

    ostream << indent << "CSShellCommand {" << endl;
    layoutItemToQML(ostream, i, level);

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
    for (DomProperty *v : uniqueProperties(w->elementProperty())) {
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
    layoutItemToQML(ostream, i, level);

    for (DomProperty *v : uniqueProperties(w->elementProperty())) {
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
    layoutItemToQML(ostream, i, level);

    for (DomProperty *v : uniqueProperties(w->elementProperty())) {
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
    layoutItemToQML(ostream, i, level);

    for (DomProperty *v : uniqueProperties(w->elementProperty())) {
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
    layoutItemToQML(ostream, i, level);

    for (DomProperty *v : uniqueProperties(w->elementProperty())) {
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

QList<DomProperty*> UI::uniqueProperties(QList<DomProperty*> p)
{
    QList<DomProperty*> u;
    QList<QString> names;
    for (DomProperty *v : p) {
        int index = names.indexOf(v->attributeName());
        if (index == -1) {
            names.append(v->attributeName());
            u.append(v);
        } else {
            u[index] = v;
        }
    }
    return u;
}

void UI::widgetToQML(QTextStream& ostream, DomWidget *w, int level, DomLayoutItem*i)
{
    QString indent(level * 4, ' ');

    QString widgetClass = w->attributeClass();
    if (widgetClass == "QTabWidget")
        tabWidgetToQML(ostream, w, level, i);
    else if (widgetClass == "QPlainTextEdit" || widgetClass == "QTextEdit")
        textEditToQML(ostream, w, level, i);
    else if (widgetClass == "QGroupBox")
        groupBoxToQML(ostream, w, level, i);
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
    else if (widgetClass == "caCartesianPlot")
        cartesianPlotToQML(ostream, w, level, i);
    else if (widgetClass == "caLinearGauge")
        indicatorToQML(ostream, w, level, i);
    else if (widgetClass == "caLed")
        ledToQML(ostream, w, level, i);
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
    else if (widgetClass == "caScriptButton")
        scriptButtonToQML(ostream, w, level, i);
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
        for (DomProperty *v : uniqueProperties(w->elementProperty())) {
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
    else {
        qDebug() << "Use CSRect for widget class " << widgetClass << endl;
        ostream << indent << "CSRect {" << endl;
        ostream << indent << "    implicitHeight: 20" << endl;
        ostream << indent << "    fillStyle: FillStyle.Outline" << endl;

        layoutItemToQML(ostream, i, level);

        for (DomProperty *v : uniqueProperties(w->elementProperty())) {
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

    for (DomProperty *v : uniqueProperties(mainWidget->elementProperty())) {
        if (v->attributeName() == "geometry") {
            rectToQML(ostream, v->elementRect());
        }
        else if (v->attributeName() == "styleSheet") {
            QString css = v->elementString()->text();
            auto match = QRegularExpression("background: rgba?\\((?<r>\\d+), (?<g>\\d+), (?<b>\\d+)(, (?<a>\\d+)\\))?").match(css);
            if (match.hasMatch()) {
                QString background = "#";
                if (!match.captured("a").isEmpty())
                    background += QString("%1").arg(match.captured("a").toShort(), 2, 16, QChar('0'));
                background += QString("%1%2%3")
                    .arg(match.captured("r").toShort(), 2, 16, QChar('0'))
                    .arg(match.captured("g").toShort(), 2, 16, QChar('0'))
                    .arg(match.captured("b").toShort(), 2, 16, QChar('0'));
                ostream << "    color: '" << background << "'" << endl;
            }
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

    ostream << "import QtQuick 2.0\n";
    ostream << "import QtQuick.Layouts 1.0\n";
    ostream << "import QtQuick.Controls 1.0\n";
    ostream << "import CSDataQuick.Components 1.0\n";
    ostream << "import CSDataQuick.Components.Private 1.0\n";
    ostream << "Item {\n";
    ostream << "    anchors.fill: parent\n";

    for (DomWidget *child : orderedChildWidgets(mainWidget))
        widgetToQML(ostream, child);
    
    ostream << "}" << endl;
}