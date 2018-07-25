#include <QFileInfo>
#include <QTextStream>

#include <QtDebug>

#include "parsermanager.h"
#include "parser.h"

int main(int argc, char **argv)
{
    QFileInfo file(argv[1]);

    QCSParser *parser = parserManager->parserForExtension(file.suffix());

    if (!parser) {
        qCritical() << file.absoluteFilePath() << " is not supported";
        return -1;
    }

    QString qml = parser->parseDisplayFile(file.absoluteFilePath(), QMap<QString, QString>());

    QTextStream(stdout) << qml;
}
