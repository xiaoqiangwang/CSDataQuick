#ifndef ADIMAGEPROVIDER_PLUGIN_H
#define ADIMAGEPROVIDER_PLUGIN_H

#include <QQmlEngine>
#include <QQmlExtensionPlugin>

class ADImageProviderPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtensionInterface/1.0")
public:
    void registerTypes(const char *uri);
    void initializeEngine(QQmlEngine *engine, const char *uri);
};

#endif // ADIMAGEPROVIDER_PLUGIN_H
