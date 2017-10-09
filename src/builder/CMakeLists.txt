set(CMAKE_AUTORCC ON)

set(SOURCES
    main.cpp
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
add_executable(builder MACOSX_BUNDLE
    ${SOURCES}
    ${RESOURCES}
)
elseif(WIN32)
add_executable(builder WIN32
    ${SOURCES}
    ${RESOURCES}
)
else()
add_executable(builder
    ${SOURCES}
    ${RESOURCES}
)
endif()
include_directories(
    ${Qt5Gui_INCLUDE_DIRS}
    ${Qt5Qml_INCLUDE_DIRS}
)
target_link_libraries(builder
    Qt5::Gui
    Qt5::Qml
)

SET_TARGET_PROPERTIES(builder PROPERTIES RUNTIME_OUTPUT_DIRECTORY 
    ${CMAKE_BINARY_DIR}/bin)

if(APPLE)
SET_TARGET_PROPERTIES(builder PROPERTIES
    INSTALL_RPATH "@executable_path/../../../../lib"
)
endif()

install(TARGETS builder DESTINATION bin)
if (APPLE)
install(CODE "
    file(WRITE \"\${CMAKE_INSTALL_PREFIX}/bin/builder.app/Contents/Resources/qt.conf\" \"[Paths]\nPrefix=../../..\")
" COMPONENT Runtime)
endif()
