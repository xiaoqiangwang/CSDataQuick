#ifndef CSDATA_H
#define CSDATA_H

#include <QFlags>
#include <QVariant>

class CSData
{
public:
    CSData();

    enum AccessFlag {
        None = 0x00,
        Read = 0x01,
        Write= 0x02,
    };
    Q_DECLARE_FLAGS(AccessFlags, AccessFlag)

    Q_PROPERTY(QVariant value READ value WRITE setValue NOTIFY valueChanged)
    Q_PROPERTY(QString units READ units WRITE setUnits NOTIFY unitsChanged)
    Q_PROPERTY(bool available READ available WRITE setAvailable NOTIFY availableChanged)
    Q_PROPERTY(QStringList choices READ choices WRITE setChoices NOTIFY choicesChanged)
    Q_PROPERTY(AccessFlags AccessFlag READ access WRITE setAccess NOTIFY accessChanged)

};

Q_DECLARE_OPERATORS_FOR_FLAGS(CSData::AccessFlags);

#endif // CSDATA_H
