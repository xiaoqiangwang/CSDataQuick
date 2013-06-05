#include <QGuiApplication>
#include "qtquick2applicationviewer.h"
#include "pvobject.h"
#include <QtDebug>
#include <QQmlEngine>
#include <QQmlComponent>

Q_DECL_EXPORT int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    qmlRegisterType<PvObject>("PvLibrary",1,0,"PvObject");

    QtQuick2ApplicationViewer viewer;
    viewer.setMainQmlFile(QLatin1String("qml/qml2/main.qml"));
    viewer.showExpanded();

    return app.exec();
}
