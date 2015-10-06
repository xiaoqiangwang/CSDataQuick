#ifndef CSDATAENGINEMANAGER
#define CSDATAENGINEMANAGER

#include <QObject>

#define engineManager QCSDataEngineManager::instance()

class QCSDataEngine;

class QCSDataEngineManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QList<QCSDataEngine*> engines MEMBER _engines CONSTANT)

public:
    static QCSDataEngineManager *instance();
    ~QCSDataEngineManager();
    QCSDataEngine *engineForName(QString source);
    QCSDataEngine *defaultEngine();

private:
    QCSDataEngineManager(QObject *parent=0);

    static QCSDataEngineManager *_manager;

    QList<QCSDataEngine*> _engines;
};

#endif // CSDATAENGINEMANAGER

