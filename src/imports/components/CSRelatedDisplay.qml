import QtQml 2.2
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
    \brief Brings up more displays.

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
        \qmlproperty font font
        The text font.
    */
    property alias font: hiddenText.font
    /*!
        \qmlproperty enumeration fontSizeMode
        This property specifies how the font size of the displayed text is determined.

        \sa {Text::} {fontSizeMode}
    */
    property int fontSizeMode: Text.Fit
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

    font.family: UtilsJS.getBestFontSize(visual == RelatedDisplayVisual.Column ? root.height / model.count - 4: root.height - 4, true).family

    implicitWidth: loader.implicitWidth 
    implicitHeight: loader.implicitHeight
    background: ColorMap.controls_background
    foreground: ColorMap.foreground

    Loader {
        id: loader
        anchors.fill: parent
        sourceComponent: visual == RelatedDisplayVisual.Menu || displayModel.count == 1 ? menuStyle : buttonStyle
    }

    Text {
        id: hiddenText
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
                    font: root.font
                    fontSizeMode: root.fontSizeMode
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
        id: doubleRect
        Image {
            source: 'image://doublerect/' + foreground
            fillMode: Image.PreserveAspectFit
            sourceSize.width: width
            sourceSize.height: height
        }
    }

    Component {
        id: menuStyle
        StyledButton {
            text: root.label.replace(/^-/, '')
            font: root.font
            fontSizeMode: root.fontSizeMode
            backgroundColor: root.background
            foregroundColor: root.foreground
            icon: root.label.charAt(0) != '-' ? doubleRect : null
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
        var i
        if (model instanceof Array) {
            displayModel.clear()
            for(i=0; i<model.length; i++)
                displayModel.append(model[i])
        } else {
            displayModel.clear()
            for(i=0; i<model.count; i++)
                displayModel.append(model.get(i))
        }
    }

    /*!
        \internal
    */
    function load(fileName, macro, replace) {
        if (!fileName)
            return

        // if not a display file, try as url
        if (!Utils.isDisplayFile(fileName)) {
            if (!Utils.openUrl(fileName))
                console.error("Failed to open file ", fileName)
            return
        }

        // search the file
        var absFilePath = Utils.searchDisplayFile(fileName, windowPath)
        if (absFilePath.toString() === '') {
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
        window = Utils.createDisplayByFile(root, absFilePath, macro)
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
