#ifndef CSDATASOURCE_H
#define CSDATASOURCE_H

#include <QObject>
#include <QQmlPropertyMap>

class CSDataSource : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString source READ source WRITE setSource NOTIFY sourceChanged)
    Q_PROPERTY(QQmlPropertyMap data READ data WRITE setData NOTIFY dataChanged)

public:
    explicit CSDataSource(QObject *parent = 0);

    QString source() const;
    void setSource(const QString source);

signals:

public slots:

private:
    QString _source;
};

#endif // CSDATASOURCE_H
