.pragma library

.import PvComponents 1.0 as PvComponents

/*!
    \qmltype UtilsJS
    \inqmlmodule PvComponents
    \brief Javascript Utility Functions
*/


/*!
    \qmlmethod UtilsJS::getBestFontSize(widget_height, with_frame)
*/
function getBestFontSize(widget_height, with_frame) {
    var fsize_label = [[ 8,8], [ 9,9], [10,10], [13,11], [14,12], [15,13], [16,14], [20,16], [21,18], [26,22], [27, 28],[35,32]]
    var fsize_frame = [[14,8], [15,9], [16,10], [20,11], [21,12], [22,13], [23,14], [27,16], [32,18], [34,22]]

    var fsize = fsize_label
    if (with_frame)
        fsize = fsize_frame

    var font_size = fsize[fsize.length - 1][1]
    for(var i=1; i<fsize.length; i++) {
        if(widget_height < fsize[i][0]) {
            font_size = fsize[i - 1][1]
            break
        }
    }

    var font_family = "Dejavu Sans Mono"
    if (font_size > 20)
        font_family = "Helvetica"

    return {"family" : font_family, "size" : font_size}
}
