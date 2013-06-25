#include "adimageprovider.h"
#include <QtDebug>

ADImageProvider::ADImageProvider()
    : QQuickImageProvider(QQuickImageProvider::Image)
{
    first = true;
}

QImage ADImageProvider::requestImage(const QString &id, QSize *size, const QSize &requestedSize)
{
    QString prefix = id;

    if (first) {
        // connect channels for the first time
        pvData.connect(prefix.toLatin1() + "ArrayData");
        pvDims.connect(prefix.toLatin1() + "NDimensions_RBV");
        pvSize0.connect(prefix.toLatin1() + "ArraySize0_RBV");
        pvSize1.connect(prefix.toLatin1() + "ArraySize0_RBV");
        pvSize2.connect(prefix.toLatin1() + "ArraySize0_RBV");
        pvColor.connect(prefix.toLatin1() + "ColorMode_RBV");
        pvUniqueId.connect(prefix.toLatin1() + "UniqueId_RBV");
        first = false;
    }

    if (!pvData.connected())
        return QImage(requestedSize, QImage::Format_Indexed8);

    static int prev_uid = 0;
    int uid = pvUniqueId.value().toInt();

    if (uid != prev_uid) {
        int width = 0, height = 0, depth = 0, size = 0;
        QImage::Format format;
        if (pvDims.value() == 2) {
            depth  = 1;
            width  = pvSize0.value().toInt();
            height = pvSize1.value().toInt();
            format = QImage::Format_Indexed8;
        } else {
            depth  = 3;
            width  = pvSize1.value().toInt();
            height = pvSize2.value().toInt();
            format = QImage::Format_RGB888;
        }
        size = width * height * depth;

        QImage image((uchar *)pvData.arrayValue(), width, height, format);
        return image;
    }
}
