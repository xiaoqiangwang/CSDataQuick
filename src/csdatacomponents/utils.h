#ifndef UTILS_H
#define UTILS_H

#include <QObject>
#include <QVariant>
#include <QUrl>
#include <QPoint>

class QWindow;
class QQuickItem;

class QCSUtils : public QObject
{
    Q_OBJECT
public:
    explicit QCSUtils(QObject *parent = 0);

    Q_INVOKABLE double calculate(QString expr, QVariantList input);
    Q_INVOKABLE bool execute(QString program);
    Q_INVOKABLE QString format(QString format, double number);
    Q_INVOKABLE QString convert(int format, QVariant value, int precision);
    Q_INVOKABLE double parse(int format, QString textValue);
    Q_INVOKABLE QUrl searchADLFile(QString fileName, QWindow *window);
    Q_INVOKABLE QString openADLDisplay(QUrl fileName, QString macro);
    Q_INVOKABLE QString openADLComposite(QUrl fileName, QString macro);
    Q_INVOKABLE QWindow* createDisplay(QString qml, QObject *display, QUrl filePath, QString macro);
    Q_INVOKABLE QWindow* createDisplayByFile(QObject *display, QUrl filePath, QString macro);
    Q_INVOKABLE QVariantMap parseX11Geometry(QString geometry);
    Q_INVOKABLE QString currentDateTime();
    Q_INVOKABLE QWindow *parentWindow(QQuickItem *item);
    Q_INVOKABLE QPoint mapToGlobal(QQuickItem *item, const QPoint point);

signals:

public slots:

};
#endif // UTILS_H
