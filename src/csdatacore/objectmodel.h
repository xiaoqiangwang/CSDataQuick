#ifndef OBJECTMODEL_H
#define OBJECTMODEL_H

#include <QAbstractItemModel>

#include <QMetaMethod>

class ObjectModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ count NOTIFY countChanged)

public:
    ObjectModel(const QByteArrayList roles, QObject *parent);

    virtual int rowCount(const QModelIndex & parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual QHash<int, QByteArray> roleNames() const;

    void append(QObject *object);
    void remove(QObject *object);

    int size() const { return _objects.size(); }
    int count() const { return _objects.count(); }
    bool contains(QObject *object) const { return _objects.contains(object);}
    QObject *at(int i) const { return _objects.at(i); }

signals:
    void countChanged();

public slots:
    void onObjectPropertyChanged();

private:
    QList<QObject*> _objects;
    QHash<int, QByteArray> _roles;
    QMetaMethod _handler;
};

#endif // OBJECTMODEL_H
