set(SOURCES
    pvobject.cpp
    adimageprovider.cpp
    enums.cpp
    plugin.cpp
    utils.cpp
    shapes.cpp
    conversion.c
    qcustomplot.cpp
    plotitem.cpp
    baseitem.cpp
    basewindow.cpp
)

set(HEADERS
    pvobject.h
    adimageprovider.h
    enums.h
    plugin.h
    utils.h
    shapes.h
    qcustomplot.h
    plotitem.h
    baseitem.h
    basewindow.h
)

# When CMake 3.0.0 is required, remove this block (see CMP0042).
if (NOT DEFINED CMAKEMACOSX_RPATH)
    set(CMAKE_MACOSX_RPATH 0)
endif()

add_library(pvcomponentsplugin SHARED
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
    ${EPICS_INCLUDE_DIRS}
    ../ADLParser
)

target_link_libraries(pvcomponentsplugin Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Quick Qt5::PrintSupport)
target_link_libraries(pvcomponentsplugin ${EPICS_LIBRARIES})
target_link_libraries(pvcomponentsplugin ADLParser)
set_target_properties(pvcomponentsplugin PROPERTIES 
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/imports/PvComponents)

set(QML_JS_FILES
    DynamicAttr.qml
    CaADImage.qml
    CaBar.qml
    CaComposite.qml
    CaChoiceButton.qml
    CaMenu.qml
    CaMessageButton.qml
    CaOval.qml
    CaPolyline.qml
    CaSlider.qml
    Slider.qml
    Slider2.qml
    CaTextEntry.qml
    CaTextUpdate.qml
    CaText.qml
    qmldir
    CaPolygon.qml
    CaControl.qml
    CaRelatedDisplay.qml
    CaShellCommand.qml
    CaMeter.qml
    StyledButton.qml
    RDButton.qml
    CaByte.qml
    CaGraphics.qml
    CaRect.qml
    utils.js
    CaArc.qml
    CaImage.qml
    CaMonitor.qml
    CaStripChart.qml
    CaWheelSwitch.qml
    WheelSwitch.qml
    WheelDigit.qml
    Limits.qml
    StyledBar.qml
    Frame.qml
    Meter.qml
    CaCartesianPlot.qml
    StyledBar2.qml
    StyledAxis.qml
    ColorMap.qml
    CaIndicator.qml
    Indicator.qml
    PvInfoDialog.qml
    PvLimitsDialog.qml
    PromptDialog.qml
    images/arrow-down.png
    images/arrow-up.png
    images/button_down.png
    images/button_up.png
    images/doublerect.png
    images/focusframe.png
)

foreach(FILE ${QML_JS_FILES})
    # make subdirectory if not existing
    if (${CMAKE_MAJOR_VERSION} LESS 3)
        get_filename_component(DIR ${FILE} PATH)
    else ()
        get_filename_component(DIR ${FILE} DIRECTORY)
    endif()

    if (NOT IS_DIRECTORY ${CMAKE_BINARY_DIR}/imports/PvComponents/${DIR})
        file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/imports/PvComponents/${DIR})
    endif()
    # copy qml files prebuild
    add_custom_command(
        TARGET pvcomponentsplugin PRE_BUILD    
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${FILE} ${CMAKE_BINARY_DIR}/imports/PvComponents/${DIR}
        DEPENDS ${CMAKE_BINARY_DIR}/imports/PvComponents/${FILE}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )
endforeach()

