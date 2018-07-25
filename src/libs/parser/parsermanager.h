#pragma once

#include <QObject>
#include "parser_global.h"

#define parserManager QCSParserManager::instance()

class QCSParser;

class PARSER_EXPORT QCSParserManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QList<QCSParser*> parsers MEMBER _parsers CONSTANT)

public:
    static QCSParserManager *instance();
    ~QCSParserManager();
    QCSParser *parserForExtension(QString source) const;
    Q_INVOKABLE QStringList supportedExtensions() const;

private:
    QCSParserManager(QObject *parent=0);

    static QCSParserManager *_manager;

    QList<QCSParser*> _parsers;
};
