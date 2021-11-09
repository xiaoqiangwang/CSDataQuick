cmake_minimum_required(VERSION 2.8.12 FATAL_ERROR)

project(CSDataQuick)

set(CSDATAQUICK_VERSION_MAJOR 1)
set(CSDATAQUICK_VERSION_MINOR 2)
set(CSDATAQUICK_VERSION_RELEASE "1")
set(CSDATAQUICK_VERSION "${CSDATAQUICK_VERSION_MAJOR}.${CSDATAQUICK_VERSION_MINOR}.${CSDATAQUICK_VERSION_RELEASE}" )

# Include custom cmake modules
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
if (NOT CMAKE_VERSION VERSION_LESS 3.0)
set(CMAKE_AUTORCC ON)
endif()

cmake_policy(SET CMP0020 NEW)
if (NOT CMAKE_VERSION VERSION_LESS 3.3)
cmake_policy(SET CMP0057 NEW)
endif()
if (NOT CMAKE_VERSION VERSION_LESS 3.10)
cmake_policy(SET CMP0071 NEW)
endif()
if (NOT CMAKE_VERSION VERSION_LESS 3.13)
cmake_policy(SET CMP0079 NEW)
endif()

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core REQUIRED)
find_package(Qt${QT_VERSION_MAJOR}
    COMPONENTS Gui Widgets Qml Quick QuickControls2 Network PrintSupport Test
    OPTIONAL_COMPONENTS Concurrent QmlWorkerScript
    QUIET)

# Use c++11 or c++0x
if (CMAKE_VERSION VERSION_LESS 3.1 AND CMAKE_COMPILER_IS_GNUCXX)
  if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 4.8)
    set (CMAKE_CXX_FLAGS "-std=c++0x ${CMAKE_CXX_FLAGS}")
  else ()
    set (CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")
  endif ()
else ()
  set (CMAKE_CXX_STANDARD 11)
endif ()

# When CMake 3.0.0 is required, remove this block (see CMP0042).
if (NOT DEFINED CMAKE_MACOSX_RPATH)
    set(CMAKE_MACOSX_RPATH 1)
endif()

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
