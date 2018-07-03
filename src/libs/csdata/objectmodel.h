#ifndef OBJECTMODEL_H
#define OBJECTMODEL_H

#include <QAbstractItemModel>

#include <QMetaMethod>
#include <QVector>

#include "csdata_global.h"

class CSDATA_EXPORT ObjectModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ count NOTIFY countChanged)

public:
    ObjectModel(QObject *parent);

    template <typename ObjectType>
    void setup(const QByteArrayList roles) {
        _roles.insert(Qt::UserRole, "object");

        for(int i=0; i<roles.count(); i++) {
            QByteArray role = roles.at(i);

            _roles.insert(Qt::UserRole + 1 + i, role);
            // find the corresponding role's property and its signal
            const QMetaObject meta = ObjectType::staticMetaObject;
            int index = meta.indexOfProperty(role);
            if (index >= 0) {
                QMetaProperty property = meta.property(index);
                if (property.hasNotifySignal())
                    _signalsIndex.append(property.notifySignalIndex());
            }
        }
        _handler = metaObject ()->method(metaObject()->indexOfMethod ("onObjectPropertyChanged()"));
    }

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
    QVector<QObject*> _objects;
    QHash<int, QByteArray> _roles;
    QVector<int> _signalsIndex;
    QMetaMethod _handler;
};

#endif // OBJECTMODEL_H
