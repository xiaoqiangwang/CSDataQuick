#include "mjpeg.h"

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

MJPEG::MJPEG(QObject *parent) : QObject(parent), _reply(0)
{
    _network = new QNetworkAccessManager(this);
}

MJPEG::~MJPEG()
{
    qDebug() << "MJPEG deleted";
}

void MJPEG::setSource(QString source)
{
    if (source.isEmpty()) {
        if (_reply) {
            disconnect(_reply, SIGNAL(readyRead()), this, SLOT(readStream()));
            _reply->abort();
        }
        _source = source;
        return;
    }

    if (source != _source) {
        if (_reply) {
            disconnect(_reply, SIGNAL(readyRead()), this, SLOT(readStream()));
            _reply->abort();
        }
        _source = source;
        _size = 0;
        _state = Header;
        _reply = _network->get(QNetworkRequest(QUrl(source)));
        connect(_reply, SIGNAL(readyRead()), this, SLOT(readStream()));
    }
}

void MJPEG::readStream()
{
    if (_state == Header) {
        while(_reply->canReadLine()) {
            QByteArray msg =_reply->readLine();
            if (msg.isEmpty()) {
                break;
            } else if (msg.startsWith("Content-Length: ")) {
                _size = msg.mid(16, msg.length() - 18).toInt();
            } else if (msg == "\r\n" && _size != 0) {
                _state = Data;
                break;
            }
        }
    } else if (_state == Data) {
        if (_reply->bytesAvailable() >= _size + 2) {
            QByteArray data = _reply->read(_size);

            QByteArray msg = _reply->read(2);
            if (msg != "\r\n") {
                qWarning() << "Unexpected end of MJPEG data";
            }

            QImage img = QImage::fromData(data);
            emit imageChanged(img);

            _size = 0;
            _state = Header;
        }
    }
}
