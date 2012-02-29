#include <qdeclarative.h>
#include <QtGui/QApplication>
#include "qmlapplicationviewer.h"
#include "pvobject.h"
#include <QtDebug>

Q_DECL_EXPORT int main(int argc, char *argv[])
{
    QScopedPointer<QApplication> app(createApplication(argc, argv));

    qmlRegisterType<PvObject>("PvLibrary",1,0,"PvObject");

    QmlApplicationViewer viewer;
    viewer.setOrientation(QmlApplicationViewer::ScreenOrientationAuto);
    viewer.setMainQmlFile(QLatin1String("qml/qml2/main.qml"));
    viewer.showExpanded();

    return app->exec();
}
