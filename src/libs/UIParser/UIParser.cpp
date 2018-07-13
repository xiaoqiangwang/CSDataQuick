#include "UIParser.h"
#include "parser.h"

#include <sstream>

#include <QFile>

std::string parseUIDisplay(std::string filename, std::map<std::string, std::string> macros)
{
    UI displayInfo;
    displayInfo.setFileName(filename);
    displayInfo.setMacros(macros);

    QFile file(filename.c_str());
    if (!file.open(QIODevice::ReadOnly))
        return "";

    QXmlStreamReader reader(&file);
    displayInfo.parse(reader);

    QString qml;
    QTextStream ostream(&qml);
    displayInfo.toQML(ostream);

    return qml.toStdString();
}

std::string parseUIComposite(std::string filename, std::map<std::string, std::string> macros)
{
    UI displayInfo;
    displayInfo.setFileName(filename);
    displayInfo.setMacros(macros);

    QFile file(filename.c_str());
    if (!file.open(QIODevice::ReadOnly))
        return "";

    QXmlStreamReader reader(&file);
    displayInfo.parse(reader);

    QString qml;
    QTextStream ostream(&qml);
    displayInfo.toPartialQML(ostream);

    return qml.toStdString();
}
