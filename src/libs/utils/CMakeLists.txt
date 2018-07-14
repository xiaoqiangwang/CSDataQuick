set(SOURCES
    enums.cpp
    utils.cpp
    conversion.c
    geometryspecification.cpp 
)

set(HEADERS
    enums.h
    utils.h
    utils_global.h
)

add_library(csutils SHARED
    ${SOURCES}
    ${HEADERS}
)

include_directories(
    ${Qt5Core_INCLUDE_DIRS}
    ${Qt5Gui_INCLUDE_DIRS}
    ${Qt5Qml_INCLUDE_DIRS}
    ${Qt5Quick_INCLUDE_DIRS}
    ${EPICS_INCLUDE_DIRS}
    ${CMAKE_BINARY_DIR}/src/include
    ${CMAKE_SOURCE_DIR}/src/libs/ADLParser
    ${CMAKE_SOURCE_DIR}/src/libs/EDLParser
    ${CMAKE_SOURCE_DIR}/src/libs/UIParser
    ${CMAKE_SOURCE_DIR}/src/libs/csdata
)

add_definitions(${EPICS_DEFINITIONS})
add_definitions(-D_CRT_SECURE_NO_WARNINGS)
add_definitions(-DUTILS_LIBRARY)

target_link_libraries(csutils
    Qt5::Core
    Qt5::Gui
    Qt5::Qml
    Qt5::Quick
    ${EPICS_LIBRARIES}
    ADLParser
    EDLParser
    UIParser
    csdata
)

set_target_properties(csutils PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
)

install(TARGETS csutils
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
)
