#include "UIParser.h"
#include "ui_parser.h"

#include <QFile>

/*!
 * \class UIParser
 * \inmodule CSDataQuick.Parser
 * \ingroup csdataquick.parser.plugins
 * \brief Parse caQtDM UI files
 *
 * The parser from Qt tool uic is used to parse the UI file. Then it traverses the widget tree and
 * creates their QML equilents.
 *
 * The parser converts well most widgets, with the following exceptions:
 * \list
 * \li CaCalc
 * \li Complex nested layout
 * \li signal/slot definitions
 * \endlist
 */
UIParser::UIParser(QObject *parent)
    : QCSParser(parent)
{
}

UIParser::~UIParser()
{
}

/*!
 * \reimp
 */
QString UIParser::extension()
{
    return "ui";
}

/*!
 * \reimp
 */
QString UIParser::description()
{
    return "caQtDM UI(ui)";
}

/*!
 * \reimp
 */
QString UIParser::parseDisplayFile(QString filename, QMap<QString, QString> macros, bool partial)
{

    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly))
        return "";

    QXmlStreamReader reader(&file);
    UI displayInfo;
    displayInfo.parse(reader);

    QString qml;
    QTextStream ostream(&qml);

    if (partial)
        displayInfo.toPartialQML(ostream);
    else
        displayInfo.toQML(ostream);

    for(auto it=macros.begin(); it != macros.end(); it++) {
        qml.replace("$("+it.key()+")", it.value());
    }

    return qml;
}
