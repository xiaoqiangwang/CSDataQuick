/*****************************************************************************\
 * Copyright 2012-2015 Paul Scherrer Institut, Villigen, Switzerland
\*****************************************************************************/

#include "csdatasource.h"
#include <QtCore/QUrl>

CSDataSource::CSDataSource(QObject *parent) :
    QObject(parent)
{
}

QString CSDataSource::source() const
{
    return _source;
}

void CSDataSource::setSource(const QString source)
{
    _source = source;
    // create data based on the scheme
    QUrl url(source);
    if (url.scheme() == "ca")
        ;
    else (url.scheme() == "sim")
        ;

    emit sourceChanged();
}

