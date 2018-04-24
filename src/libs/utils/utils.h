#ifndef UTILS_H
#define UTILS_H

#include <QObject>
#include <QVariant>
#include <QUrl>
#include <QPoint>
#include <QJSValue>

#include "utils_global.h"

class QWindow;
class QQuickItem;
class QCSData;

class UTILS_EXPORT QCSUtils : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool inPuppet MEMBER _inPuppet CONSTANT)
public:
    explicit QCSUtils(QObject *parent = 0);

    Q_INVOKABLE static double calculate(QString expr, QVariantList input);
    Q_INVOKABLE static bool execute(QString program);
    Q_INVOKABLE static QString format(QString format, double number);
    Q_INVOKABLE static QString convert(int format, QVariant value, int precision);
    Q_INVOKABLE static double parse(int format, QString textValue);
    Q_INVOKABLE static QUrl searchDisplayFile(QString fileName, QWindow *window);
    Q_INVOKABLE static QString openQMLDisplay(QUrl fileName, QString macro);
    Q_INVOKABLE static QString openADLDisplay(QUrl fileName, QString macro);
    Q_INVOKABLE static QString openADLComposite(QUrl fileName, QString macro);
    Q_INVOKABLE static QWindow* createDisplay(QString qml, QObject *display, QUrl filePath, QString macro);
    Q_INVOKABLE static QWindow* createDisplayByFile(QObject *display, QUrl filePath, QString macro);
    Q_INVOKABLE static QVariantMap parseX11Geometry(QString geometry);
    Q_INVOKABLE static QString currentDateTime();
    Q_INVOKABLE static QWindow *parentWindow(QQuickItem *item);
    Q_INVOKABLE static QPoint mapToGlobal(QQuickItem *item, const QPoint point);
    Q_INVOKABLE static void vectorGet(QVariant variant, QJSValue array, int index=0, int count=-1);
    Q_INVOKABLE static QVariant vectorSet(QVariant variant, QJSValue value, int index);
    Q_INVOKABLE static QVariantList parseExecList();
    Q_INVOKABLE static QVariant getProperty(QObject *object, QString name);
    Q_INVOKABLE static void copyToClipboard(const QString &text);
    Q_INVOKABLE static int qtVersion();
    Q_INVOKABLE static QString formatString(QCSData* data, int format, int precision, QVariant value=QVariant());
signals:

public slots:

private:
    bool _inPuppet;
};
#endif // UTILS_H
