#ifndef CSDATACOMPONENTS_PLUGIN_H
#define CSDATACOMPONENTS_PLUGIN_H

#include <QQmlExtensionPlugin>

class CSDataComponentsPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtensionInterface/1.0")

public:
    void registerTypes(const char *uri);
    void initializeEngine(QQmlEngine *engine, const char *uri);
};

#endif // CSDATACOMPONENTS_PLUGIN_H
