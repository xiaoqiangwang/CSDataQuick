set(CMAKE_AUTORCC ON)

set(SOURCES
    ipcserver.cpp
    main.cpp
)

set(HEADERS
    ipcserver.h
)

if (CMAKE_VERSION VERSION_LESS 3.0)
    qt5_add_resources(RESOURCES
        main.qrc
    )
else ()
    set(RESOURCES
        main.qrc
    )
endif()

if(APPLE)
add_executable(ADLViewer MACOSX_BUNDLE
    ${SOURCES}
    ${HEADERS}
    ${RESOURCES}
)
else()
add_executable(ADLViewer
    ${SOURCES}
    ${HEADERS}
    ${RESOURCES}
)
endif()

include_directories(
    ${Qt5Qt_INCLUDE_DIRS}
    ${Qt5Core_INCLUDE_DIRS}
    ${Qt5Network_INCLUDE_DIRS}
    ${Qt5Gui_INCLUDE_DIRS}
    ${Qt5Qml_INCLUDE_DIRS}
    ${Qt5Quick_INCLUDE_DIRS}
    ../ADLParser
    ../csdata/core
)
target_link_libraries(ADLViewer
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
    Qt5::Qml
    Qt5::Quick
    Qt5::Network
    ADLParser
)
if(APPLE)
    find_library(OSXSecurity Security)
    target_link_libraries(ADLViewer ${OSXSecurity})
endif()

SET_TARGET_PROPERTIES(ADLViewer PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

if(APPLE)
SET_TARGET_PROPERTIES(ADLViewer PROPERTIES
    INSTALL_RPATH "@executable_path/../../../../lib")
elseif(UNIX)
SET_TARGET_PROPERTIES(ADLViewer PROPERTIES
    INSTALL_RPATH "$ORIGIN/../lib")
endif()

install(TARGETS ADLViewer DESTINATION bin)
if (APPLE)
install(CODE "
    file(WRITE \"\${CMAKE_INSTALL_PREFIX}/bin/ADLViewer.app/Contents/Resources/qt.conf\" \"[Paths]\nPrefix=../../..\")
" COMPONENT Runtime)
endif()
