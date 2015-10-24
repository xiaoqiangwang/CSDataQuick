#include "objectmodel.h"

#include <QMetaObject>
#include <QMetaProperty>

#include <QtDebug>

ObjectModel::ObjectModel(const QByteArrayList roles, QObject *parent)
    : QAbstractListModel(parent)
{
    setObjectName("QObjectListModel");
    for(int i=0; i<roles.count(); i++) {
        _roles.insert(Qt::UserRole + i, roles.at(i));
    }
    _handler = metaObject ()->method(metaObject()->indexOfMethod ("onObjectPropertyChanged()"));
}

QHash<int, QByteArray> ObjectModel::roleNames () const
{
    return _roles;
}

int ObjectModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return _objects.length();
}

QVariant ObjectModel::data(const QModelIndex &index, int role) const
{
    const QObject *object = _objects.at(index.row());
    if (!object)
        return QVariant();

    const QByteArray roleName = _roles.value(role);
    if (roleName.isEmpty())
        return QVariant();

    return object->property(roleName);
}

void ObjectModel::append(QObject *object)
{
    if (!object)
        return;

    int pos = _objects.count();
    beginInsertRows(QModelIndex(), pos, pos);
    _objects.append(object);
    // add property change signal handler
    const QMetaObject* meta = object->metaObject();
    QHashIterator<int, QByteArray> it(_roles);
    while (it.hasNext()) {
        it.next();
        int index = meta->indexOfProperty(it.value());
        if (index == -1)
            continue;
        QMetaProperty property = meta->property(index);
        if (property.hasNotifySignal()) {
            QMetaMethod method = property.notifySignal();
            connect(object, method, this, _handler);
        }
    }

    endInsertRows();
    emit countChanged();
}

void ObjectModel::remove(QObject *object)
{
    if(!object)
        return;

    int index = _objects.indexOf(object);
    if (index == -1)
        return;

    beginRemoveRows(QModelIndex(), index, index);
    _objects.removeOne(object);
    // remove property change signal handler
    disconnect(object, Q_NULLPTR, this, Q_NULLPTR);
    endRemoveRows();
    emit countChanged();
}

void ObjectModel::onObjectPropertyChanged()
{
    QObject *object = sender();
    QModelIndex index = QAbstractListModel::index(_objects.indexOf(object));
    emit dataChanged(index, index);
}
