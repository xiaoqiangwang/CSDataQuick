/*****************************************************************************\
 * Copyright 2012-2018 Paul Scherrer Institut, Villigen, Switzerland
\*****************************************************************************/

#include "objectmodel.h"

#include <QMetaObject>
#include <QMetaProperty>

#include <QtDebug>

ObjectModel::ObjectModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

QHash<int, QByteArray> ObjectModel::roleNames () const
{
    return _roles;
}

int ObjectModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return _roles.size();
}

int ObjectModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return _objects.length();
}

QVariant ObjectModel::data(const QModelIndex &index, int role) const
{
    QObject *object = _objects.at(index.row());
    if (!object)
        return QVariant();

    const QByteArray roleName = _roles.value(role);
    if (roleName.isEmpty())
        return QVariant();

    if (roleName == "object")
        return QVariant::fromValue<QObject*>(object);
    else
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
    foreach(int signalIndex, _signalsIndex)
        connect(object, object->metaObject()->method(signalIndex),
                this, _handler);
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
