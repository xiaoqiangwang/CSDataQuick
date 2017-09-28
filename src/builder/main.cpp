#include <QGuiApplication>
#include <QQmlApplicationEngine>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
#ifdef Q_OS_MAC
    engine.addImportPath(QGuiApplication::applicationDirPath() + "/../../../../qml/");
#else
    engine.addImportPath(QGuiApplication::applicationDirPath() + "/../qml/");
#endif
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}

