QML_IMPORT_PATH = .
QML_FILES += \
    $$PWD/qmldir \
    $$PWD/plugins.qmltypes \
    $$PWD/BaseWindow.qml \
    $$PWD/DynamicAttribute.qml \
    $$PWD/CaADImage.qml \
    $$PWD/CaArc.qml \
    $$PWD/CaBar.qml \
    $$PWD/CaByte.qml \
    $$PWD/CaCartesianPlot.qml \
    $$PWD/CaChoiceButton.qml \
    $$PWD/CaComposite.qml \
    $$PWD/CaControl.qml \
    $$PWD/CaGraphics.qml \
    $$PWD/CaImage.qml \
    $$PWD/CaIndicator.qml \
    $$PWD/CaMenu.qml \
    $$PWD/CaMessageButton.qml \
    $$PWD/CaMeter.qml \
    $$PWD/CaMonitor.qml \
    $$PWD/CaOval.qml \
    $$PWD/CaPolygon.qml \
    $$PWD/CaPolyline.qml \
    $$PWD/CaRect.qml \
    $$PWD/CaRelatedDisplay.qml \
    $$PWD/CaShellCommand.qml \
    $$PWD/CaSlider.qml \
    $$PWD/CaStripChart.qml \
    $$PWD/CaTextEntry.qml \
    $$PWD/CaTextUpdate.qml \
    $$PWD/CaText.qml \
    $$PWD/CaWheelSwitch.qml \
    $$PWD/utils.js \
    $$PWD/ColorMap.qml \
    $$PWD/DisplayListDialog.qml \
    $$PWD/PvInfoDialog.qml \
    $$PWD/PvLimitsDialog.qml \
    $$PWD/PromptDialog.qml \
    $$PWD/Private/qmldir \
    $$PWD/Private/Frame.qml \
    $$PWD/Private/Indicator.qml \
    $$PWD/Private/Meter.qml \
    $$PWD/Private/RDButton.qml \
    $$PWD/Private/SliderControl.qml \
    $$PWD/Private/StyledAxis.qml \
    $$PWD/Private/StyledBar.qml \
    $$PWD/Private/StyledButton.qml \
    $$PWD/Private/WheelDigit.qml \
    $$PWD/Private/WheelSwitch.qml \
    $$PWD/Private/images/arrow-down.png \
    $$PWD/Private/images/arrow-down-focus.png \
    $$PWD/Private/images/arrow-up.png \
    $$PWD/Private/images/arrow-up-focus.png \
    $$PWD/Private/images/button_down.png \
    $$PWD/Private/images/button_up.png \
    $$PWD/Private/images/focusframe.png \
    $$PWD/Private/images/groove_horz.png \
    $$PWD/Private/images/groove_vert.png

DESIGNER_FILES = \
    $$PWD/designer/BaseWindowSpecifics.qml \
    $$PWD/designer/CaArcSpecifics.qml \
    $$PWD/designer/CaBarSpecifics.qml \
    $$PWD/designer/CaByteSpecifics.qml \
    $$PWD/designer/CaCartesianPlotSpecifics.qml \
    $$PWD/designer/CaCompositeSpecifics.qml \
    $$PWD/designer/CaChoiceButtonSpecifics.qml \
    $$PWD/designer/CaImageSpecifics.qml \
    $$PWD/designer/CaIndicatorSpecifics.qml \
    $$PWD/designer/CaMenuSpecifics.qml \
    $$PWD/designer/CaMessageButtonSpecifics.qml \
    $$PWD/designer/CaMeterSpecifics.qml \
    $$PWD/designer/CaOvalSpecifics.qml \
    $$PWD/designer/CaPolygonSpecifics.qml \
    $$PWD/designer/CaPolylineSpecifics.qml \
    $$PWD/designer/CaRectSpecifics.qml \
    $$PWD/designer/CaRelatedDisplaySpecifics.qml \
    $$PWD/designer/CaShellCommandSpecifics.qml \
    $$PWD/designer/CaSliderSpecifics.qml \
    $$PWD/designer/CaStripChartSpecifics.qml \
    $$PWD/designer/CaTextSpecifics.qml \
    $$PWD/designer/CaTextEntrySpecifics.qml \
    $$PWD/designer/CaTextUpdateSpecifics.qml \
    $$PWD/designer/CaWheelSwitchSpecifics.qml \
    $$PWD/designer/ColorSection.qml \
    $$PWD/designer/ColorPalette.qml \
    $$PWD/designer/DataSourceSection.qml \
    $$PWD/designer/DynamicAttributeSection.qml \
    $$PWD/designer/GraphicsSection.qml \
    $$PWD/designer/LimitsSection.qml \
    $$PWD/designer/PolylineEditor.qml \
    $$PWD/designer/images/arc.png \
    $$PWD/designer/images/bar.png \
    $$PWD/designer/images/byte.png \
    $$PWD/designer/images/cartesianPlot.png \
    $$PWD/designer/images/choiceButton.png \
    $$PWD/designer/images/composite.png \
    $$PWD/designer/images/image.png \
    $$PWD/designer/images/indicator.png \
    $$PWD/designer/images/menu.png \
    $$PWD/designer/images/messageButton.png \
    $$PWD/designer/images/meter.png \
    $$PWD/designer/images/oval.png \
    $$PWD/designer/images/polygon.png \
    $$PWD/designer/images/polyline.png \
    $$PWD/designer/images/rectangle.png \
    $$PWD/designer/images/relatedDisplay.png \
    $$PWD/designer/images/shellCommand.png \
    $$PWD/designer/images/slider.png \
    $$PWD/designer/images/stripChart.png \
    $$PWD/designer/images/text.png \
    $$PWD/designer/images/textEntry.png \
    $$PWD/designer/images/textUpdate.png \
    $$PWD/designer/images/wheelSwitch.png \
    $$PWD/designer/images/window.png \
    $$PWD/designer/csdatacomponents.metainfo
    
OTHER_FILES = $$QML_FILES $$DESIGNER_FILES

# These rules are to copy qml file to TARGETPATH
defineReplace(qmlModStripSrcDir) {
    return($$TARGETPATH/$$relative_path($$1, $$_PRO_FILE_PWD_))
}

qmlfiles2build.input = OTHER_FILES
qmlfiles2build.output_function = qmlModStripSrcDir
qmlfiles2build.commands = $$QMAKE_COPY ${QMAKE_FILE_NAME} ${QMAKE_FILE_OUT}
qmlfiles2build.depends = $$OTHER_FILES
qmlfiles2build.CONFIG = no_link target_predeps

QMAKE_EXTRA_COMPILERS += qmlfiles2build
