/*****************************************************************************\
 * Copyright 2018 Paul Scherrer Institut, Villigen, Switzerland
\*****************************************************************************/

#include "ADLParser.h"
#include "element.h"

#include <fstream>
#include <sstream>

/*!
 * \class ADLParser
 * \inmodule CSDataQuick.Parser
 * \ingroup csdataquick.parser.plugins
 * \brief Parse MEDM ADL files
 *
 * ADL (ASCII Display List) files are produced by MEDM (Motif Editor and Display Manager).
 * All graphical objects are well supported.
 */
ADLParser::ADLParser(QObject *parent)
    : QCSParser(parent)
{
}

ADLParser::~ADLParser()
{
}

/*!
 * \reimp
 */
QString ADLParser::extension()
{
    return "adl";
}

/*!
 * \reimp
 */
QString ADLParser::description()
{
    return "MEDM ASCII Display List (adl)";
}

/*!
 * \reimp
 */
QString ADLParser::parseDisplayFile(QString filename, QMap<QString, QString> macros, bool partial)
{
    Display displayInfo(0);
    displayInfo.setFileName(filename.toStdString());

    // QMap to std::map
    std::map<std::string, std::string> smacros;
    for(auto it = macros.begin(); it != macros.end(); it++)
        smacros[it.key().toStdString()] = it.value().toStdString();
    displayInfo.setMacros(smacros);

    std::ifstream ifstream(filename.toStdString().c_str());
    displayInfo.parse(ifstream);

    std::ostringstream osstream;
    if (partial)
        displayInfo.toPartialQML(osstream);
    else
        displayInfo.toQML(osstream);
    osstream.flush(); 

    return QString::fromStdString(osstream.str());
}
