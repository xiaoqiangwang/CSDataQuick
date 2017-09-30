set(SOURCES
    enums.cpp
    plugin.cpp
    utils.cpp
    shapes.cpp
    conversion.c
    qcustomplot.cpp
    plotitem.cpp
    baseitem.cpp
    windowmanager.cpp
    geometryspecification.cpp
    attribute.cpp
    adimage.cpp
    csimageitem.cpp
    mjpeg.cpp
)

set(HEADERS
    enums.h
    plugin.h
    utils.h
    shapes.h
    qcustomplot.h
    plotitem.h
    baseitem.h
    windowmanager.h
    attribute.h
    adimage.h
    csimageitem.h
    mjpeg.h
)

add_library(csdatacomponentsplugin MODULE
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
    ${Qt5Concurrent_INCLUDE_DIRS}
    ${Qt5PrintSupport_INCLUDE_DIRS}
    ${EPICS_INCLUDE_DIRS}
    ../ADLParser
    ../csdata/core
)

add_definitions(${EPICS_DEFINITIONS})
add_definitions(-D_CRT_SECURE_NO_WARNINGS)

target_link_libraries(csdatacomponentsplugin Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Quick Qt5::Concurrent Qt5::PrintSupport)
target_link_libraries(csdatacomponentsplugin ${EPICS_LIBRARIES})
target_link_libraries(csdatacomponentsplugin ADLParser)
target_link_libraries(csdatacomponentsplugin csdata)
set_target_properties(csdatacomponentsplugin PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/qml/CSDataQuick/Components)

if(APPLE)
set_target_properties(csdatacomponentsplugin PROPERTIES
    INSTALL_RPATH "@loader_path/../../../lib"
)
endif()

