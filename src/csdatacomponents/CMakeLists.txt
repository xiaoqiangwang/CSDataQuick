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
    windowmanager.cpp
    geometryspecification.cpp
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
    windowmanager.h
)

# When CMake 3.0.0 is required, remove this block (see CMP0042).
if (NOT DEFINED CMAKEMACOSX_RPATH)
    set(CMAKE_MACOSX_RPATH 0)
endif()

add_library(csdatacomponentsplugin SHARED
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

target_link_libraries(csdatacomponentsplugin Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Quick Qt5::PrintSupport)
target_link_libraries(csdatacomponentsplugin ${EPICS_LIBRARIES})
target_link_libraries(csdatacomponentsplugin ADLParser)
set_target_properties(csdatacomponentsplugin PROPERTIES 
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/imports/CSData/Components)

set(QML_JS_FILES
    qmldir 
    BaseWindow.qml 
    DynamicAttr.qml 
    CaADImage.qml 
    CaArc.qml 
    CaBar.qml 
    CaByte.qml 
    CaCartesianPlot.qml 
    CaChoiceButton.qml 
    CaComposite.qml 
    CaControl.qml 
    CaGraphics.qml 
    CaImage.qml 
    CaIndicator.qml 
    CaMenu.qml 
    CaMessageButton.qml 
    CaMeter.qml 
    CaMonitor.qml 
    CaOval.qml 
    CaPolygon.qml 
    CaPolyline.qml 
    CaRect.qml 
    CaRelatedDisplay.qml 
    CaShellCommand.qml 
    CaSlider.qml 
    CaStripChart.qml 
    CaTextEntry.qml 
    CaTextUpdate.qml 
    CaText.qml 
    CaWheelSwitch.qml 
    utils.js 
    ColorMap.qml 
    DisplayListDialog.qml 
    Limits.qml 
    PvInfoDialog.qml 
    PvLimitsDialog.qml 
    PromptDialog.qml 
    Private/qmldir 
    Private/Frame.qml 
    Private/Indicator.qml 
    Private/Meter.qml 
    Private/RDButton.qml 
    Private/SliderControl.qml 
    Private/StyledAxis.qml 
    Private/StyledBar.qml 
    Private/StyledButton.qml 
    Private/WheelDigit.qml 
    Private/WheelSwitch.qml 
    Private/images/arrow-down.png 
    Private/images/arrow-down-focus.png 
    Private/images/arrow-up.png 
    Private/images/arrow-up-focus.png 
    Private/images/button_down.png 
    Private/images/button_up.png 
    Private/images/focusframe.png 
    Private/images/groove_horz.png 
    Private/images/groove_vert.png
)

foreach(FILE ${QML_JS_FILES})
    # make subdirectory if not existing
    if (${CMAKE_MAJOR_VERSION} LESS 3)
        get_filename_component(DIR ${FILE} PATH)
    else ()
        get_filename_component(DIR ${FILE} DIRECTORY)
    endif()

    if (NOT IS_DIRECTORY ${CMAKE_BINARY_DIR}/imports/CSData/Components/${DIR})
        file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/imports/CSData/Components/${DIR})
    endif()
    # copy qml files prebuild
    add_custom_command(
        TARGET csdatacomponentsplugin PRE_BUILD    
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${FILE} ${CMAKE_BINARY_DIR}/imports/CSData/Components/${DIR}
        DEPENDS ${CMAKE_BINARY_DIR}/imports/CSData/Components/${FILE}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )
endforeach()

