/*****************************************************************************\
 * Copyright 2012-2018 Paul Scherrer Institut, Villigen, Switzerland
\*****************************************************************************/

#include "mjpeg.h"

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QRegularExpression>
#include <QRegularExpressionMatch>

MJPEG::MJPEG(QObject *parent) : QObject(parent), _reply(0)
{
    _network = new QNetworkAccessManager(this);
}

MJPEG::~MJPEG()
{
    delete _reply;
    delete _network;
}

void MJPEG::setSource(QString source)
{
    if (qgetenv("QML_PUPPET_MODE") == "true") {
        _source = source;
        emit sourceChanged();
        emit imageChanged(QImage(":/images/blank_video.jpg"));
        return;
    }

    if (source != _source) {
        if (_reply) {
            disconnect(_reply);
            _reply->abort();
            _reply->deleteLater();
            _reply = Q_NULLPTR;
        }
        _source = source;
        if (!source.isEmpty()) {
            _boundary = "";
            _state = Neutral;
            _reply = _network->get(QNetworkRequest(QUrl(source)));
            connect(_reply, SIGNAL(metaDataChanged()), this, SLOT(readHeader()));
            connect(_reply, SIGNAL(readyRead()), this, SLOT(readStream()));
            connect(_reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(error(QNetworkReply::NetworkError)));
        }
        emit sourceChanged();
    }
}

void MJPEG::error(QNetworkReply::NetworkError code)
{
    qCritical() << "Connection error" << code;
}

void MJPEG::readHeader()
{
    QVariant contentType = _reply->header(QNetworkRequest::ContentTypeHeader);
    if (!contentType.isValid())
        return;

    QRegularExpression re("multipart/x-mixed-replace; *boundary=(.*)");
    QRegularExpressionMatch m = re.match(contentType.toString());
    _boundary = m.captured(1).toUtf8();

    if (_boundary.isEmpty()) {
        _reply->abort();
        qCritical() << "No \"boundary\" specified in HTTP header" << _source;
    }
}

void MJPEG::readStream()
{
    while (_reply->canReadLine()) {
        QByteArray msg =_reply->readLine();
        switch (_state) {
        case Neutral:
            if (msg.startsWith(_boundary)) {
                _state = Header;
            }
            else {
                _reply->abort();
                qCritical() << "Boundary" << _boundary << "not found in MJPEG stream" << _source;
            }
            break;
        case Header:
            if (msg == "\r\n") {
                _data.clear();
                _state = Data;
            }
            break;
        case Data:
            if (msg.startsWith(_boundary)) {
                _state = Header;
                emit imageChanged(QImage::fromData(_data));
            }
            else {
                _data.append(msg.constData(), msg.length());
            }
        default:
            break;
        }
    }
}
