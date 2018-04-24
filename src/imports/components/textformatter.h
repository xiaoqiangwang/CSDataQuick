#ifndef TEXTFORMATTER_H
#define TEXTFORMATTER_H

#include <QObject>
class QCSData;

class TextFormatter : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QCSData* data MEMBER _data      WRITE setData)
    Q_PROPERTY(int format    MEMBER _format    WRITE setFormat)
    Q_PROPERTY(int precision MEMBER _precision WRITE setPrecision)
    Q_PROPERTY(QString text  MEMBER _text      WRITE setText      NOTIFY textChanged)
public:
    explicit TextFormatter(QObject *parent = 0);

    void setData(QCSData *);
    void setFormat(int);
    void setPrecision(int);
    void setText(QString);

public slots:
    void valueChanged();
    void sourceChanged();
    void stateStringsChanged();

signals:
    void textChanged();

private:
    QCSData *_data;
    int _format;
    int _precision;
    QString _text;
};

#endif // MJPEG_H
