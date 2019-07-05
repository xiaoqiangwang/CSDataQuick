#pragma once

#include "parser.h"

class EDLParser : public QObject,
                  public QCSParser
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "ch.psi.sls.ParserInterface")
    Q_INTERFACES(QCSParser)

public:
    EDLParser(QObject *parent=Q_NULLPTR);
    virtual ~EDLParser();

    virtual QString extension();
    virtual QString description();
    virtual QString parseDisplayFile(QString filename, QMap<QString, QString> macros, bool partial=false);
};
