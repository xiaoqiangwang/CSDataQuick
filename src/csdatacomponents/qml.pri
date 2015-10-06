QML_IMPORT_PATH = .
QML_FILES += \
    $$PWD/qmldir \
    $$PWD/BaseWindow.qml \
    $$PWD/DynamicAttr.qml \
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
    $$PWD/Limits.qml \
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
    $$PWD/images/arrow-down.png \
    $$PWD/images/arrow-down-focus.png \
    $$PWD/images/arrow-up.png \
    $$PWD/images/arrow-up-focus.png \
    $$PWD/images/button_down.png \
    $$PWD/images/button_up.png \
    $$PWD/images/focusframe.png \
    $$PWD/images/groove_horz.png \
    $$PWD/images/groove_vert.png

OTHER_FILES = $$QML_FILES

# These rules are to copy qml file to TARGETPATH
defineReplace(qmlModStripSrcDir) {
    return($$TARGETPATH/$$relative_path($$1, $$_PRO_FILE_PWD_))
}

qmlfiles2build.input = QML_FILES
qmlfiles2build.output_function = qmlModStripSrcDir
qmlfiles2build.commands = $$QMAKE_COPY ${QMAKE_FILE_NAME} ${QMAKE_FILE_OUT}
qmlfiles2build.depends = $$QML_FILES
qmlfiles2build.CONFIG = no_link target_predeps

QMAKE_EXTRA_COMPILERS += qmlfiles2build
