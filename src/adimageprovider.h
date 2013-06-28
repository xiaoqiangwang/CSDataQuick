#ifndef ADIMAGEPROVIDER_H
#define ADIMAGEPROVIDER_H

#include <QQuickImageProvider>
#include <pvobject.h>

class ADImageProvider : public QQuickImageProvider
{
public:
    ADImageProvider();
    ~ADImageProvider();

    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize);

    void connectChannels(const QString &id);
private:
    QMap<QString, QMap<QString, PvObject *> > detectors;
    QMap<QString, QImage> images;
    QMap<QString, int> uids;

    QImage blankimage;
};

#endif // ADIMAGEPROVIDER_H
