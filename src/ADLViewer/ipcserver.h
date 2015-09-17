#ifndef IPCSERVER_H
#define IPCSERVER_H

class QLocalSocket;

#include <QString>
#include <QThread>
#include <QLocalServer>

enum MessageID {
    MSG_PING = 1593, // Magic Number
    MSG_DISPATCH
};

class IPCHandler : public QThread
{
    Q_OBJECT

public:
    IPCHandler(quintptr socketDescriptor, QObject *parent=0);
    void run();

signals:
    void dispatchRequestReceived(QString fileName, QString macroString, QString geometry);

protected:
    void handleClient(QLocalSocket *client);

private:
    quintptr m_socketDescriptor;
};

class IPCClient
{
public:
    static bool checkExistence();
    static bool requestDispatch(QString fileName, QString macroString, QString geometry);

protected:
    static QByteArray writeAndRead(QLocalSocket *socket, quint16 messageId, QByteArray messageOut);
};

class IPCServer : public QLocalServer
{
    Q_OBJECT

public:
    bool launchServer(bool cleanup=false);

signals:
    void dispatchRequestReceived(QString fileName, QString macroString, QString geometry);

protected:
    virtual void incomingConnection(quintptr socketDescriptor);
    static QString getServerName();

friend class IPCClient;
};

#endif // IPCSERVER_H
