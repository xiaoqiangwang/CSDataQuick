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

    for(auto it=macros.begin(); it != macros.end(); it++) {
        qml.replace("$("+it.key()+")", it.value());
    }

    return qml;
}
