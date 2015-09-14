import QtQuick 2.1
import QtQuick.Controls 1.0

import PvComponents 1.0

/*!
    \qmltype CaRelatedDisplay
    \inqmlmodule PvComponents
    \brief Brings up more displays

    The Related Display provides a means of bringing up more displays.
    It is a menu button that usually has a graphic consisting of two overlapping squares and a label.
    See the paragraph above for other options for the graphic and label.
    The menu items denote the related displays that can be displayed.
    If there is only one related display specified in the menu,
    then the two overlapping squares and/or label appear in the middle of the button,
    and the related display is activated immediately without bringing up a menu.
    It is not activated until the button is released, so you can depress the button to check
    if there is more than one menu item or not, then abort by releasing the button away from the Related Display.
    If there is more than one item on the menu, the squares and/or label are at the left of the button.
    If Ctrl-Btn1 is used in place of Btn1 to select the new display,
    then the parent display goes away and is replaced by the new display.
    The new display can also be configured by "replace" property to always replace the parent.
*/

BaseItem {
    id: display
    /*! foreground color */
    property color foreground: ColorMap.color14
    /*! background color */
    property color background: ColorMap.color51 // color4 is the actual default
    /*! font family */
    property string fontFamily
    /*! font size */
    property int fontSize
    /*!
        text displayed on button

        Normally there are two overlapping squares on the button to denote a Related Display control.
        However, if the label text begins with  "-" (for example "-My Label"), the overlapping squares will not appear.
    */
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
        var window
        if (fname.substr(-4) == '.adl') {
            var qmlCmd = Utils.openADLDisplay(fname, args, baseWindow.fileName)
            window = Qt.createQmlObject(qmlCmd, display, fname)
        }
        else {
            var component = Qt.createComponent(fname)
            if (component.status == Component.Ready) {
                window = component.createObject()
            }
            else if (component.status == Component.Error)
                console.log(component.errorString())
            else
                component.onStatusChanged = function done(status) {
                    if (status == Component.Ready) {
                        window = component.createObject()
                    } else
                        console.log(component.errorString())
                }
        }
        window.title = fname
        window.visible = true
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
