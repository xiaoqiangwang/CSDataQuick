#pragma once

#include <QString>
#include <QVariant>
#include <QObject>

#include "parser_global.h"

class PARSER_EXPORT QCSParser : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString extension READ extension CONSTANT)
    Q_PROPERTY(QString description READ description CONSTANT)

public:
    QCSParser(QObject *parent);
    virtual ~QCSParser() {}

    virtual QString extension() = 0;
    virtual QString description() = 0;
    virtual QString parseDisplayFile(QString filename, QMap<QString, QString> macros, bool partial=false) = 0;
};

Q_DECLARE_INTERFACE(QCSParser, "ch.psi.sls.Parser/1.0")
