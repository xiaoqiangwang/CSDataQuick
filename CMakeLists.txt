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

find_package(Qt5 5.6
    COMPONENTS Core Gui Widgets Qml Quick Concurrent PrintSupport)

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

# When CMake 3.0.0 is required, remove this block (see CMP0042).
if (NOT DEFINED CMAKE_MACOSX_RPATH)
    set(CMAKE_MACOSX_RPATH 1)
endif()

find_package(EPICS REQUIRED)

set(ENV{QML_IMPORT_PATH} )

add_subdirectory(src)
add_subdirectory(docs)
add_subdirectory(examples)
add_subdirectory(tools)

# Copy QtGui plugins
foreach(plugin ${Qt5Gui_PLUGINS})
  get_target_property(_loc ${plugin} LOCATION)
  get_filename_component(_plugindir ${_loc} PATH)
  get_filename_component(_pluginname ${_plugindir} NAME)
  install(CODE "
  file(COPY ${_loc} DESTINATION \${CMAKE_INSTALL_PREFIX}/plugins/${_pluginname})
      " COMPONENT Runtime)
endforeach()
# Copy Qt frameworks
foreach(QtLIB Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Network Qt5::Qml Qt5::Quick Qt5::Concurrent Qt5::PrintSupport)
    if(APPLE)
        get_target_property(QT_LIBRARY_DIR ${QtLIB} LOCATION)
        get_filename_component(QT_LIBRARY_DIR ${QT_LIBRARY_DIR} PATH)
        install(DIRECTORY ${QT_LIBRARY_DIR} DESTINATION lib
            PATTERN "Headers" EXCLUDE
            PATTERN "*_debug" EXCLUDE
            PATTERN "*.prl" EXCLUDE
        )
    elseif(UNIX)
        get_target_property(QT_LIBRARY_DIR ${QtLIB} LOCATION)
        install(FILES ${QT_LIBRARY_DIR} DESTINATION lib)
    elseif(WIN32)
        get_target_property(QT_LIBRARY_DIR ${QtLIB} LOCATION)
        install(FILES ${QT_LIBRARY_DIR} DESTINATION bin)
    endif ()
endforeach()
# Copy QtQuick 2
get_target_property (QT_QMAKE_EXECUTABLE Qt5::qmake IMPORTED_LOCATION)
function(_QT5_QUERY_QMAKE VAR RESULT)
  execute_process(COMMAND "${QT_QMAKE_EXECUTABLE}" -query ${VAR}
    RESULT_VARIABLE return_code
    OUTPUT_VARIABLE output
    OUTPUT_STRIP_TRAILING_WHITESPACE ERROR_STRIP_TRAILING_WHITESPACE)
  if(NOT return_code)
    file(TO_CMAKE_PATH "${output}" output)
    set(${RESULT} ${output} PARENT_SCOPE)
  endif()
endfunction()

_qt5_query_qmake(QT_INSTALL_QML QT_QML_DIR)
install(DIRECTORY ${QT_QML_DIR}/ DESTINATION qml
    PATTERN "*_debug.dylib" EXCLUDE
    PATTERN "*.dSYM" EXCLUDE
    PATTERN "*.pdb" EXCLUDE
    PATTERN "*.qmlc" EXCLUDE
)
# Copy EPICS (Windows only)
if(WIN32)
    install(FILES ${EPICS_SHARED_LIBRARIES} DESTINATION bin)
endif()
