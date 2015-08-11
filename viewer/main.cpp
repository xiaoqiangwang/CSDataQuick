
#include "common.h"
#include "viewer.h"

#include <sstream>
#include <iostream>

#include <QApplication>
#include <QCommandLineParser>
#include <QFile>
#include <QRect>
#include <QPushButton>
#include <QDir>
#include <QLocalServer>
#include <QLocalSocket>

#include <QtDebug>

class IPCServer : public QObject
{
    Q_OBJECT
public:
    IPCServer(QObject *parent):QObject(parent) {
        m_server = new QLocalServer(this);
        m_server->listen("ADLViewer");
        connect(m_server, &QLocalServer::newConnection,
                this, &IPCServer::onNewConnection);
    }

    void requestDispatch(QString fileName, QMap<QString, QString> macro, QRect geometry) {
        QByteArray message;
        QDataStream stream(message);
        stream << fileName << macro << geometry;

        QLocalSocket client;
        client.connectToServer("ADLViewer");
        client.write(message);
        client.flush();
        client.close();
    }

public slots:
    void onNewConnection() {
        while (QLocalSocket *client = m_server->nextPendingConnection()) {
            QByteArray message = client->readAll();
            if (message.isEmpty())
                continue;
            QDataStream stream(message);
            QString fileName;
            QMap<QString, QString> macro;
            QRect geometry;
            stream >> fileName >> macro >> geometry;
            emit dispatchRequested(fileName, macro, geometry);
        }
    }
signals:
    void dispatchRequested(QString fileName, QMap<QString, QString> macro, QRect geometry);

private:
    QLocalServer *m_server;
};

/* From QGuiApplication.cpp */
struct QWindowGeometrySpecification
{
    Qt::Corner corner;
    int xOffset;
    int yOffset;
    int width;
    int height;
};

// Parse a token of a X11 geometry specification "200x100+10-20".
static inline int nextGeometryToken(const QByteArray &a, int &pos, char *op)
{
    *op = 0;
    const int size = a.size();
    if (pos >= size)
        return -1;

    *op = a.at(pos);
    if (*op == '+' || *op == '-' || *op == 'x')
        pos++;
    else if (isdigit(*op))
        *op = 'x'; // If it starts with a digit, it is supposed to be a width specification.
    else
        return -1;

    const int numberPos = pos;
    for ( ; pos < size && isdigit(a.at(pos)); ++pos) ;

    bool ok;
    const int result = a.mid(numberPos, pos - numberPos).toInt(&ok);
    return ok ? result : -1;
}

QRect parseGeometry(const QByteArray &a)
{
    QWindowGeometrySpecification result;
    result.width = -1; result.height = -1;
    result.xOffset = -1; result.yOffset = -1;

    int pos = 0;
    for (int i = 0; i < 4; ++i) {
        char op;
        const int value = nextGeometryToken(a, pos, &op);
        if (value < 0)
            break;
        switch (op) {
        case 'x':
            (result.width >= 0 ? result.height : result.width) = value;
            break;
        case '+':
        case '-':
            if (result.xOffset >= 0) {
                result.yOffset = value;
                if (op == '-')
                    result.corner = result.corner == Qt::TopRightCorner ? Qt::BottomRightCorner : Qt::BottomLeftCorner;
            } else {
                result.xOffset = value;
                if (op == '-')
                    result.corner = Qt::TopRightCorner;
            }
        }
    }

    return QRect(result.xOffset, result.yOffset, result.width, result.height);
}

QMap<QString, QString> parseMacro(QString macro)
{
    QMap<QString, QString> macroMap;
    foreach(QString m, macro.split(',')) {
        QStringList paires = m.split('=');
        if (paires.length() == 2)
            macroMap.insert(paires[0], paires[1]);
        else
            qDebug() << "macro unclear" << m;
    }

    return macroMap;
}

int main(int argc, char **argv)
{
    QApplication a(argc, argv);
    QApplication::setApplicationName("adl-viewer");
    QApplication::setApplicationVersion("0.1");

    QCommandLineParser parser;
    parser.setSingleDashWordOptionMode(QCommandLineParser::ParseAsLongOptions);
    parser.setApplicationDescription("adl viewer");
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("adl file", QCoreApplication::translate("main", "adl file to open."));

    // A boolean option indicating execution mode (-x, --execute)
    QCommandLineOption execOption(QStringList() << "x" << "execute", "Open in execute mode.");
    parser.addOption(execOption);

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
    QCommandLineOption cleanupOption(QStringList() << "c" << "cleanup", "Use this viewer as existing viewer.");
    parser.addOption(cleanupOption);

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
    parser.process(a);

    // macros
    QMap<QString, QString> marcoMap = parseMacro(parser.value(macroOption));

    // geomtry
    QRect geometry = parseGeometry(parser.value(geometryOption).toLatin1());

    // EPICS_DISPLAY_PATH
    QList<QByteArray> filePaths = qgetenv("EPICS_DISPLAY_PATH").split(':');
    QDir::addSearchPath("displays", QDir::currentPath());
    foreach (QByteArray path, filePaths) {
        QDir::addSearchPath("displays", path);
    }

    // adl files is in args
    const QStringList args = parser.positionalArguments();
    foreach (QString arg, args) {
        QFile file;
        if (QDir::isAbsolutePath(arg))
            file.setFileName(arg);
        else
            file.setFileName("displays:" + arg);
        if (!file.open(QIODevice::ReadOnly)) {
            qDebug() << "Unable to open " << arg;
            continue;
        }
        std::string input(file.readAll().data());
        std::istringstream isstream(input);
        char token[MAX_TOKEN_LENGTH];

        //while (getToken(isstream, token) != T_EOF)
        //    std::cout << token << std::endl;
    }

    Viewer viewer;
    viewer.show();
    return a.exec();
}
