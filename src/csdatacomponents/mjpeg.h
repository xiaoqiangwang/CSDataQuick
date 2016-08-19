#ifndef MJPEG_H
#define MJPEG_H

#include <QObject>
#include <QImage>
class QNetworkAccessManager;
class QNetworkReply;
class QNetworkRequest;

class MJPEG : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString source READ source WRITE setSource NOTIFY sourceChanged)

public:
    explicit MJPEG(QObject *parent = 0);
    ~MJPEG();

    enum MJPEGState {
        Header = 0,
        Data
    };

    QString source() { return _source; }
    void setSource(QString source);

public slots:
    void readStream();

signals:
    void sourceChanged();
    void imageChanged(QImage image);

private:
    QString _source;
    MJPEGState _state;
    qint64  _size;

    QNetworkAccessManager *_network;
    QNetworkReply *_reply;
};

#endif // MJPEG_H
