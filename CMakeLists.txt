project(epicsqml)
cmake_minimum_required(VERSION 2.8.6 FATAL_ERROR)
set(EPICSQML_VERSION_MAJOR 1)
set(EPICSQML_VERSION_MINOR 0)
set(EPICSQML_VERSION_RELEASE 0)
set(EPICSQML_VERSION "${QTERM_VERSION_MAJOR}.${QTERM_VERSION_MINOR}.${QTERM_VERSION_RELEASE}" )

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

find_package(Qt5Core)
find_package(Qt5Gui)
find_package(Qt5Widgets)
find_package(Qt5Qml)
find_package(Qt5Quick)
find_package(Qt5PrintSupport)

find_package(EPICS)

set(ENV{QML_IMPORT_PATH} )

add_subdirectory(src)
add_subdirectory(docs)
add_subdirectory(examples)
add_subdirectory(tests)
