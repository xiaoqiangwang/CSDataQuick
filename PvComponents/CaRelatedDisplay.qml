import QtQuick 2.1
import QtQuick.Controls 1.0

import PvComponents 1.0

/*!
    \qmltype CaRelatedDisplay
    \inqmlmodule PvComponents
    \brief Brings up more displays
*/

Rectangle {
    id: display
    /*! foreground color */
    property color foreground: '#000000'
    /*! background color */
    property color background: '#73dfff' // '#bbbbbb' is the actual default
    /*! font family */
    property string fontFamily
    /*! font size */
    property int fontSize
    /*! text displayed on button */
    property string label: ''
    /*!
        \qmlproperty enumeration visual
        \list
            \li 0: Use a pull down menu for the choices.
            \li 1: Use a row of buttons for the choices.
            \li 2: Use a column of buttons for the choices.
        \endlist
    */
    property int visual: 0

    /*! related display list model */
    property ListModel model: ListModel {
    }

    /*!
        \internal
    */
    function load(fname, args, remove) {
        // TODO: substitue "name=values"
        // TODO: Handel both adl and qml
        var component = Qt.createComponent(fname)
        if (component.status == Component.Ready)
            component.createObject()
        else if (component.status == Component.Error)
            console.log(component.errorString())
        else
            component.onStatusChanged = function done(status) {
                if (status == Component.Ready) {
                    component.createObject()
                } else
                    console.log(component.errorString())
            }
    }

    Component.onCompleted: {
        if (visual == 3)
            visible = false
        var label, fname, args, remove;
        var imp ='import QtQuick 2.1; import QtQuick.Controls 1.0; import PvComponents 1.0;'

        // If button label starts with '-', remove it
        // Otherwise attach an icon in front
        var btnLabel
        var icon
        if (display.label.charAt(0) == '-') {
            icon = ''
            btnLabel = display.label.substring(1)
        } else {
            icon = 'iconSource: "images/doublerect.png";'
            btnLabel = display.label
        }

        // If only one display file, one push button will be used
        if (display.model.count <= 1) {
            // Compose button creation statements
            var qmlCmd = imp +
                        'StyledButton {' +
                            'anchors.fill: parent;' +
                            'text: "%1";'.arg(btnLabel) +
                            'pixelSize: display.fontSize;'+
                            'fontFamily: display.fontFamily;' +
                            'background: parent.background;' +
                            'foreground: parent.foreground;' +
                             icon
            // If model is empty, no signal will be triggered
            if (display.model.count == 1)
                qmlCmd += 'onClicked:  load("%1", "%2", "%3");'
                            .arg(display.model.get(0).fname)
                            .arg(display.model.get(0).args)
                            .arg(display.model.get(0).remove)
            qmlCmd += '}'
            var btn = Qt.createQmlObject(qmlCmd, display, 'button')
            btn.align = Text.AlignHCenter
            return
        }

        switch (display.visual) {
        case 0: // Button with popupmenu
            // Compose button creation statements
            var btnCmd = imp +
                        'StyledButton {' +
                            'anchors.fill: parent;' +
                            'text: "%1";' +
                            'pixelSize: display.fontSize;'+
                            'fontFamily: display.fontFamily;' +
                            'menu: Menu{}\n' +
                            'background: parent.background;' +
                            'foreground: parent.foreground;' +
                            icon + '}'
            // Create button and pulldown menu
            var btn = Qt.createQmlObject(btnCmd.arg(btnLabel), display, 'button')
            btn.align = Text.AlignLeft
            for(var i=0; i<model.count; i++) {
                label = model.get(i).label
                fname = model.get(i).fname
                args = model.get(i).args
                remove = model.get(i).remove
                var action = Qt.createQmlObject('import QtQuick 2.1; import QtQuick.Controls 1.0; Action{onTriggered: load("%1", "%2", %3)}'
                                                .arg(fname).arg(args).arg(remove), display, 'action')
                var item = btn.menu.insertItem(i, label);
                item.action = action
            }
            break;
        case 1: // Row of button
        case 2: // Column of buttons
            var layout
            if (display.visual == 1)
                layout = 'RowLayout'
            else
                layout = 'ColumnLayout'
            var qmlcmd = imp + 'import QtQuick.Layouts 1.0;' + layout + ' {anchors.fill: parent; spacing: 0;'
            for(var i=0; i<model.count; i++) {
                label = model.get(i).label
                fname = model.get(i).fname
                args = model.get(i).args
                remove = model.get(i).remove
                qmlcmd += 'StyledButton{text: "%1";'.arg(label) +
                        'onClicked: load("%1", "%2", %3);'.arg(fname).arg(args).arg(remove)  +
                              'background: display.background;' +
                              'foreground: display.foreground;' +
                              'Layout.fillWidth: true;' +
                              'Layout.fillHeight: true' +
                          ' }'
            }
            qmlcmd += '}'
            Qt.createQmlObject(qmlcmd, display, 'item')
            break;
        default:
            console.log('CaRelatedDisplay: Unknown visual style ' + display.visual)
            break;
        }
    }
}
