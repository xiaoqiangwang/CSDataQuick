QML_IMPORT_PATH = .
QML_FILES += \
    $$PWD/DynamicAttr.qml \
    $$PWD/CaADImage.qml \
    $$PWD/CaBar.qml \
    $$PWD/CaComposite.qml \
    $$PWD/CaChoiceButton.qml \
    $$PWD/CaMenu.qml \
    $$PWD/CaMessageButton.qml \
    $$PWD/CaOval.qml \
    $$PWD/CaPolyline.qml \
    $$PWD/CaSlider.qml \
    $$PWD/Slider.qml \
    $$PWD/Slider2.qml \
    $$PWD/CaTextEntry.qml \
    $$PWD/CaTextUpdate.qml \
    $$PWD/CaText.qml \
    $$PWD/qmldir \
    $$PWD/CaPolygon.qml \
    $$PWD/CaControl.qml \
    $$PWD/CaRelatedDisplay.qml \
    $$PWD/CaShellCommand.qml \
    $$PWD/CaMeter.qml \
    $$PWD/StyledButton.qml \
    $$PWD/RDButton.qml \
    $$PWD/CaByte.qml \
    $$PWD/CaGraphics.qml \
    $$PWD/CaRect.qml \
    $$PWD/utils.js \
    $$PWD/CaArc.qml \
    $$PWD/CaImage.qml \
    $$PWD/CaMonitor.qml \
    $$PWD/CaStripChart.qml \
    $$PWD/CaWheelSwitch.qml \
    $$PWD/WheelSwitch.qml \
    $$PWD/WheelDigit.qml \
    $$PWD/Limits.qml \
    $$PWD/StyledBar.qml \
    $$PWD/Frame.qml \
    $$PWD/Meter.qml \
    $$PWD/CaCartesianPlot.qml \
    $$PWD/StyledBar2.qml \
    $$PWD/StyledAxis.qml \
    $$PWD/ColorMap.qml \
    $$PWD/CaIndicator.qml \
    $$PWD/Indicator.qml \
    $$PWD/PvInfoDialog.qml \
    $$PWD/PvLimitsDialog.qml \
    $$PWD/images/arrow-down.png \
    $$PWD/images/arrow-down-focus.png \
    $$PWD/images/arrow-up.png \
    $$PWD/images/arrow-up-focus.png \
    $$PWD/images/button_down.png \
    $$PWD/images/button_up.png \
    $$PWD/images/doublerect.png \
    $$PWD/images/focusframe.png

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
