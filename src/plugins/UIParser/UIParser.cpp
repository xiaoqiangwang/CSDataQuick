#include "UIParser.h"
#include "ui_parser.h"

#include <QFile>

UIParser::UIParser(QObject *parent)
    : QCSParser(parent)
{
}

UIParser::~UIParser()
{
}

QString UIParser::extension()
{
    return "ui";
}

QString UIParser::description()
{
    return "caQtDM UI(ui)";
}

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
