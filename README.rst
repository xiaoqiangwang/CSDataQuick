epicsqml
========

This project brings EPICS integration to QML programming. By its essense, the PV access has been encapsulated into a Qt Object.


Install
-------
Use qmake::

    $ qmake epicsqml.pro
    $ make

Demos
-----
First load the example EPICS database into an IOC, e.g.::
    
    $ cd examples/ioc
    $ softIoc startup.script


To have an overview of UI components::

    $ qmlscene -I imports examples/widgets.qml


To explore possible options for each component::

    $ cd examples/gallery
    $ qmlscene -I ../../imports main.qml

To see the ADLViewer in work::

    $ ./bin/ADLViewer examples/controls.adl
