#include "adimageprovider.h"
#include <QtDebug>

ADImageProvider::ADImageProvider()
    : QQuickImageProvider(QQuickImageProvider::Image),
      blankimage(100, 100, QImage::Format_ARGB32_Premultiplied)
{
    blankimage.fill(Qt::white);
}

ADImageProvider::~ADImageProvider()
{
    QMapIterator<QString, QMap<QString, PvObject*> > detector(detectors);
    while (detector.hasNext()) {
        detector.next();
        QMap<QString, PvObject*> pvs = detector.value();
        foreach(PvObject *pv, pvs) {
            delete pv;
        }
    }
}

QImage ADImageProvider::requestImage(const QString &id, QSize *size, const QSize &requestedSize)
{
    Q_UNUSED(requestedSize);

    QString prefix = id;
    connectChannels(prefix);
    if (!detectors[prefix]["data"]->connected())
        return blankimage;

    // return cached image if areaDetector source not updating
    int uid = detectors[prefix]["uid"]->value().toInt();
    if (uid == uids[prefix])
        return images[prefix];

    // image info
    int width = 0, height = 0, depth = 0, bytesize = 0;
    QImage::Format format;

    // only support int8/uint8 data type image
    if (detectors[prefix]["dtype"]->value().toInt() >= 2) {
        return blankimage;
    }

    switch (detectors[prefix]["color"]->value().toInt()) {
    case 0: // Mono
    case 1: // Bayer
        depth  = 1;
        width  = detectors[prefix]["size0"]->value().toInt();
        height = detectors[prefix]["size1"]->value().toInt();
        format = QImage::Format_Indexed8;
        break;
    case 2: // RGB1
        depth  = 3;
        width  = detectors[prefix]["size1"]->value().toInt();
        height = detectors[prefix]["size2"]->value().toInt();
        format = QImage::Format_RGB888;
        break;
    default: // we don't support them, yet.
        return blankimage;
    }

    bytesize = width * height * depth;

    QImage image((uchar *)detectors[prefix]["data"]->getArrayValue(bytesize), width, height, format);

    if (size)
        *size = QSize(width, height);

    // cache the image
    uids[prefix] = uid;
    images[prefix] = image;

    return image;
}

void ADImageProvider::connectChannels(const QString &prefix)
{
    if (detectors.contains(prefix))
        return;

    uids[prefix] = 0;
    images[prefix] = blankimage;

    PvObject * pv;

    pv = new PvObject();
    pv->setMonitor(false); // disable monitor to avoid excessive traffic
    pv->connect(prefix.toLatin1() + "ArrayData");
    detectors[prefix]["data"] = pv;

    pv = new PvObject();
    pv->connect(prefix.toLatin1() + "NDimensions_RBV");
    detectors[prefix]["ndims"] = pv;

    pv = new PvObject();
    pv->connect(prefix.toLatin1() + "ArraySize0_RBV");
    detectors[prefix]["size0"] = pv;

    pv = new PvObject();
    pv->connect(prefix.toLatin1() + "ArraySize1_RBV");
    detectors[prefix]["size1"] = pv;

    pv = new PvObject();
    pv->connect(prefix.toLatin1() + "ArraySize2_RBV");
    detectors[prefix]["size2"] = pv;

    pv = new PvObject();
    pv->connect(prefix.toLatin1() + "DataType_RBV");
    detectors[prefix]["dtype"] = pv;

    pv = new PvObject();
    pv->connect(prefix.toLatin1() + "ColorMode_RBV");
    detectors[prefix]["color"] = pv;

    pv = new PvObject();
    pv->connect(prefix.toLatin1() + "UniqueId_RBV");
    detectors[prefix]["uid"] = pv;
}
