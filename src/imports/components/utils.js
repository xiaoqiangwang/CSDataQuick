.pragma library

.import CSDataQuick.Data 1.0 as Core
.import CSDataQuick.Components 1.0 as Components

/*!
    \qmltype UtilsJS
    \inqmlmodule CSDataQuick.Components
    \brief Javascript Utility Functions.
*/


/*!
    \qmlmethod UtilsJS::getBestFontSize(item_height, with_frame)
    Calculates the font size/family based on item height.

    This function uses an imperically determined (item height, font size) table.
    Actually two sets of tables are defined. One for items without enclosing frame, e.g. CSText.
    One for items with a border frame, e.g. CSTextEntry. The reason for this manual tweak is to
    match MEDM displays.

    The font family is either \e{Dejavu Sans Mono} for font size less than 20 or \e Helvetica otherwise.
*/
function getBestFontSize(item_height, with_frame) {
    var fsize_label = [[ 8,8], [ 9,9], [10,10], [13,11], [14,12], [15,13], [16,14], [20,16], [21,18], [26,22], [27, 28],[35,32]]
    var fsize_frame = [[14,8], [15,9], [16,10], [20,11], [21,12], [22,13], [23,14], [27,16], [32,18], [34,22]]

    var fsize = fsize_label
    if (with_frame)
        fsize = fsize_frame

    var font_size = fsize[fsize.length - 1][1]
    for(var i=1; i<fsize.length; i++) {
        if(item_height < fsize[i][0]) {
            font_size = fsize[i - 1][1]
            break
        }
    }

    var font_family = "Dejavu Sans Mono"
    if (font_size > 20)
        font_family = "Helvetica"

    return {"family" : font_family, "size" : font_size}
}

/*!
    \qmlmethod UtilsJS::popupDataLimitsDialog(parent)

    This function popups up a dialog for user to configure the limits settings.
*/
function popupDataLimitsDialog(parent) {
    var cmd = 'import CSDataQuick.Components 1.0; DataLimitsDialog {}'
    var dialog = Qt.createQmlObject(cmd, parent, 'limitsDialog')

    dialog.channel = parent.source

    dialog.limits.loprSrc     = parent.limits.loprSrc
    dialog.limits.loprChannel = parent.limits.loprChannel
    dialog.limits.loprDefault = parent.limits.loprDefault
    dialog.limits.loprUser    = parent.limits.loprUser

    dialog.limits.hoprSrc     = parent.limits.hoprSrc
    dialog.limits.hoprChannel = parent.limits.hoprChannel
    dialog.limits.hoprDefault = parent.limits.hoprDefault
    dialog.limits.hoprUser    = parent.limits.hoprUser

    dialog.limits.precSrc     = parent.limits.precSrc
    dialog.limits.precChannel = parent.limits.precChannel
    dialog.limits.precDefault = parent.limits.precDefault
    dialog.limits.precUser    = parent.limits.precUser

    dialog.accepted.connect(function() {
        // only user values can be modified
        // so copy them only
        parent.limits.loprSrc  = dialog.limits.loprSrc
        parent.limits.loprUser = dialog.limits.loprUser

        parent.limits.hoprSrc  = dialog.limits.hoprSrc
        parent.limits.hoprUser = dialog.limits.hoprUser

        parent.limits.precSrc  = dialog.limits.precSrc
        parent.limits.precUser = dialog.limits.precUser

        dialog.destroy()
    })
    dialog.rejected.connect(function() {
        dialog.destroy()
    })

    dialog.open()
}

/*!
    \qmlmethod UtilsJS::popupPromptDialog(parent, title, hint, input)

    This function popups a dialog to obtains user's input.
*/
function popupPromptDialog(parent, title, hint, input) {
    var cmd = 'import CSDataQuick.Components 1.0; PromptDialog {}'
    var dialog = Qt.createQmlObject(cmd, parent, 'promptDialog')
    dialog.title = title
    dialog.hint = hint
    dialog.input = input
    dialog.open()
    return dialog
}

/*!
    \qmlmethod UtilsJS::dumpCSData(data)

    This function returns a printable string representation of the \a data object.
*/
function dumpCSData(data) {
    var date = new Date()
    var text
    text = '           Data Infomation\n\n'
    text += date + '\n\n'
    text += data.source + '\n'
    text += '===================================\n'
    if (!data.connected) {
        text += 'diconnected'
        return text
    }

    text += 'TYPE: %1\n'.arg(data.fieldType)
    text += 'COUNT: %1\n'.arg(data.count)
    text += 'ACCESS: '
    if (data.accessRight & Core.CSData.ReadAccess)
        text += 'R'
    if (data.accessRight & Core.CSData.WriteAccess)
        text += 'W'
    text += '\n'
    text += 'HOST: %1\n'.arg(data.host)
    text += 'VALUE: %1\n'.arg(data.value)
    text += 'STAMP: %1\n'.arg(data.timeStamp)
    // Data type specific information
    text += '\n'
    if (data.fieldType == Core.CSData.Float || data.fieldType == Core.CSData.Double)
        text += 'PRECISION: %1\n'.arg(data.precision)
    if (data.range.lower != data.range.upper)
        text += 'HOPR: %1  LOPR %2\n'.arg(data.range.upper).arg(data.range.lower)

    if (data.stateStrings.length)
        text += 'STATES: %1\n'.arg(data.stateStrings.length)
    for(var i=0; i<data.stateStrings.length; i++) {
        text += 'STATE %1: %2\n'.arg(i).arg(data.stateStrings[i])
    }
    return text
}

/*!
    \qmlmethod UtilsJS::formatString(value, format, type, precision, states)
    Format the value based on data type.

    \table
    \header
    \li type
    \li output
    \row
    \li Enum
    \li string at the \a value index from the \a states list.
    \row
    \li String
    \li as it is.
    \row
    \li Char
    \li string by joinning the char array with ending null character removed.
    \row
    \li Number
    \li string determined by \a format and \a precision.
    \endtable

     \sa Utils::convert,
*/
function formatString(value, format, type, precision, states) {
    if (value === undefined) {
        if (format == Components.TextFormat.String)
            return ""
        else
            return Components.Utils.convert(format, 0, precision)
    }
    if (type == Core.CSData.Enum)
        return states[value]
    if (type == Core.CSData.String)
        return value
    if (type == Core.CSData.Char && format == Components.TextFormat.String) {
        if (value instanceof Array)
            return String.fromCharCode.apply(null, value).replace(/\0/g, '')
        else {
            if (Components.Utils.qtVersion() < 0x050700) {
                var varlist = []
                Components.Utils.vectorGet(value, varlist);
                return String.fromCharCode.apply(null, varlist).replace(/\0/g, '')
            } else
                return String.fromCharCode(value).replace(/\0/g, '')
        }
    }
    if (value instanceof Array)
        value = value[0]
    var result = Components.Utils.convert(format, value, precision)
    return result
}
