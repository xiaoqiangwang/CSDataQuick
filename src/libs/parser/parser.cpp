/*****************************************************************************\
 * Copyright 2018 Paul Scherrer Institut, Villigen, Switzerland
\*****************************************************************************/

#include "parser.h"

/*!
    \class QCSParser
    \inmodule CSDataQuick.Parser
    \brief The QCSParser class defines the interface of display file parser.

    This is the base class for display file parsers, which parses various display files and output equivalent QML files.

    Pasers are loaded as plugins on demand and parse zero, one or more types of file.
*/

/*!
    \property QCSParser::extension
    File extension supported by this parser.
*/

/*!
    \property QCSParser::description
    Descriptive text of this file parser.
*/

/*!
    \fn QString QCSParser::parseDisplayFile(QString filename, QMap<QString, QString> macros, bool partial)
    Parse the given display file.

    The file parser must implements this to parse the given \a filename with \a macros substitution.
    The returned QML representation should be a standard QML source, unless \a partial is specified
    in which only the item definitions are returned excluding import statements and root item.
*/
