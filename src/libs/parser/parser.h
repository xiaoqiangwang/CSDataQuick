#pragma once

#include <QString>
#include <QVariant>
#include <QObject>

#include "parser_global.h"

class QCSParser
{
public:
    virtual ~QCSParser() = default;

    virtual QString extension() = 0;
    virtual QString description() = 0;
    virtual QString parseDisplayFile(QString filename, QMap<QString, QString> macros, bool partial=false) = 0;
};

Q_DECLARE_INTERFACE(QCSParser, "ch.psi.sls.Parser/1.0")
