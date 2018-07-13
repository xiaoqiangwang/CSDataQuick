#include <QFileInfo>
#include <QtDebug>

#include <iostream>

#include "EDLParser.h"
#include "ADLParser.h"
#include "UIParser.h"

int main(int argc, char **argv)
{
    QFileInfo file(argv[1]);

    std::string qml;
    
    if (file.suffix() == "adl")
        qml = parseADLDisplay(file.absoluteFilePath().toStdString(), std::map<std::string, std::string>());
    else if (file.suffix() == "edl")
        qml = parseEDLDisplay(file.absoluteFilePath().toStdString(), std::map<std::string, std::string>());
    else if (file.suffix() == "ui")
        qml = parseUIDisplay(file.absoluteFilePath().toStdString(), std::map<std::string, std::string>());
    else {
        qCritical() << file.absoluteFilePath() << " is not supported";
        return -1;
    }

    std::cout << qml << std::endl;
}
