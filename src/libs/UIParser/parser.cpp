#include <QtDebug>

#include "parser.h"
#include "ui4.h"

UI::UI()
{
    ui = nullptr;
}

UI::~UI()
{
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

    ostream << "#" << hex << qSetFieldWidth(2) 
        << v->elementRed()
        << v->elementGreen()
        << v->elementBlue();

    if (v->hasAttributeAlpha())
        ostream << v->attributeAlpha();

    return color;
}

void UI::fontToQML(QTextStream& ostream, DomFont *v, int level)
{
    QString indent(level * 4, ' ');

    if (v->hasElementFamily())
        ostream << indent << "    font.family: '" << v->elementFamily() << "'" << endl;
    if (v->hasElementPointSize())
        ostream << indent << "    font.pointSize: " << v->elementPointSize() << endl;
    if (v->hasElementWeight())
        ostream << indent << "    font.weight: " << v->elementWeight() << endl;
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
        }
    }

    if (w->attributeClass() == "QTabWidget")
        ostream << indent << "    Tab {" << endl;

    for (DomWidget *child : w->elementWidget()) {
        widgetToQML(ostream, child, level+1);
        if (w->attributeClass() == "QTabWidget")
            ostream << indent << "    }" << endl;
    }
    ostream << indent << "}" << endl;
}


void UI::widgetToQML(QTextStream& ostream, DomWidget *w, int level, DomLayoutItem*i)
{
    QString indent(level * 4, ' ');

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

    for (DomWidget *child : w->elementWidget())
        widgetToQML(ostream, child, level+1);

    ostream << indent << "}" << endl;
}

void UI::toQML(QTextStream& ostream)
{
    ostream << "import QtQuick 2.0\n";
    ostream << "import QtQuick.Layouts 1.0\n";
    ostream << "import QtQuick.Controls 1.0\n";
    ostream << "import CSDataQuick.Components 1.0\n";
    ostream << "BaseWindow {\n";

    DomWidget *mainWidget = ui->elementWidget();
    if (!mainWidget)
        return;

    for (DomProperty *v : mainWidget->elementProperty()) {
        if (v->attributeName() == "geometry") {
            rectToQML(ostream, v->elementRect());
        }
    }

    for (DomWidget *child : mainWidget->elementWidget())
        widgetToQML(ostream, child, 1);

    ostream << "}" << endl;
}

void UI::toPartialQML(QTextStream& ostream)
{
    DomWidget *mainWidget = ui->elementWidget();
    if (!mainWidget)
        return;

    for (DomWidget *child : mainWidget->elementWidget())
        widgetToQML(ostream, child);
}
