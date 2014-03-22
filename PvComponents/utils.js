.pragma library

function getBestFontSize(h) {
    var height = [8, 9, 10, 13, 14, 15, 16, 20, 22, 24, 26, 30];
    var size =   [8, 9, 10, 13, 14, 15, 16, 20, 22, 22, 22, 34];
    var font = 34
    for(var i=1; i<height.length; i++) {
        if(h < height[i]) {
            font = size[i - 1]
            break
        }
    }
    if(!font)
        font == 34
    if (font == 13 || font == 14)
        font -= 2
    else if (font == 16 || font == 20)
        font -= 3
    return font
}
