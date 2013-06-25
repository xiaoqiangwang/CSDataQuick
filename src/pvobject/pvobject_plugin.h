#ifndef PVOBJECT_PLUGIN_H
#define PVOBJECT_PLUGIN_H

#include <QQmlExtensionPlugin>

class PvObjectPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtensionInterface/1.0")

public:
    void registerTypes(const char *uri);
};

#endif // PVOBJECT_PLUGIN_H
