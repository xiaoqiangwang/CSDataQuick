#include "pvobject.h"
#include <QtDebug>

int main(int argc, char **argv)
{
    PvObject pv;
    pv.setChannel("ARIDI-PCT:CURRENT");
    qDebug() << pv.value();
}
