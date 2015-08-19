TEMPLATE = aux

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
    $$PWD/Meter.qml \
    $$PWD/CaCartesianPlot.qml \
    $$PWD/images/arrow-down.png \
    $$PWD/images/arrow-up.png \
    $$PWD/images/button_down.png \
    $$PWD/images/button_up.png \
    $$PWD/images/doubleect.png \
    $$PWD/images/focusframe.png

OTHER_FILES = $$QML_FILES \
    StyledBar2.qml \
    StyledAxis.qml
QML_FILES.TARGET = $$PWD/../PvComponents
