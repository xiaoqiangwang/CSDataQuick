#ifndef ADIMAGEPROVIDER_H
#define ADIMAGEPROVIDER_H

#include <QQuickImageProvider>
#include <pvobject.h>

class ADImageProvider : public QQuickImageProvider
{
public:
    ADImageProvider();

    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize);

private:
    PvObject pvData;
    PvObject pvDims;
    PvObject pvSize0;
    PvObject pvSize1;
    PvObject pvSize2;
    PvObject pvColor;
    PvObject pvUniqueId;

    bool first;
};

#endif // ADIMAGEPROVIDER_H
