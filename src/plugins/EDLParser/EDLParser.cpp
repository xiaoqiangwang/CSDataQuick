#include "EDLParser.h"
#include "edl_parser.h"

#include <QDateTime>
#include <QCoreApplication>

#include <fstream>
#include <sstream>

/*!
 * \class EDLParser
 * \inmodule CSDataQuick.Parser
 * \ingroup csdataquick.parser.plugins
 * \brief Parse EDM (Extensible Display Manager) files
 *
 * This parser only supports 4.0 version of EDL files. The unspported features are:
 * \list
 * \li CALC Pv
 * \li Dynamic color rules
 * \li Dynamic symbol
 * \endlist
 */
EDLParser::EDLParser(QObject *parent)
    : QCSParser(parent)
{
}

EDLParser::~EDLParser()
{
}

/*!
 * \reimp
 */
QString EDLParser::extension()
{
    return "edl";
}

/*!
 * \reimp
 */
QString EDLParser::description()
{
    return "EDM Display List (edl)";
}

/*!
 * \reimp
 */
QString EDLParser::parseDisplayFile(QString filename, QMap<QString, QString> macros, bool partial)
{
    Screen displayInfo;
    displayInfo.setFileName(filename.toStdString());

    // add special macros
    //  !A - application specific
    //  !W - window specific
    macros["!A"] = QString::number(qApp->applicationPid(), 16);
    macros["!W"] = QString::number(QDateTime::currentMSecsSinceEpoch(), 16);
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
