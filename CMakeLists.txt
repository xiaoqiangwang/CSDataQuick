project(CSDataQuick)
cmake_minimum_required(VERSION 2.8.6 FATAL_ERROR)
set(CSDATAQUICK_VERSION_MAJOR 1)
set(CSDATAQUICK_VERSION_MINOR 0)
set(CSDATAQUICK_VERSION_RELEASE 0)
set(CSDATAQUICK_VERSION "${CSDATAQUICK_VERSION_MAJOR}.${CSDATAQUICK_VERSION_MINOR}.${CSDATAQUICK_VERSION_RELEASE}" )

# Include custom cmake modules
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

find_package(Qt5Core)
find_package(Qt5Gui)
find_package(Qt5Widgets)
find_package(Qt5Qml)
find_package(Qt5Quick)
find_package(Qt5Concurrent)
find_package(Qt5PrintSupport)
# For an older CMake to add Qt5<Module>_EXECUTABLE_COMPILE_FLAGS to the
# CMAKE_CXX_FLAGS so that the -fPIE flags are added to the compile flags if
# necessary (as is the default with Qt 5).
if (${CMAKE_MAJOR_VERSION} LESS 3)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")
    # -fPIC
    if (${CMAKE_COMPILER_IS_GNUCXX})
        set(CMAKE_CXX_FLAGS "-fPIC ${CMAKE_CXX_FLAGS}")
    endif()

    # Qt 5.7 requires C++11
    if (NOT(${Qt5Core_VERSION} VERSION_LESS 5.7))
        set(CMAKE_CXX_FLAGS "--std=gnu++11 ${CMAKE_CXX_FLAGS}")
    endif()
endif()

find_package(EPICS)

set(ENV{QML_IMPORT_PATH} )

add_subdirectory(src)
add_subdirectory(docs)
add_subdirectory(examples)
add_subdirectory(tools)