set(QML_JS_FILES
    qmldir
    plugins.qmltypes
    BaseWindow.qml
    DynamicAttribute.qml
    CS2DSscan.qml
    CSADImage.qml
    CSArc.qml
    CSBar.qml
    CSByte.qml
    CSCartesianPlot.qml
    CSChoiceButton.qml
    CSComposite.qml
    CSControl.qml
    CSGraphics.qml
    CSImage.qml
    CSIndicator.qml
    CSMenu.qml
    CSMessageButton.qml
    CSMeter.qml
    CSMJPEGVideo.qml
    CSMonitor.qml
    CSOval.qml
    CSPolygon.qml
    CSPolyline.qml
    CSRect.qml
    CSRelatedDisplay.qml
    CSShellCommand.qml
    CSSlider.qml
    CSSpinBox.qml
    CSStripChart.qml
    CSTextArea.qml
    CSTextEntry.qml
    CSTextEntryArray.qml
    CSTextUpdate.qml
    CSText.qml
    CSWheelSwitch.qml
    utils.js
    ColorMap.qml
    DisplayListDialog.qml
    DataInfoDialog.qml
    DataLimitsDialog.qml
    DataTableDialog.qml
    PromptDialog.qml
    Private/qmldir
    Private/ContextMenu.qml
    Private/Indicator.qml
    Private/Meter.qml
    Private/RDButton.qml
    Private/SliderControl.qml
    Private/StyledAxis.qml
    Private/StyledBar.qml
    Private/StyledButton.qml
    Private/StyledButton2.qml
    Private/StyledFrame.qml
    Private/StyledTextEntry.qml
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
    designer/AligmentHorizontalButtonsFix.qml
    designer/BaseWindowSpecifics.qml
    designer/CS2DSscanSpecifics.qml
    designer/CSADImageSpecifics.qml
    designer/CSArcSpecifics.qml
    designer/CSBarSpecifics.qml
    designer/CSByteSpecifics.qml
    designer/CSCartesianPlotSpecifics.qml
    designer/CSCompositeSpecifics.qml
    designer/CSChoiceButtonSpecifics.qml
    designer/CSImageSpecifics.qml
    designer/CSIndicatorSpecifics.qml
    designer/CSMenuSpecifics.qml
    designer/CSMessageButtonSpecifics.qml
    designer/CSMeterSpecifics.qml
    designer/CSOvalSpecifics.qml
    designer/CSPolygonSpecifics.qml
    designer/CSPolylineSpecifics.qml
    designer/CSRectSpecifics.qml
    designer/CSRelatedDisplaySpecifics.qml
    designer/CSShellCommandSpecifics.qml
    designer/CSSliderSpecifics.qml
    designer/CSSpinBoxSpecifics.qml
    designer/CSStripChartSpecifics.qml
    designer/CSTextSpecifics.qml
    designer/CSTextAreaSpecifics.qml
    designer/CSTextEntrySpecifics.qml
    designer/CSTextEntryArraySpecifics.qml
    designer/CSTextUpdateSpecifics.qml
    designer/CSWheelSwitchSpecifics.qml
    designer/CartesianPlotEditor.qml
    designer/ColorSection.qml
    designer/ColorPalette.qml
    designer/ComboBoxFix.qml
    designer/CommandEditor.qml
    designer/DataSourceSection.qml
    designer/DisplayEditor.qml
    designer/DynamicAttributeSection.qml
    designer/FieldEditor.qml
    designer/GraphicsSection.qml
    designer/IconButton.qml
    designer/LimitsSection.qml
    designer/PolylineEditor.qml
    designer/StripChartEditor.qml
    designer/TextFormatSection.qml
    designer/images/2dsscan.png
    designer/images/areaDetector.png
    designer/images/arc.png
    designer/images/bar.png
    designer/images/byte.png
    designer/images/cartesianPlot.png
    designer/images/choiceButton.png
    designer/images/composite.png
    designer/images/image.png
    designer/images/indicator.png
    designer/images/menu.png
    designer/images/messageButton.png
    designer/images/meter.png
    designer/images/mjpegvideo.png
    designer/images/oval.png
    designer/images/polygon.png
    designer/images/polyline.png
    designer/images/rectangle.png
    designer/images/relatedDisplay.png
    designer/images/shellCommand.png
    designer/images/slider.png
    designer/images/spinbox.png
    designer/images/stripChart.png
    designer/images/text.png
    designer/images/textArea.png
    designer/images/textEntry.png
    designer/images/textEntryArray.png
    designer/images/textUpdate.png
    designer/images/wheelSwitch.png
    designer/images/window.png
    designer/images/add.png
    designer/images/apply.png
    designer/images/arrow_down.png
    designer/images/arrow_up.png
    designer/images/delete.png
    designer/csdatacomponents.metainfo
)

foreach(FILE ${QML_JS_FILES})
    set(DST_QML_JS_FILES ${DST_QML_JS_FILES} ${CMAKE_BINARY_DIR}/qml/CSDataQuick/Components/${FILE})
endforeach()

add_custom_target(CopyQMLFiles ALL
    DEPENDS ${DST_QML_JS_FILES})

foreach(FILE ${QML_JS_FILES})
    # make subdirectory if not existing
    if (${CMAKE_MAJOR_VERSION} LESS 3)
        get_filename_component(DIR ${FILE} PATH)
    else ()
        get_filename_component(DIR ${FILE} DIRECTORY)
    endif()

    if (NOT IS_DIRECTORY ${CMAKE_BINARY_DIR}/qml/CSDataQuick/Components/${DIR})
        file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/qml/CSDataQuick/Components/${DIR})
    endif()
    # copy qml files prebuild
    add_custom_command(
        OUTPUT ${CMAKE_BINARY_DIR}/qml/CSDataQuick/Components/${FILE}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${FILE} ${CMAKE_BINARY_DIR}/qml/CSDataQuick/Components/${DIR}
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${FILE}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )
endforeach()

add_dependencies(csdatacomponentsplugin CopyQMLFiles)

install(TARGETS csdatacomponentsplugin DESTINATION qml/CSDataQuick/Components)
foreach (file ${QML_JS_FILES})
    get_filename_component(dir ${file} DIRECTORY)
    install(FILES ${file} DESTINATION qml/CSDataQuick/Components/${dir})
endforeach()
