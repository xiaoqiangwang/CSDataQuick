CSDataQuick
===========

This project brings EPICS control system data into QML programming.


Install
-------

Get Qt5
~~~~~~~
On RHEL/Centos/SL 6.x, install qt5.6 from EPEL repository::

    # yum install yum-conf-epel
    # yum --enablerepo=epel install qt5-qtdeclarative-devel
    # yum --enablerepo=epel install qt5-qtquickcontrols

Or download the `installer https://www.qt.io/download-open-source/`_.

With Qt 5.7, the 2D render backend is made available as open source.
This is especially valuable when the OpenGL driver function poorly.
For example, graphics could be rendered ugly on intel graphics driver on SL6.4.
The 2D render could be enabled, by setting the following environment variable::

    $ export QMLSCENE_DEVICE=softwarecontext


Build
~~~~~
Use qmake::

    $ qmake CSDataQuick.pro
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

Designer
--------
Be sure to change the render from 2D to OpenGLi,::

    $ export QMLSCENE_DEVICE=

Open options dialog, Tools -> Options, select *Qt Quick* from the left list, then select 
*Qt Quick Designer* tab, in the *QML Enumlation Layer* group, select 
*Use QML emulation layer that is built bythe selected Qt*.



