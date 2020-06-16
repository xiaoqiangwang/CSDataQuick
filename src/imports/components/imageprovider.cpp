#include "imageprovider.h"

#include <QPainter>
#include <QPainterPath>

DoubleRectProvider::DoubleRectProvider()
    : QQuickImageProvider(QQuickImageProvider::Pixmap)
{
}

QPixmap DoubleRectProvider::requestPixmap(const QString &id, QSize *size, const QSize &requestedSize)
{
    int width = requestedSize.width() > 0 ? requestedSize.width() : 25;
    int height = requestedSize.height() > 0 ? requestedSize.height() : 25;
    width = height = qMin(width, height);

    if (size)
        *size = QSize(width, height);

    QColor color(id); // id used as pen color

    static QRectF rectangle(4./25., 4./25., 13./25., 14./25.);
    static QVector<QPointF> segmentData = QVector<QPointF>()
        << QPointF(17./25.,9./25.)
        << QPointF(22./25.,9./25.)
        << QPointF(22./25.,22./25.)
        << QPointF(10./25.,22./25.)
        << QPointF(10./25.,18./25.);

    QPainterPath path;

    QRectF rc(rectangle.x() * width, rectangle.y() * height,
              rectangle.width() * width, height * rectangle.height());
    path.addRect(rc);

    QVector<QPointF> points;
    foreach (QPointF point, segmentData) {
        points << QPointF(point.x() * width, point.y() * height);
    }
    path.addPolygon(QPolygonF(points));

    QPixmap pixmap(width, height);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    painter.setPen(color);
    painter.drawPath(path);
    return pixmap;
}
