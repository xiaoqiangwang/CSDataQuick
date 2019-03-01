#pragma once

#include "parser.h"

class UIParser : public QCSParser
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "ch.psi.sls.ParserInterface")
    Q_INTERFACES(QCSParser)

public:
    UIParser(QObject *parent=Q_NULLPTR);
    virtual ~UIParser();

    virtual QString extension();
    virtual QString description();
    virtual QString parseDisplayFile(QString filename, QMap<QString, QString> macros, bool partial);
};
