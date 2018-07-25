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
    UI displayInfo;
    displayInfo.setFileName(filename.toStdString());

    // QMap to std::map
    std::map<std::string, std::string> smacros;
    for(auto it = macros.begin(); it != macros.end(); it++)
        smacros[it.key().toStdString()] = it.value().toStdString();
    displayInfo.setMacros(smacros);

    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly))
        return "";

    QXmlStreamReader reader(&file);
    displayInfo.parse(reader);

    QString qml;
    QTextStream ostream(&qml);

    if (partial)
        displayInfo.toPartialQML(ostream);
    else
        displayInfo.toQML(ostream);

    return qml;
}
