/*****************************************************************************\
 * Copyright 2012-2018 Paul Scherrer Institut, Villigen, Switzerland
\*****************************************************************************/

#include "ipcserver.h"

#include <sstream>
#include <iostream>

#include <QApplication>
#include <QCommandLineParser>
#include <QFile>
#include <QDir>
#include <QLocalServer>
#include <QLocalSocket>
#include <QPointer>

#include <QQmlEngine>
#include <QQmlContext>
#include <QQmlComponent>
#include <QQuickWindow>
#include <QMutex>

#include <QQmlDebuggingEnabler>
#include <QtDebug>

#include "parsermanager.h"
#include "parser.h"
#include "cs_global.h"

static QPointer<QQuickWindow> window;

void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{

    static QMutex recursiveMutex(QMutex::Recursive);
    static QMutex nonRecursiveMutex(QMutex::NonRecursive);

    // Prevent multiple threads from calling this method simultaneoulsy.
    // But allow recursive calls, which is required to prevent a deadlock
    // if the logger itself produces an error message.
    recursiveMutex.lock();

    // Fall back to stderr when this method has been called recursively.
    if (window && nonRecursiveMutex.tryLock()) {
        QString formatedMessage = QString("%s (%s:%u %s)").asprintf(msg.toLocal8Bit(),
                                                                   context.file,
                                                                   context.line,
                                                                   context.function);
        QMetaObject::invokeMethod(window, "outputMessage",
                                  Q_ARG(QVariant, type),
                                  Q_ARG(QVariant, formatedMessage));
        nonRecursiveMutex.unlock();
    }
    else {
        if (!msg.isEmpty())
            fprintf(stderr, "Error: %s\n",
                msg.toLocal8Bit().constData());
    }

    recursiveMutex.unlock();
}

