/*****************************************************************************\
 * Copyright 2012-2015 Paul Scherrer Institut, Villigen, Switzerland
\*****************************************************************************/

#include "ipcserver.h"

#include <QLocalServer>
#include <QLocalSocket>
#include <QBuffer>
#include <QDataStream>

#if defined(Q_OS_MAC) && !defined(Q_OS_IOS)
    #include <Security/AuthSession.h>
#elif defined  Q_WS_X11
    #include <X11/Xlib.h>
    #include <QX11Info>
#elif defined Q_OS_WIN32
    #include <Windows.h>
#endif

// 3 seconds as connection/read/write timeout
const int TIMEOUT = 3000;


IPCHandler :: IPCHandler(quintptr socketDescriptor, QObject *parent)
    : QThread(parent)
    , m_socketDescriptor(socketDescriptor)
{
}

void IPCHandler :: run()
{
    QLocalSocket *client = new QLocalSocket();
    client->setSocketDescriptor(m_socketDescriptor);

    handleClient(client);

    delete client;
}

void IPCHandler :: handleClient(QLocalSocket *client)
{
    QDataStream iostream(client);

    // read message size
    while (client->bytesAvailable() < (int)sizeof(quint16)) {
        if (!client->waitForReadyRead(TIMEOUT)) {
            qWarning() << "IPCHandler: Timeout in read message size info";
            return;
        }
    }
    quint16 blockSize;
    iostream >> blockSize;

    // wait message
    while (client->bytesAvailable() < blockSize) {
        if (!client->waitForReadyRead(TIMEOUT)) {
            qWarning() << "IPCHandler: Timeout in read message";
            return;
        }
    }

    // decode message
    quint16 msgId;
    iostream >> msgId;

    QByteArray reply;
    switch (msgId) {
    case MSG_PING: {
        QByteArray message;
        iostream >> message;
        if (message == "PING")
            reply = "PONG";
        else
            reply = "ERROR";
    }
        break;
    case MSG_DISPATCH: {
        QByteArray message;
        iostream >> message;
        QDataStream ds(&message, QIODevice::ReadOnly);

        QString fileName;
        QString macroString;
        QString geometry;
        ds >> fileName >> macroString >> geometry;
        if (!fileName.isEmpty()) {
            // signal new request
            emit dispatchRequestReceived(fileName, macroString, geometry);
            reply = "OK";
        } else
            reply = "ERROR";
    }
        break;
    default:
        qWarning() << "IPCHandler: Unknown message id" << msgId;
        reply = "ERROR";
    }
    iostream << quint16(reply.size() + sizeof(quint16)) << msgId << reply;
    client->flush();
    client->waitForDisconnected();
}


QString IPCServer :: getServerName() {
    int id = 0;
    QString serverName = "ADLViewer";
#if defined(Q_OS_MAC) && !defined(Q_OS_IOS)
    OSStatus error;
    SecuritySessionId sessionId;
    SessionAttributeBits sessionInfo;
    error = SessionGetInfo(callerSecuritySession, &sessionId, &sessionInfo);
    if (error == noErr)
        id = sessionId;
#elif defined Q_WS_X11
    id = qChecksum(DisplayString(QX11Info::display()));
#elif defined Q_OS_WIN32
    DWORD sessionId = 0;
    BOOL status = 0;
    status = ProcessIdToSessionId(GetCurrentProcessId(), &sessionId);
    if (status)
        id = sessionId;
#endif
    return serverName + "-" + QString::number(id, 16);
}

bool IPCServer :: launchServer(bool cleanup)
{
    if (isListening())
        return true;

    QString name = IPCServer::getServerName();

    if (cleanup)
        QLocalServer::removeServer(name);

    if (listen(name)) {
        return true;
    } else {
        qWarning() << "IPCServer::launchServer error:" << errorString();
        return false;
    }
}

QByteArray IPCClient :: writeAndRead(QLocalSocket *socket, quint16 messageId, QByteArray messageOut)
{
    QDataStream iostream(socket);

    iostream << quint16(messageOut.size() + sizeof(quint16)) << messageId << messageOut;

    // IPCClient runs without event loop, it might be good to flush the socket buffer.
    socket->flush();

    // read message size
    while (socket->bytesAvailable() < (int)sizeof(quint16)) {
        if (!socket->waitForReadyRead(TIMEOUT)) {
            qWarning() << "IPCClient::writeAndRead: Timeout when read message size";
            return QByteArray();
        }
    }
    quint16 blockSize;
    iostream >> blockSize;

    // wait for message body
    while (socket->bytesAvailable() < blockSize) {
        if (!socket->waitForReadyRead(TIMEOUT)) {
            qWarning() << "IPCClient::writeAndRead: Timeout when read message body";
            return QByteArray();
        }
    }

    // read message id
    quint16 id;
    iostream >> id;
    if (id != messageId) {
        qWarning() << "IPCClient::writeAndRead: Received message id is not the same as request";
        return QByteArray();
    }

    // read message body
    QByteArray messageIn;
    iostream >> messageIn;

    return messageIn;
}

bool IPCClient :: requestDispatch(QString fileName, QString macroString, QString geometry) {
    QByteArray message;

    QDataStream ostream(&message, QIODevice::WriteOnly);
    ostream << fileName << macroString << geometry;

    QString name = IPCServer::getServerName();
    QLocalSocket client;
    client.connectToServer(name);
    if (client.waitForConnected(TIMEOUT)) {
        QByteArray reply = writeAndRead(&client, MSG_DISPATCH, message);
        client.disconnectFromServer();
        client.close();
        return (reply == "OK");
    } else {
        qWarning() << "IPCClient::requestDispatch: Timeout when connect to server";
        return false;
    }
}

bool IPCClient :: checkExistence() {
    QByteArray message = "PING";

    QString name = IPCServer::getServerName();
    QLocalSocket client;
    client.connectToServer(name);
    if (client.waitForConnected(TIMEOUT)) {
        QByteArray reply = writeAndRead(&client, MSG_PING, message);
        client.disconnectFromServer();
        client.close();
        return (reply == "PONG");
    } else {
        qWarning() << "IPCClient::checkExistence: Timeout when connect to server";
        return false;
    }
}

void IPCServer :: incomingConnection(quintptr socketDescriptor)
{
    IPCHandler *handler = new IPCHandler(socketDescriptor);
    connect(handler, SIGNAL(finished()), handler, SLOT(deleteLater()));
    connect(handler, &IPCHandler::dispatchRequestReceived, this, &IPCServer::dispatchRequestReceived);
    handler->start();
}
