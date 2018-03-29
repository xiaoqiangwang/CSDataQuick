#ifndef UTILS_H
#define UTILS_H

#include <QObject>
#include <QVariant>
#include <QUrl>
#include <QPoint>
#include <QJSValue>

class QWindow;
class QQuickItem;
class QCSData;

class QCSUtils : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool inPuppet MEMBER _inPuppet CONSTANT)
public:
    explicit QCSUtils(QObject *parent = 0);

    Q_INVOKABLE double calculate(QString expr, QVariantList input);
    Q_INVOKABLE bool execute(QString program);
    Q_INVOKABLE QString format(QString format, double number);
    Q_INVOKABLE QString convert(int format, QVariant value, int precision);
    Q_INVOKABLE double parse(int format, QString textValue);
    Q_INVOKABLE QUrl searchDisplayFile(QString fileName, QWindow *window);
    Q_INVOKABLE QString openQMLDisplay(QUrl fileName, QString macro);
    Q_INVOKABLE QString openADLDisplay(QUrl fileName, QString macro);
    Q_INVOKABLE QString openADLComposite(QUrl fileName, QString macro);
    Q_INVOKABLE QWindow* createDisplay(QString qml, QObject *display, QUrl filePath, QString macro);
    Q_INVOKABLE QWindow* createDisplayByFile(QObject *display, QUrl filePath, QString macro);
    Q_INVOKABLE QVariantMap parseX11Geometry(QString geometry);
    Q_INVOKABLE QString currentDateTime();
    Q_INVOKABLE QWindow *parentWindow(QQuickItem *item);
    Q_INVOKABLE QPoint mapToGlobal(QQuickItem *item, const QPoint point);
    Q_INVOKABLE void vectorGet(QVariant variant, QJSValue array, int index=0, int count=-1);
    Q_INVOKABLE QVariant vectorSet(QVariant variant, QJSValue value, int index);
    Q_INVOKABLE QVariantList parseExecList();
    Q_INVOKABLE QVariant getProperty(QObject *object, QString name);
    Q_INVOKABLE void copyToClipboard(const QString &text);
    Q_INVOKABLE int qtVersion();
    Q_INVOKABLE QString formatString(QCSData* data, int format, int precision);
signals:

public slots:

private:
    bool _inPuppet;
};
#endif // UTILS_H
