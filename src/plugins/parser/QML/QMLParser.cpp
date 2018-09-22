/*****************************************************************************\
 * Copyright 2018 Paul Scherrer Institut, Villigen, Switzerland
\*****************************************************************************/

#include "QMLParser.h"

#include <QFile>

/*!
 * \class QMLParser
 * \inmodule CSDataQuick.Parser
 * \ingroup csdataquick.parser.plugins
 * \brief Parse CSDataQuick QML files
 *
 * This parser actually only reads the QML file and performs macros expansion.
 */
QMLParser::QMLParser(QObject *parent)
    : QCSParser(parent)
{
}

QMLParser::~QMLParser()
{
}

/*!
 * \reimp
 */
QString QMLParser::extension()
{
    return "qml";
}

/*!
 * \reimp
 */
QString QMLParser::description()
{
    return "CSDataQuick (qml)";
}

/*!
 * \reimp
 */
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
