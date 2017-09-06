project(CSDataQuick)
cmake_minimum_required(VERSION 2.8.10 FATAL_ERROR)
set(CSDATAQUICK_VERSION_MAJOR 1)
set(CSDATAQUICK_VERSION_MINOR 0)
set(CSDATAQUICK_VERSION_RELEASE 0)
set(CSDATAQUICK_VERSION "${CSDATAQUICK_VERSION_MAJOR}.${CSDATAQUICK_VERSION_MINOR}.${CSDATAQUICK_VERSION_RELEASE}" )

# Include custom cmake modules
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

cmake_policy(SET CMP0020 NEW)

# Set minimum Qt version
set(QT_MINIMUM_VERSION 5.6)

find_package(Qt5Core ${QT_MINIMUM_VERSION})
find_package(Qt5Gui ${QT_MINIMUM_VERSION})
find_package(Qt5Widgets ${QT_MINIMUM_VERSION})
find_package(Qt5Qml ${QT_MINIMUM_VERSION})
find_package(Qt5Quick ${QT_MINIMUM_VERSION})
find_package(Qt5Concurrent ${QT_MINIMUM_VERSION})
find_package(Qt5PrintSupport ${QT_MINIMUM_VERSION})

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

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" AND CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 3.5)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-inconsistent-missing-override")
endif ()

find_package(EPICS REQUIRED)

set(ENV{QML_IMPORT_PATH} )

add_subdirectory(src)
add_subdirectory(docs)
add_subdirectory(examples)
add_subdirectory(tools)
