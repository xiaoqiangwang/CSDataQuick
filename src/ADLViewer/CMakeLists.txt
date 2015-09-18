set(SOURCES
    viewer.cpp
    ipcserver.cpp
    main.cpp
)

set(HEADERS
    viewer.h
    ipcserver.h
)

add_executable(ADLViewer
    ${SOURCES}
    ${HEADERS}
)

include_directories(
    ${Qt5Qt_INCLUDE_DIRS}
    ${Qt5Core_INCLUDE_DIRS}
    ${Qt5Gui_INCLUDE_DIRS}
    ${Qt5Widgets_INCLUDE_DIRS}
    ${Qt5Qml_INCLUDE_DIRS}
    ${Qt5Quick_INCLUDE_DIRS}
    ${Qt5PrintSupport_INCLUDE_DIRS}
    ../ADLParser
)
target_link_libraries(ADLViewer Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Quick Qt5::PrintSupport)
target_link_libraries(ADLViewer ADLParser)
if(APPLE)
    find_library(OSXSecurity Security)
    target_link_libraries(ADLViewer ${OSXSecurity})
endif()

SET_TARGET_PROPERTIES(ADLViewer PROPERTIES RUNTIME_OUTPUT_DIRECTORY 
    ${CMAKE_BINARY_DIR}/bin)
