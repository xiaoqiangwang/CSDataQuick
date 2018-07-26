#include "QMLParser.h"

#include <QFile>

QMLParser::QMLParser(QObject *parent)
    : QCSParser(parent)
{
}

QMLParser::~QMLParser()
{
}

QString QMLParser::extension()
{
    return "qml";
}

QString QMLParser::description()
{
    return "CSDataQuick (qml)";
}

QString QMLParser::parseDisplayFile(QString filename, QMap<QString, QString> macros, bool partial)
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return "";

    QString qml = file.readAll();

    for(auto m : macros) {
        qml.replace("$("+m+")", macros[m]);
    }

    return qml;
}
