set(CMAKE_AUTORCC ON)

set (APPNAME Viewer)

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

add_executable(${APPNAME} WIN32 MACOSX_BUNDLE
    ${SOURCES}
    ${HEADERS}
    ${RESOURCES}
)

target_include_directories(${APPNAME} PRIVATE
    ${CMAKE_BINARY_DIR}/src/include
)
target_link_libraries(${APPNAME} PRIVATE
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
    Qt5::Qml
    Qt5::Quick
    Qt5::Network
    csparser
)
if(APPLE)
    find_library(OSXSecurity Security)
    target_link_libraries(${APPNAME} PRIVATE ${OSXSecurity})
endif()

SET_TARGET_PROPERTIES(${APPNAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

if(APPLE)
    SET_TARGET_PROPERTIES(${APPNAME} PROPERTIES
        INSTALL_RPATH "@executable_path/../../../../lib")
elseif(UNIX)
    SET_TARGET_PROPERTIES(${APPNAME} PROPERTIES
        INSTALL_RPATH "$ORIGIN/../lib")
endif()

install(TARGETS ${APPNAME} DESTINATION ${KDE_INSTALL_BINDIR})
#if (APPLE)
#install(CODE "
#file(WRITE \"\${CMAKE_INSTALL_PREFIX}/${KDE_INSTALL_BINDIR}/${APPNAME}.app/Contents/Resources/qt.conf\" \"[Paths]\nPrefix=../../..\")
#" COMPONENT Runtime)
#endif()
