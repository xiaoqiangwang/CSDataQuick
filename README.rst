CSDataQuick
===========

This project brings EPICS control system data into QML programming.
The control system data is modeled into CSData object.

Binary Package
--------------

Anaconda
~~~~~~~~

This module can be installed into anaconda environment,::
    
    $ conda install -c paulscherrerinstitute csdataquick

.. note:: Until Anaconda 4.4, on Linux the qt package has 
          `no OpenGL support <https://github.com/ContinuumIO/anaconda-issues/issues/1267>`_.
          One needs to install qt/pyqt package from conda-forge,::

            $ conda install --force -c conda-forge icu qt sip pyqt

.. note:: On Windows, install add also epics binary folder to the path,::

            > set PATH=C:\Miniconda\epics\bin\windows-x64;%PATH%

Install
-------

Get EPICS
~~~~~~~~~
Download and install EPICS base. Set the following environment variables,::

    $ export EPICS_BASE=/opt/epics/base-3.14.12
    $ export EPICS_HOST_ARCH=linux-x86_64

On Windows also add the binary folder to the PATH,::

    > set EPICS_BASE=c:/epics/base-3.14.12
    > set EPICS_HOST_ARCH=windows-x64
    > set PATH=%PATH%;%EPICS_BASE%/bin/%EPICS_HOST_ARCH%

Get Qt5
~~~~~~~
On RHEL/Centos/SL 6.x, install qt5.6 from EPEL repository::

    # yum install yum-conf-epel
    # yum --enablerepo=epel install qt5-qtdeclarative-devel
    # yum --enablerepo=epel install qt5-qtquickcontrols

Or download the `installer <https://www.qt.io/download-open-source/>`_.

With Qt 5.7, the 2D render backend is made available as open source.
This is especially valuable when the OpenGL driver functions poorly, e.g via ssh.
For example, graphics could be rendered ugly on intel graphics driver on SL6.4.
The 2D render could be enabled, by setting the following environment variable::

    $ export QMLSCENE_DEVICE=softwarecontext

.. note:: Qt 5.7 requires C++11, which means gcc 4.8 and above.

Build
~~~~~

Get the source,::

    $ git clone https://github.com/xiaoqiangwang/CSDataQuick.git

Use qmake::

    $ mkdir CSDataQuick-qmake
    $ cd CSDataQuick-qmake
    $ qmake ../CSDataQuick
    $ make

Use cmake::

    $ mkdir CSDataQuick-cmake
    $ cd CSDataQuick-cmake
    $ cmake ../CSDataQuick
    $ make

On Windows in the Qt SDK's prompt::

    > mkdir CSDataQuick-cmake
    > cd CSDataQuick-cmake
    > "C:\Program Files\CMake\bin\cmake" -DCMAKE_BUILD_TYPE=Release -G "NMake Makefiles" ../CSDataQuick
    > nmake

.. note:: On Windows CMake has Qt libraries for its GUI client.
          Don't add it to the PATH.

Demos
-----
First load the example EPICS database into an IOC, e.g.::
    
    $ cd examples/ioc
    $ softIoc startup.script


To have an overview of UI components::

    $ qmlscene -I qml examples/widgets.qml


To explore possible options for each component::

    $ cd examples/gallery
    $ qmlscene -I ../../qml main.qml

To see the ADLViewer in work::

    $ ./bin/ADLViewer examples/controls.adl

Designer
--------
Launch Qt Creator,::

    $ export QML_IMPORT_PATH=`pwd`/qml
    $ qtcreator

Open options dialog, Tools -> Options, select *Qt Quick* from the left list, then select 
*Qt Quick Designer* tab, in the *QML Enumlation Layer* group, select 
*Use QML emulation layer that is built bythe selected Qt*.

Documents
---------

The document is written in qdoc format. The html format can be created by::

    $ cd docs
    $ make html

Or use the generated `online version <https://xiaoqiangwang.github.io/CSDataQuick-doc>`_.
