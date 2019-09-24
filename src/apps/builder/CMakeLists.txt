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
add_executable(builder WIN32 MACOSX_BUNDLE
    ${SOURCES}
    ${RESOURCES}
)
target_link_libraries(builder PRIVATE
    Qt5::Gui
    Qt5::Qml
)

SET_TARGET_PROPERTIES(builder PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

if(APPLE)
SET_TARGET_PROPERTIES(builder PROPERTIES
    INSTALL_RPATH "@executable_path/../../../../lib"
)
endif()

install(TARGETS builder DESTINATION ${KDE_INSTALL_BINDIR})
#if (APPLE)
#install(CODE "
#file(WRITE \"\${KDE_INSTALL_PREFIX}/${KDE_INSTALL_BINDIR}/builder.app/Contents/Resources/qt.conf\" \"[Paths]\nPrefix=../../..\")
#" COMPONENT Runtime)
#endif()
