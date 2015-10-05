#ifndef CSDATASOURCE_H
#define CSDATASOURCE_H

#include <QObject>
#include <QQmlPropertyMap>

class CSDataEngine;

class CSDataSource : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString source READ source WRITE setSource NOTIFY sourceChanged)
    Q_PROPERTY(QQmlPropertyMap *data READ data NOTIFY dataChanged)
public:
    explicit CSDataSource(QObject *parent = 0);

    QString source() const;
    void setSource(const QString source);

     QQmlPropertyMap *data() const;

signals:
    void sourceChanged();
    void dataChanged();

public slots:

private:
    QString _source;
    QQmlPropertyMap  *_data;
    QList<CSDataEngine*> _interfaces;
};

#endif // CSDATASOURCE_H