int main(int argc, char **argv)
{

    //enable the QML debugger only if -qmljsdebugger is passed as a command line arg
    //this must be called before the QApplication constructor
    for(int i=0; i<argc; i++) {
        if (QByteArray(argv[i]).startsWith("-qmljsdebugger"))
            QQmlDebuggingEnabler enabler;
    }

    // It is enough to use QGuiApplication mostly.
    // However the print dialog depends still on QtWidgets and thus QApplication.
    QApplication app(argc, argv);
    app.setOrganizationName("Paul Scherrer Institut");
    app.setOrganizationDomain("psi.ch");
    app.setApplicationName("Viewer");
    app.setApplicationVersion(CSDATAQUICK_VERSION);

    qRegisterMetaType<QtMsgType>("QtMsgType");
    QString supportedExtensions = parserManager->supportedExtensions().join("/");

    QCommandLineParser parser;
    parser.setSingleDashWordOptionMode(QCommandLineParser::ParseAsLongOptions);
    parser.setApplicationDescription(supportedExtensions + " viewer");
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("file", supportedExtensions + " file");

    // A boolean option indicating execution mode (-x, --execute)
    QCommandLineOption execOption(QStringList() << "x" << "execute", "Open in execute mode.");
    parser.addOption(execOption);

    // A boolean option indicating conversion mode (-c, --convert)
    QCommandLineOption convertOption(QStringList() << "c" << "convert", "Open in conversion mode.");
    parser.addOption(convertOption);

    // A boolean option indicating editing mode (-e, --edit)
    QCommandLineOption editOption(QStringList() << "e" << "edit", "Open in editor mode.");
    parser.addOption(editOption);

    // A boolean option indicating launch mode (-attach)
    QCommandLineOption attachOption(QStringList() << "a" << "attach", "Attach to existing viewer.");
    parser.addOption(attachOption);

    // A boolean option indicating launch mode (-local)
    QCommandLineOption localOption(QStringList() << "l" << "local", "Do not use existing viewer or be available as existing viewer.");
    parser.addOption(localOption);

    // A boolean option indicating launch mode (-cleanup)
    QCommandLineOption cleanupOption(QStringList() << "cleanup", "Use this viewer as existing viewer.");
    parser.addOption(cleanupOption);

    // A boolean option indicating launch mode (-noMsg)
    QCommandLineOption noMsgOption(QStringList() << "noMsg", "Do not raise the message window");
    parser.addOption(noMsgOption);

    // An option with marco substitution
    QCommandLineOption macroOption(QStringList() << "m" << "macro",
                QCoreApplication::translate("main", "Macro definition <macros>."),
                QCoreApplication::translate("main", "macros"));
    parser.addOption(macroOption);

    // An option with window geometry
    QCommandLineOption geometryOption(QStringList() << "dg" << "displayGeometry",
                QCoreApplication::translate("main", "[[width][xheight]][+xoffset[+yoffset]]."),
                QCoreApplication::translate("main", "geometry"));
    parser.addOption(geometryOption);

    // Process the actual command line arguments given by the user
    parser.process(app.arguments());

    // macros
    QString macroString = parser.value(macroOption);

    // geomtry
    QString geometry = parser.value(geometryOption);

    // EPICS_DISPLAY_PATH
    QList<QByteArray> filePaths = qgetenv("EPICS_DISPLAY_PATH").split(':');
    QDir::addSearchPath("displays", QDir::currentPath());
    foreach (QByteArray path, filePaths) {
        QDir::addSearchPath("displays", path);
    }

    // display files is in args
    const QStringList args = parser.positionalArguments();

    // Do conversion
    if (parser.isSet(convertOption)) {
        foreach (QString arg, args) {
            QFileInfo fi(arg);
            if (fi.isRelative()) {
                fi.setFile("displays:" + arg);
            }
            if (!fi.exists()) {
                qWarning() << "Cannot find file " << arg;
                continue;
            }
            QCSParser *parser = parserManager->parserForExtension(fi.suffix());
            if (!parser) {
                qCritical() << fi.absoluteFilePath() << " is not supported";
                continue;
            }

            QString qml = parser->parseDisplayFile(fi.absoluteFilePath(), QMap<QString, QString>());
            if (!qml.isEmpty()) {
                QFile file(fi.baseName() + ".qml");
                if (file.open(QIODevice::WriteOnly | QIODevice::Text))
                    file.write(qml.toUtf8());
            }
        }
        app.quit();
        return 0;
    }
    // Do remote protocol if local option is not specified
    if (parser.isSet(attachOption)) {
        bool existing = IPCClient::checkExistence();
        if (existing) {
            qDebug() << "Attaching to existing Viewer";
            foreach(QString arg, args) {
                if (IPCClient::requestDispatch(arg, macroString, geometry))
                    qDebug() << "  Dispatched: " << arg << macroString << geometry;
                else
                    qDebug() << "  Dispatch failed for " << arg;
            }
            app.quit();
            return 0;
        } else {
            qWarning() << "\nCannot connect to existing Viewer because it is invalid\n"
                       << "  Continuing with this one as if -cleanup were specified\n";
        }
    }

    QQmlEngine *engine = new QQmlEngine();
    engine->rootContext()->setContextProperty("app", &app);
    engine->rootContext()->setContextProperty("serverMode", QVariant(false));
#ifdef Q_OS_MAC
    engine->addImportPath(QGuiApplication::applicationDirPath() + "/../../../../qml/");
#else
    engine->addImportPath(QGuiApplication::applicationDirPath() + "/../qml/");
#endif
    QQmlComponent component(engine, QUrl("qrc:/main.qml"));
    while(!component.isReady()) {
        if (component.isError()) {
            foreach(QQmlError error, component.errors())
                qCritical() << error;
            return -1;
        }
    }
    window = qobject_cast<QQuickWindow *>(component.create(engine->rootContext()));
    if(window.isNull()) {
        qCritical() << "Failed to create main window";
        return -1;
    }

    qInstallMessageHandler(myMessageOutput);

    qDebug().noquote() << app.applicationName() << "started with PID:" << app.applicationPid();

    if (parser.isSet(noMsgOption))
        window->showMinimized();
    else
        window->show();

    if (parser.isSet(attachOption) || parser.isSet(cleanupOption)) {
        IPCServer *server = new IPCServer();
        if (server->launchServer(true)) {
            engine->rootContext()->setContextProperty("serverMode", QVariant(true));
            QObject::connect(server, SIGNAL(dispatchRequestReceived(QVariant,QVariant,QVariant)),
                         window, SLOT(dispatchRequestReceived(QVariant,QVariant,QVariant)));
        }
        else
            qWarning() << "Failed to start IPC server";
    }

    foreach (QString fileName, args) {
        QMetaObject::invokeMethod(window, "createDisplay",
                             Q_ARG(QVariant, fileName),
                             Q_ARG(QVariant, macroString),
                             Q_ARG(QVariant, geometry));
    }

    return app.exec();
}
