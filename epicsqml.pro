
TEMPLATE = subdirs


SUBDIRS += src/pvobject src/adimageprovider

OTHER_FILES += \
    PvComponents/CaBar.qml \
    PvComponents/CaChoiceButton.qml \
    PvComponents/CaMenu.qml \
    PvComponents/CaMessageButton.qml \
    PvComponents/CaOval.qml \
    PvComponents/CaSlider.qml \
    PvComponents/CaTextEntry.qml \
    PvComponents/CaTextLabel.qml \
    PvComponents/qmldir

# Copy qml files post build
#win32 {
#    DESTDIR_WIN = $${DESTDIR}
#    DESTDIR_WIN ~= s,/,\\,g
#    PWD_WIN = $${PWD}
#    PWD_WIN ~= s,/,\\,g
#    for(FILE, OTHER_FILES){
#        QMAKE_POST_LINK += $$quote(cmd /c copy /y $${PWD_WIN}\\$${FILE} $${DESTDIR_WIN}$$escape_expand(\\n\\t))
#    }
#}
#unix {
#    for(FILE, OTHER_FILES){
#        QMAKE_POST_LINK += $$quote(cp $${PWD}/$${FILE} $${DESTDIR}$$escape_expand(\\n\\t))
#    }
#}
