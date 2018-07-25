#pragma once

#include "parser.h"

class ADLParser : public QCSParser
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "ch.psi.sls.ParserInterface")
    Q_INTERFACES(QCSParser)

public:
    ADLParser(QObject *parent=0);
    virtual ~ADLParser();

    virtual QString extension();
    virtual QString description();
    virtual QString parseDisplayFile(QString filename, QMap<QString, QString> macros, bool partial=false);
};


