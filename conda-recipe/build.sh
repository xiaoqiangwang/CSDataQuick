mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX=$PREFIX \
      -DKDE_INSTALL_BINDIR=bin \
      -DKDE_INSTALL_LIBDIR=lib \
      -DKDE_INSTALL_PLUGINDIR=plugins \
      -DKDE_INSTALL_QMLDIR=qml \
      ..
make install
