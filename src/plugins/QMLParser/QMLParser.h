#pragma once

#include "parser.h"

class QMLParser : public QCSParser
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "ch.psi.sls.ParserInterface")
    Q_INTERFACES(QCSParser)

public:
    QMLParser(QObject *parent=0);
    virtual ~QMLParser();

    virtual QString extension();
    virtual QString description();
    virtual QString parseDisplayFile(QString filename, QMap<QString, QString> macros, bool partial);
};
