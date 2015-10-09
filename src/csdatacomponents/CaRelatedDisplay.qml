import QtQuick 2.1
import QtQuick.Controls 1.0

import CSDataQuick.Components 1.0
import "utils.js" as UtilsJS

/*!
    \qmltype CaRelatedDisplay
    \inqmlmodule CSDataQuick.Components
    \brief Brings up more displays

    The Related Display provides a means of bringing up more displays.
    It is by default a menu button that has a graphic consisting of two overlapping squares and a label.
    But it can also be a group of buttons layout in a row or column. This visual reprsentation is set by property \l visual.

    The \l model denote the related displays that can be displayed.
    If there is only one related display specified in the menu,
    then the two overlapping squares and/or label appear in the middle of the button,
    and the related display is activated immediately without bringing up a menu.

    It is not activated until the button is released, so you can depress the button to check
    if there is more than one menu item or not, then abort by releasing the button away from the Related Display.
    If there is more than one item on the menu, the squares and/or label are at the left of the button.

    The new display can be configured by "remove" property to always replace the parent.
     When the parent display is replaced by the new display, its upper-left corner should be at the former position of the parent,
     unless the new display was already in existence.  In that case the existing display is popped up, and its position is not changed.

     \qml
    Row {
        spacing: 10
        CaRelatedDisplay {
            width: 100
            height: 25
            label: 'More ...'
            model: ListModel {
                ListElement {label:'Monitors'; fname: 'monitors.adl'; remove: true;}
                ListElement {label:'Graphics'; fname: 'graphics.adl'; remove: true;}
            }
        }
        CaRelatedDisplay {
            width: 100
            height: 60
            visual: RelatedDisplayVisual.Column
            model: ListModel {
                ListElement {label:'Monitors'; fname: 'monitors.adl'; remove: true;}
                ListElement {label:'Graphics'; fname: 'graphics.adl'; remove: true;}
            }
        }
    }
     \endqml

     \image relateddisplay.png
*/

BaseItem {
    id: root
    /*! foreground color */
    property color foreground: ColorMap.color14
    /*! background color */
    property color background: ColorMap.color51 // color4 is the actual default
    /*!
        text displayed on button

        Normally there are two overlapping squares on the button to denote a Related Display control.
        However, if the label text begins with  "-" (for example "-My Label"), the overlapping squares will not appear.
    */
    property string label: ''
    /*!
        \qmlproperty enumeration visual
        \list
            \li RelatedDisplayVisual.Menu: Use a pull down menu for the choices.
            \li RelatedDisplayVisual.Row: Use a row of buttons for the choices.
            \li RelatedDisplayVisual.Column: Use a column of buttons for the choices.
        \endlist
    */
    property int visual: RelatedDisplayVisual.Menu

    /*!
        related display list model

        The model contains entries to display files. Each entry contains the following.
        \list
        \li label: text description
        \li fname: display file name
        \li args: macros substitution
        \li remove: replace the parent display if true
        \endlist
    */
    property ListModel model: ListModel {
    }

    readonly property var font: UtilsJS.getBestFontSize(visual == RelatedDisplayVisual.Column ? root.height / model.count - 4: root.height - 4, true)

    /*!
        \internal
    */
    function load(fileName, macro, remove) {
        // search the file
        var absFilePath = Utils.searchADLFile(fileName, baseWindow)
        if (absFilePath == '') {
            console.error("Failed to find file ", fileName)
            return
        }

        // if the window exists, bring it up
        var window = WindowManager.findWindow(absFilePath, macro)
        if (window) {
            window.raise()
            window.requestActivate()
            console.info('Activate the already existing display ', absFilePath, macro)
            return
        }
        // open new display
        if (fileName.substr(-4) == '.adl') {
            var qmlCmd = Utils.openADLDisplay(absFilePath, macro)
            window = Utils.createDisplay(qmlCmd, root, absFilePath)
            window.macro = macro
        }
        if (fileName.substr(-4) == '.qml') {
            window = Utils.createDisplayByFile(root, absFilePath, macro)
        }
        if (!window) {
            console.error("Failed to create window from ", fileName)
            return
        } else {
            if (remove) {
                window.x = baseWindow.x
                window.y = baseWindow.y
                baseWindow.close()
            }
            console.info('Open ', absFilePath, macro)
            WindowManager.appendWindow(window, absFilePath, macro)
            window.visible = true
        }
    }

    Component.onCompleted: {
        var label, fname, args, remove
        var qmlCmd
        var imp ='import QtQuick 2.1; import QtQuick.Controls 1.0; import CSDataQuick.Components.Private 1.0;'

        // If button label starts with '-', remove it and use normal button. Otherwise use RDButton with double rect icon
        var btnLabel, btnType
        if (root.label.charAt(0) == '-') {
            btnLabel = root.label.substring(1)
            btnType = 'StyledButton'
        }
        else {
            btnLabel = root.label
            btnType = 'RDButton'
        }

        // set font family and size
        var btnCmdTemplate = '%1 {' +
                    'text: "%2";'+
                    'font.family: root.font.family;'+
                    'font.pixelSize: root.font.size;'+
                    'background: root.background;' +
                    'foreground: root.foreground;' +
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
            btn = Qt.createQmlObject(qmlCmd, root, 'button')
            btn.anchors.fill = root
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
            btn = Qt.createQmlObject(qmlCmd, root, 'button')
            btn.anchors.fill = root
            btn.align = Text.AlignLeft
            break;
        case RelatedDisplayVisual.Row: // Row of button
        case RelatedDisplayVisual.Column: // Column of buttons
            var layout
            if (visual == RelatedDisplayVisual.Row)
                layout = 'RowLayout'
            else
                layout = 'ColumnLayout'
            qmlCmd = imp + 'import QtQuick.Layouts 1.0;' + layout + ' {anchors.fill: root; spacing: 0;'
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
            Qt.createQmlObject(qmlCmd, root, 'layout')
            break;
        case RelatedDisplayVisual.Hidden:
            visible = false
            break;
        }
    }
}