#include "mjpeg.h"

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

MJPEG::MJPEG(QObject *parent) : QObject(parent), _reply(0)
{
    qDebug() << parent;
    _network = new QNetworkAccessManager(this);
    connect(this, SIGNAL(destroyed(QObject*)), this, SLOT(done(QObject*)));
}

MJPEG::~MJPEG()
{
    printf("MJPEG deleted\n");
}

void MJPEG::setSource(QString source)
{
    if (source.isEmpty()) {
        if (_reply)
            _reply->abort();
        return;
    }

    _reply = _network->get(QNetworkRequest(QUrl(source)));
    _state = Header;
    connect(_reply, SIGNAL(readyRead()), this, SLOT(readStream()));
}

void MJPEG::readStream()
{
    QByteArray msg;
    if (_state == Header) {
        while(_reply->canReadLine()) {
            msg =_reply->readLine();
            if (msg.isEmpty()) {
                break;
            } else if (msg.startsWith("Content-Length: ")) {
                _size = msg.mid(16, msg.length() - 18).toInt();
               printf("%s", msg.constData());
            } else if (msg == "\r\n") {
                _state = Data;
                printf("start data\n");
                break;
            }
        }
    }
    else if (_state == Data) {
        if (_reply->bytesAvailable() >= _size + 2) {
            QByteArray data = _reply->read(_size);
            QImage img;
            if(!img.loadFromData(data)) {
                printf("invalid data\n");
            } else {
                printf("got image data\n");
            }
            _reply->readLine();
            emit imageChanged(img);
            _state = Header;
        }
    }
}
