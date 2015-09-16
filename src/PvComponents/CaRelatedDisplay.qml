import QtQuick 2.1
import QtQuick.Controls 1.0

import PvComponents 1.0
import "utils.js" as UtilsJS

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

     When the parent display is replaced by the new display, its upper-left corner should be at the former position of the parent,
     unless the new display was already in existence.  In that case the existing display is popped up, and its position is not changed.
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
    property int visual: RelatedDisplayVisual.Menu

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
            window.fileName = fname
        }
        else if (fname.substr(-4) == '.qml') {
            var component = Qt.createComponent(fname)
            if (component.status == Component.Ready) {
                window = component.createObject()
            }
            else if (component.status == Component.Error)
                console.error(component.errorString())
            else
                component.onStatusChanged = function done(status) {
                    if (status == Component.Ready) {
                        window = component.createObject()
                    } else
                        console.error(component.errorString())
                }
        }
        if (window) {
            if (remove) {
                window.x = baseWindow.x
                window.y = baseWindow.y
                baseWindow.close()
            }
            window.title = fname
            window.visible = true
        }
    }

    Component.onCompleted: {
        var label, fname, args, remove
        var qmlCmd
        var imp ='import QtQuick 2.1; import QtQuick.Controls 1.0; import PvComponents 1.0;'

        // If button label starts with '-', remove it and use normal button. Otherwise use RDButton with double rect icon
        var btnLabel, btnType
        if (display.label.charAt(0) == '-') {
            btnLabel = display.label.substring(1)
            btnType = 'StyledButton'
        }
        else {
            btnLabel = display.label
            btnType = 'RDButton'
        }

        // set font family and size
        var font = UtilsJS.getBestFontSize(visual == RelatedDisplayVisual.Column ? display.height / model.count - 4: display.height - 4, true)
        display.fontFamily = font.family
        display.fontSize = font.size

        var btnCmdTemplate = '%1 {' +
                    'text: "%2";'+
                    'fontFamily: display.fontFamily;'+
                    'pixelSize: display.fontSize;'+
                    'background: display.background;' +
                    'foreground: display.foreground;' +
                    '%3}'

        var btn
        // If only one display file, one push button will be used independent of the visual style
        if (model.count <= 1) {
            // Compose button creation statements
            var triggerCmd = ''
            if (model.count == 1)
                triggerCmd = 'onClicked: load("%1", "%2", %3)'
                            .arg(model.get(0).fname)
                            .arg(model.get(0).args)
                            .arg(model.get(0).remove ? 'true' : 'false')

            qmlCmd = imp + btnCmdTemplate.arg(btnType).arg(btnLabel).arg(triggerCmd)
            btn = Qt.createQmlObject(qmlCmd, display, 'button')
            btn.anchors.fill = display
            btn.align = Text.AlignHCenter
            return
        }

        switch (visual) {
        case RelatedDisplayVisual.Menu: // Button with popupmenu
            // Compose menu creation statements
            var menuCmd = 'menu: Menu {\n'
            for(var i=0; i<model.count; i++) {
                label = model.get(i).label
                fname = model.get(i).fname
                args = model.get(i).args
                remove = model.get(i).remove
                menuCmd += 'MenuItem {text: "%1"; onTriggered: load("%2", "%3", %4)}\n'
                           .arg(label).arg(fname).arg(args).arg(remove ? 'true' : 'false')
            }
            menuCmd += '}'
            // Create button and pulldown menu
            qmlCmd = imp + btnCmdTemplate.arg(btnType).arg(btnLabel).arg(menuCmd)
            btn = Qt.createQmlObject(qmlCmd, display, 'button')
            btn.anchors.fill = display
            btn.align = Text.AlignLeft
            break;
        case RelatedDisplayVisual.Row: // Row of button
        case RelatedDisplayVisual.Column: // Column of buttons
            var layout
            if (visual == RelatedDisplayVisual.Row)
                layout = 'RowLayout'
            else
                layout = 'ColumnLayout'
            qmlCmd = imp + 'import QtQuick.Layouts 1.0;' + layout + ' {anchors.fill: display; spacing: 0;'
            for(var i=0; i<model.count; i++) {
                label = model.get(i).label
                fname = model.get(i).fname
                args = model.get(i).args
                remove = model.get(i).remove
                var btnCmd = 'Layout.fillWidth: true; Layout.fillHeight: true;' +
                             'onClicked: load("%1", "%2", %3)'
                             .arg(model.get(i).fname)
                             .arg(model.get(i).args)
                             .arg(model.get(i).remove ? 'true' : 'false')

                qmlCmd += btnCmdTemplate.arg('StyledButton').arg(label).arg(btnCmd)
            }
            qmlCmd += '}'
            Qt.createQmlObject(qmlCmd, display, 'layout')
            break;
        case RelatedDisplayVisual.Hidden:
            visible = false
            break;
        }
    }
}
