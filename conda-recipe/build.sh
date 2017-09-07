mkdir build
cd build
cmake ..
make
cp -r bin $PREFIX
cp -r lib $PREFIX
cp -r plugins $PREFIX
cp -r qml $PREFIX
