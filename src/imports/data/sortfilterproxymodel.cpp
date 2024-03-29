/****************************************************************************
**
** Copyright (C) 2014 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
**     of its contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "sortfilterproxymodel.h"
#include <QtDebug>
#include <QtQml>

SortFilterProxyModel::SortFilterProxyModel(QObject *parent) : QSortFilterProxyModel(parent)
{
    connect(this, SIGNAL(rowsInserted(QModelIndex,int,int)), this, SIGNAL(countChanged()));
    connect(this, SIGNAL(rowsRemoved(QModelIndex,int,int)), this, SIGNAL(countChanged()));
}

int SortFilterProxyModel::count() const
{
    return rowCount();
}

QObject *SortFilterProxyModel::source() const
{
    return sourceModel();
}

void SortFilterProxyModel::setSource(QObject *source)
{
    setSourceModel(qobject_cast<QAbstractItemModel *>(source));
    // re-assure the sort/filter role
    QSortFilterProxyModel::setSortRole(roleKey(_sortRole));
    QSortFilterProxyModel::setFilterRole(roleKey(_filterRole));
}

QByteArray SortFilterProxyModel::sortRole() const
{
    return _sortRole;
}

void SortFilterProxyModel::setSortRole(const QByteArray &role)
{
    _sortRole = role;
    QSortFilterProxyModel::setSortRole(roleKey(role));
}

void SortFilterProxyModel::setSortOrder(Qt::SortOrder order)
{
    QSortFilterProxyModel::sort(0, order);
}

QByteArray SortFilterProxyModel::filterRole() const
{
    return _filterRole;
}

void SortFilterProxyModel::setFilterRole(const QByteArray &role)
{
    _filterRole = role;
    QSortFilterProxyModel::setFilterRole(roleKey(role));
}

QString SortFilterProxyModel::filterString() const
{
    return _filter;
}

void SortFilterProxyModel::setFilterString(const QString &filter)
{
    if (filter == _filter)
        return;
    _filter = filter;

    updateFilter();
}

SortFilterProxyModel::FilterSyntax SortFilterProxyModel::filterSyntax() const
{
    return _syntax;
}

void SortFilterProxyModel::setFilterSyntax(SortFilterProxyModel::FilterSyntax syntax)
{
    if (syntax == _syntax)
        return;
    _syntax = syntax;

    updateFilter();
}

QJSValue SortFilterProxyModel::get(int idx) const
{
    QJSEngine *engine = qmlEngine(this);
    QJSValue value = engine->newObject();
    if (idx >= 0 && idx < count()) {
        QHash<int, QByteArray> roles = roleNames();
        QHashIterator<int, QByteArray> it(roles);
        while (it.hasNext()) {
            it.next();
            value.setProperty(QString::fromUtf8(it.value()), data(index(idx, 0), it.key()).toString());
        }
    }
    return value;
}

int SortFilterProxyModel::roleKey(const QByteArray &role) const
{
    return roleNames().key(role, -1);
}

QHash<int, QByteArray> SortFilterProxyModel::roleNames() const
{
    if (QAbstractItemModel *source = sourceModel())
        return source->roleNames();
    return QHash<int, QByteArray>();
}

bool SortFilterProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    if (_filter.isEmpty())
        return true;

#if (QT_VERSION >= QT_VERSION_CHECK(5,15,0))
    QRegularExpression regexp = filterRegularExpression();
#else
    QRegExp regexp = filterRegExp();
#endif
    if (!regexp.isValid())
        return true;

    QModelIndex sourceIndex = sourceModel()->index(sourceRow, 0, sourceParent);
    if (!sourceIndex.isValid())
        return true;
    QString key = sourceModel()->data(sourceIndex, QSortFilterProxyModel::filterRole()).toString();
    return key.contains(regexp);
}

void SortFilterProxyModel::updateFilter()
{
#if (QT_VERSION >= QT_VERSION_CHECK(5,15,0))
    QString filter = _filter;
    if (_syntax == Wildcard)
        filter = QRegularExpression::wildcardToRegularExpression(_filter);
    else if (_syntax == FixedString)
        filter = QRegularExpression::escape(_filter);

    QRegularExpression::PatternOptions options = QRegularExpression::NoPatternOption;
    if (filterCaseSensitivity() == Qt::CaseInsensitive)
        options |= QRegularExpression::CaseInsensitiveOption;

    QRegularExpression regexp(filter, options);
    setFilterRegularExpression(regexp);
#else
    setFilterRegExp(QRegExp(filterString(), filterCaseSensitivity(), static_cast<QRegExp::PatternSyntax>(_syntax)));
#endif
}
