/*****************************************************************************\
 * Copyright 2012-2018 Paul Scherrer Institut, Villigen, Switzerland
\*****************************************************************************/

#include "csdataengine.h"

QCSDataEngine::QCSDataEngine(QObject *parent)
    : QObject(parent)
{

}

/*!
    \class QCSDataEngine
    \inmodule CSDataQuick.Data
    \brief The QCSDataEngine class defines the interface of data engine.

    This is the base class for data engines, which define the interface to create, delete and interact with QCSData objects. 

    Data engines are loaded as plugins on demand and provide zero, one or more data sources which are identified by name.
*/

/*!
    \property QCSDataEngine::name
    Name of the data engine as used in the data source scheme.

    \sa QCSData::source
*/

/*!
    \property QCSDataEngine::description
    Descriptive text of this data engine.
*/

/*!
    \property QCSDataEngine::allData
    ObjectModel that contains all QCSData objects created by this data engine.
*/

/*!
    \fn void QCSDataEngine::create(QCSData* data)
    Connects the source to the \a data object.

    The data engine must implements this to check whether the data source is valid,
    and if so add it to \l allData. The data engine possibly retrieves the information
    from the source and populates \a data properties.
*/

/*!
    \fn void QCSDataEngine::close(QCSData* data)
    Closes the \a data object's connection to the source.

    The data engine must implements this to release the connection to the source and remove it from \l allData.
*/

/*!
    \fn void QCSDataEngine::setValue(QCSData* data, const QVariant value)
    Sets a new \a value for the \a data's source.

    The data engine must implements this to accept requests to change the \a data source's value.
*/
