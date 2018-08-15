/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of Qt Creator.
**
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 as published by the Free Software
** Foundation with exceptions as appearing in the file LICENSE.GPL3-EXCEPT
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
****************************************************************************/

import QtQuick 2.1
import HelperWidgets 2.0
import QtQuick.Layouts 1.0
import QtQuick.Controls 1.0 as Controls
import QtQuickDesignerTheme 1.0

Section {
    id: fontSection
    anchors.left: parent.left
    anchors.right: parent.right
    caption: qsTr("Font")

    property bool showStyle: false

    property variant fontFamily: backendValues.font_family
    property variant pixelSize: backendValues.font_pixelSize

    property variant boldStyle: backendValues.font_bold
    property variant italicStyle: backendValues.font_italic
    property variant underlineStyle: backendValues.font_underline
    property variant strikeoutStyle: backendValues.font_strikeout


    SectionLayout {
        columns: 2
        Label {
            text: qsTr("Font")
        }
        FontComboBox {
            backendValue: fontSection.fontFamily
            Layout.fillWidth: true
        }

        Label {
            text: qsTr("Size")
        }

        SecondColumnLayout {
            SpinBox {
                minimumValue: 0
                maximumValue: 400
                backendValue: backendValues.font_pixelSize
                enabled: sizeModeWidget.currentIndex == 1
            }
            Label {
                text: qsTr("pixel")
            }
        }

        Label {
            text: qsTr("Font size mode")
        }

        ComboBox {
            id: sizeModeWidget
            Layout.fillWidth: true
            backendValue: backendValues.fontSizeMode
            model:  ["Fit", "FixedSize", "HorizontalFit", "VerticalFit"]
            scope: "Text"
            onCurrentIndexChanged: {
                if (currentIndex == 1)
                    pixelSize.expression = '12'
                else
                    pixelSize.resetValue() 
            }
        }

        Label {
            text: qsTr("Font style")
        }
        FontStyleButtons {

        }

        Label {
            text: qsTr("Font capitalization")
            toolTip: qsTr("Sets the capitalization for the text.")
        }

        ComboBox {
            Layout.fillWidth: true
            backendValue: backendValues.font_capitalization
            model:  ["MixedCase", "AllUppercase", "AllLowercase", "SmallCaps", "Capitalize"]
            scope: "Font"
        }

        Label {
            text: qsTr("Font weight")
            toolTip: qsTr("Sets the font's weight.")
        }

        ComboBox {
            Layout.fillWidth: true
            backendValue: backendValues.font_weight
            model:  ["Normal", "Light", "ExtraLight", "Thin", "Medium", "DemiBold", "Bold", "ExtraBold", "Black"]
            scope: "Font"
        }

        Label {
            visible: showStyle
            text: qsTr("Style")
        }

        ComboBox {
            visible: showStyle
            Layout.fillWidth: true
            backendValue: (backendValues.style === undefined) ? dummyBackendValue : backendValues.style
            model:  ["Normal", "Outline", "Raised", "Sunken"]
            scope: "Text"
        }

        Label {
            text: qsTr("Spacing")
        }

        SecondColumnLayout {
            Label {
                text: qsTr("Word")
                tooltip: qsTr("Sets the word spacing for the font.")
                width: 42
            }
            SpinBox {
                maximumValue: 9999999
                minimumValue: -9999999
                decimals: 0
                backendValue: backendValues.font_wordSpacing
                Layout.fillWidth: true
            }
            Item {
                width: 4
                height: 4
            }

            Label {
                text: qsTr("Letter")
                tooltip: qsTr("Sets the letter spacing for the font.")
                width: 42
            }
            SpinBox {
                maximumValue: 9999999
                minimumValue: -9999999
                decimals: 0
                backendValue: backendValues.font_letterSpacing
                Layout.fillWidth: true
            }
        }
    }
}
