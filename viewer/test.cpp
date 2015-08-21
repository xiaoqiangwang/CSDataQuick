#include <sstream>

#include <QtCore/QFile>
#include <QtDebug>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlComponent>
#include <QQuickView>

#include <fstream>
#include <sstream>

#include "common.h"
#include "element.h"
#include "fileinfo.h"

int main(int argc, char **argv)
{
    std::fstream ifstream;
    std::string filename = argv[1];

    FileInfo fi = FileInfo("").getFile(filename);
    ifstream.open(fi.absolutePath().c_str(), std::fstream::in);

    Display displayInfo(0);
    displayInfo.setFileName(fi.absolutePath());
    displayInfo.parse(ifstream);

    std::ostringstream osstream;
    displayInfo.toQML(osstream);
    osstream.flush();

    QGuiApplication app(argc, argv);
    QQmlEngine engine;
    engine.addImportPath("..");

    QQmlComponent component(&engine);
    component.setData(osstream.str().c_str(), QUrl());
    while(!component.isReady()) ;
    QWindow * window = qobject_cast<QQuickWindow *>(component.create());
    window->show();

    return app.exec();
}
