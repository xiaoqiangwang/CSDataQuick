import QtQuick 2.1
import QtQuick.Controls 1.0
import QtQuick.Layouts 1.0

import CSDataQuick.Components 1.0
import CSDataQuick.Components.Private 1.0
import "utils.js" as UtilsJS

/*!
    \qmltype CSRelatedDisplay
    \inqmlmodule CSDataQuick.Components
    \ingroup csdataquick.components.controls
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

    The new display can be configured by "replace" property to always replace the parent.
    When the parent display is replaced by the new display, its upper-left corner should be at the former position of the parent,
    unless the new display was already in existence.  In that case the existing display is popped up, and its position is not changed.

    \qml
    Row {
        spacing: 10
        CSRelatedDisplay {
            width: 100
            height: 25
            label: 'More ...'
            model: ListModel {
                ListElement {label:'Monitors'; file: 'monitors.adl'; replace: true;}
                ListElement {label:'Graphics'; file: 'graphics.adl'; replace: true;}
            }
        }
        CSRelatedDisplay {
            width: 100
            height: 60
            visual: RelatedDisplayVisual.Column
            model: [
                {"label":"Monitors", "file":"monitor.adl", "macro":"", "replace":true},
                {"label":"Controls", "file":"control.adl", "macro":"", "replace":true}
            ]
        }
    }
     \endqml

     \image relateddisplay.png
*/

BaseItem {
    id: root

    /*!
        This property holds the text displayed on the button.

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
        This property holds the related display list model

        The model contains entries to display files. It can be ListModel or JSON object array.
        In either case, each entry contains the following.
        \list
        \li label: text description
        \li file: display file name
        \li macro: macro substitution
        \li replace: replace the parent display if true
        \endlist

        \e{Note: Only in the form of JSON object array, it can be used in Qt Quick Designer}
    */
    property var model

    /*! \internal */
    readonly property var font: UtilsJS.getBestFontSize(visual == RelatedDisplayVisual.Column ? root.height / model.count - 4: root.height - 4, true)

    implicitWidth: visual == RelatedDisplayVisual.Row ? displayModel.count * 50 : 50
    implicitHeight: visual == RelatedDisplayVisual.Column ? displayModel.count * 20 : 20
    background: ColorMap.controls_background
    foreground: ColorMap.foreground

    Loader {
        anchors.fill: parent
        sourceComponent: visual == RelatedDisplayVisual.Menu || displayModel.count == 1 ? menuStyle : buttonStyle
    }

    Component {
        id: buttonStyle
        GridLayout {
            rowSpacing: 0
            columnSpacing: 0
            rows: visual == RelatedDisplayVisual.Column ? displayModel.count : 1
            columns: visual == RelatedDisplayVisual.Row ? displayModel.count : 1
            Repeater {
                model: displayModel.count
                StyledButton {
                    text: displayModel.get(index).label
                    font.family: root.font.family
                    font.pixelSize: root.font.size
                    backgroundColor: root.background
                    foregroundColor: root.foreground
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    onClicked: load(displayModel.get(index).file,
                                    displayModel.get(index).macro,
                                    displayModel.get(index).replace)
                }
            }
        }
    }

    Component {
        id: menuStyle
        RDButton {
            text: root.label.replace(/^-/, '')
            font.family: root.font.family
            font.pixelSize: root.font.size
            background: root.background
            foreground: root.foreground
            showIcon: root.label.charAt(0) != '-'
            align: displayModel.count <= 1 ? Text.AlignHCenter : Text.AlignLeft
            menu: displayModel.count > 1 ? popupMenu : null
            Menu {
                id: popupMenu
                Instantiator {
                    model: displayModel
                    delegate: MenuItem {
                        text: model.label
                        onTriggered: load(model.file, model.macro, model.replace)
                    }
                    onObjectAdded: popupMenu.insertItem(index, object)
                    onObjectRemoved: popupMenu.removeItem(object)
                }
            }
            onClicked: {
                if (displayModel.count == 1) {
                    load(displayModel.get(0).file,
                         displayModel.get(0).macro,
                         displayModel.get(0).replace)
                }
            }
        }
    }

    ListModel {
        id: displayModel
    }

    onModelChanged: generateModel()
    /*! \internal */
    function generateModel() {
        if (model instanceof Array) {
            displayModel.clear()
            for(var i=0; i<model.length; i++)
                displayModel.append(model[i])
        } else {
            displayModel.clear()
            for(var i=0; i<model.count; i++)
                displayModel.append(model.get(i))
        }
    }

    /*!
        \internal
    */
    function load(fileName, macro, replace) {
        if (!fileName)
            return
        // search the file
        var absFilePath = Utils.searchDisplayFile(fileName, baseWindow)
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
            window = Utils.createDisplay(qmlCmd, root, absFilePath, macro)
        }
        if (fileName.substr(-4) == '.qml') {
            window = Utils.createDisplayByFile(root, absFilePath, macro)
        }
        if (!window) {
            console.error("Failed to create window from ", fileName)
            return
        } else {
            if (replace) {
                window.x = baseWindow.x
                window.y = baseWindow.y
                baseWindow.close()
            }
            console.info('Open ', absFilePath, macro)
            WindowManager.appendWindow(window)
            window.visible = true
        }
    }
}