#ifndef CSDATAENGINEMANAGER
#define CSDATAENGINEMANAGER

#include <QObject>

#define engineManager CSDataEngineManager::instance()

class CSDataEngine;

class CSDataEngineManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QList<CSDataEngine*> engines MEMBER _engines CONSTANT)

public:
    static CSDataEngineManager *instance();
    ~CSDataEngineManager();
    CSDataEngine *engineForName(QString source);
    CSDataEngine *defaultEngine();

private:
    CSDataEngineManager(QObject *parent=0);

    static CSDataEngineManager *_manager;

    QList<CSDataEngine*> _engines;
};

#endif // CSDATAENGINEMANAGER

