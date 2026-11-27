cmake_minimum_required(VERSION 3.16)

project(CSDataQuick)

set(CSDATAQUICK_VERSION_MAJOR 1)
set(CSDATAQUICK_VERSION_MINOR 2)
set(CSDATAQUICK_VERSION_RELEASE "1")
set(CSDATAQUICK_VERSION "${CSDATAQUICK_VERSION_MAJOR}.${CSDATAQUICK_VERSION_MINOR}.${CSDATAQUICK_VERSION_RELEASE}" )

# Include custom cmake modules
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 6.8
    COMPONENTS Core Gui Widgets Qml Quick QuickControls2 Network PrintSupport Test
    OPTIONAL_COMPONENTS Concurrent Help ToolsTools QmlWorkerScript WebSockets
    QUIET)

find_package(EPICS QUIET)

set(QML_IMPORT_PATH ${CMAKE_BINARY_DIR}/qml CACHE STRING "QML Import Paths")

include(BuildHelpers)
include(InstallDirs)
include(CTest)

add_subdirectory(src)
add_subdirectory(docs)
add_subdirectory(examples)
add_subdirectory(tools)
add_subdirectory(tests)

include(Packaging)
