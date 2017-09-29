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

install(TARGETS builder DESTINATION bin)
