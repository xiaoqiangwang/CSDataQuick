#ifndef IMAGEPROVIDER_H
#define IMAGEPROVIDER_H

#include <QQuickImageProvider>

class DoubleRectProvider : public QQuickImageProvider
{
public:
    DoubleRectProvider();

    QPixmap requestPixmap(const QString &id, QSize *size, const QSize &requestedSize) Q_DECL_OVERRIDE;
};

#endif // IMAGEPROVIDER_H
