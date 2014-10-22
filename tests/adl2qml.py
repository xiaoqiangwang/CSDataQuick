#!/usr/bin/env python
# -*- coding: utf-8 -*-
"""
A module to parse adl display file created by MEDM.


"""


import re
import sys
from collections import OrderedDict

"""
Conversion Table from MEDM literal to QML enum
"""
TextFormat = {
    '"decimal"' : 'TextFormat.Decimal',
    '"exponential"' : 'TextFormat.Exponential',
    '"eng. notation"' : 'TextFormat.EngNotation',
    '"compact"' : 'TextFormat.Compact',
    '"truncated"' : 'TextFormat.Truncated',
    '"hexadecimal"' : 'TextFormat.Hexadecimal',
    '"octal"' : 'TextFormat.Octal',
    '"string"' : 'TextFormat.String',
    '"sexagesimal"' : 'TextFormat.Sexagesiaml',
    '"sexagesiamlDMS"' : 'TextFormat.SexagesimalDMS',
    '"sexagesimalHMS"' : 'TextFormat.SexagesimalHMS',

}

TextAlign = {
    '"horiz. left"' : 'Text.AlignLeft',
    '"horiz. centered"' : 'Text.AlignHCenter',
    '"horiz. right"' : 'Text.AlignRight',
    '"vert. left"' : 'Text.AlignLeft',
    '"vert. centered"' : 'Text.AlignHCenter',
    '"vert. right"' : 'Text.AlignRight',
}

ColorMode = {
    '"static"' : 'ColorMode.Static',
    '"alarm"'  : 'ColorMode.Alarm',
    '"discrete"': 'ColorMode.Static',
}

VisMode = {
    '"static"' : 'VisibilityMode.Static',
    '"calc"' : 'VisibilityMode.Calc',
    '"if not zero"' : 'VisibilityMode.IfNotZero',
    '"if zero"' : 'VisibilityMode.IfZero',
}

FillStyle = {
    '"solid"' : 'FillStyle.Solid',
    '"outline"' : 'FillStyle.Outline',
}

EdgeStyle = {
    '"solid"' : 'EdgeStyle.Solid',
    '"dash"'  : 'EdgeStyle.Dash',
}

Visual = {
    '"menu"' : 0,
    '"a row of buttons"' : 1,
    '"a column of buttons"' : 2,
    '"invisible"' : 3,
}

Direction = {
    '"right"': '0',
    '"up"' : '1',
    '"left"' : '2',
    '"down"' : '3',
}

Stacking = {
    '"row"' : 1,
    '"column"': 0
}

LimitsSource = {
    '"channel"' : 'LimitsSource.Channel',
    '"default"' : 'LimitsSource.Default',
    '"user"' : 'LimitsSource.User'
}

TimeUnit = {
    '"milli second"' : 'TimeUnit.MilliSecond',
    '"milli-second"' : 'TimeUnit.MilliSecond',
    '"second"' : 'TimeUnit.Second',
    '"minute"' : 'TimeUnit.Minute',
}

def calcBestFont(height, framed):
    fsize = [(8,8), (9,9), (10,10), (13,13), (14,14), (15,15), (16,16), (20,20), (22,22), (24,22),
             (26,22), (30,34)]
    fsize_label = [( 8,8), ( 9,9), (10,10), (13,11), (14,12), (15,13), (16,14), (20,16), (21,18), (26,22), (27, 28), (35,32)]
    fsize_frame = [(14,8), (15,9), (16,10), (20,11), (21,12), (22,13), (23,14), (27,16), (32,18), (34,22),
              (44,30)]
    if framed:
        fsize = fsize_frame
    else:
        fsize = fsize_label
    font = fsize[-1][1]
    for i,s in enumerate(fsize):
        if height < s[0]:
            font = fsize[i - 1][1]
            break
    if height <= 20:
        family = '"Dejavu Sans Mono"'
    else:
        family = '"Helvetica"'

    return font, family


class MEDMLimits(object):
    def __init__(self, d):
        self.loprSrc = d.get('loprSrc', '"channel"')
        self.loprDefault = d.get('loprDefault', 0)
        self.hoprSrc = d.get('hoprSrc', '"channel"')
        self.hoprDefault = d.get('hoprDefault', 1)
        self.precSrc = d.get('hoprSrc', '"channel"')
        self.precDefault = d.get('hoprDefault', 0)

    def toQML(self):
        s = """
Limits {
    loprSrc: %s
    loprDefault: %s
    hoprSrc: %s
    hoprDefault: %s
    precSrc: %s
    precDefault: %s
}
""" % (LimitsSource[self.loprSrc], self.loprDefault,
       LimitsSource[self.hoprSrc], self.hoprDefault,
       LimitsSource[self.precSrc], self.precDefault)

        return s

class MEDMObject(object):
    """
    Base class of MEDM objects
    """
    def __init__(self, d, parent=None):
        self.parent = parent
        self.children = []

        self.x = int(d['object']['x'])
        self.y = int(d['object']['y'])
        self.width = max(1, int(d['object']['width']))
        self.height = max(1, int(d['object']['height']))

    def toQML(self):
        x, y = self.x, self.y
        if self.parent and not isinstance(self.parent, MEDMDisplay):
            x = x - self.parent.x
            y = y - self.parent.y

        s = """
    x: %s
    y: %s
    width: %s
    height: %s
""" % (x, y, self.width, self.height)
        return s


class MEDMDisplay(MEDMObject):
    """
    MEDM top level display
    """
    def __init__(self, d, parent=None):
        self.parent = parent
        self.children = []
        self.x = d['display']['object']['x']
        self.y = d['display']['object']['y']
        self.width = d['display']['object']['width']
        self.height = d['display']['object']['height']
        self.foreground = "#%s" % color_map[int(d['display']['clr'])]
        self.background = "#%s" % color_map[int(d['display']['bclr'])]

        for o in d['objects']:
            child = convertObject(o, self)
            self.children.append(child)

    def toQML(self):
        s = ''
        for c in self.children:
            qml = c.toQML()
            if '{' not in qml:
                qml = 'Item {\n' + qml + '}\n'
            s += qml

        return """import QtQuick 2.0
import QtQuick.Controls 1.0
import QtQuick.Window 2.0

import PvComponents 1.0

Rectangle {
    visible: true

    //x: %s
    //y: %s
    width: %s
    height: %s
    color: "%s"
    %s

}
""" % (self.x, self.y, self.width, self.height, self.background, s)


#
#  Controls
#
class MEDMControl(MEDMObject):
    """
    Control type
    """
    def __init__(self, d, parent=None):
        super(MEDMControl, self).__init__(d, parent)
        self.channel = d['control'].get('chan', '""')
        self.foreground = '"#%s"' % color_map[int(d['control']['clr'])]
        self.background = '"#%s"' % color_map[int(d['control']['bclr'])]

    def toQML(self):
        s = """
    z: 1
    foreground: %s
    background: %s
    channel: %s
""" % (self.foreground, self.background, self.channel)

        return super(MEDMControl, self).toQML() + s

class MEDMTextEntry(MEDMControl):
    def __init__(self, d, parent=None):
        super(MEDMTextEntry, self).__init__(d, parent)
        self.align =  '%s' % TextAlign[d.get('align', '"horiz. left"')]
        self.color_mode =  d.get('clrmod', '"static"')
        self.prec_src = d.get('limits', {}).get('precSrc', '"channel"')
        self.prec_default = d.get('limits', {}).get('precDefault', 0)
        self.format = d.get('format', 'decimal')

    def toQML(self):
        size, family = calcBestFont(self.height, True )

        s = """CaTextEntry {
     %s
    align: %s
    fontSize: %s
    fontFamily: %s
    colorMode: %s
    limits.precSrc: %s
    limits.precDefault: %s
}
""" % (super(MEDMTextEntry, self).toQML(), self.align, size, family, ColorMode[self.color_mode], LimitsSource[self.prec_src], self.prec_default)
        return s

class MEDMButton(MEDMControl):
    def __init__(self, d, parent=None):
        super(MEDMButton, self).__init__(d, parent)
        self.text = d.get('label', '""')
        self.on = d.get('press_msg', '""')
        self.off = d.get('release_msg', '""')

    def toQML(self):
        size, family = calcBestFont(self.height, True)
        s = """CaMessageButton {
    %s
    text: %s
    fontSize: %s
    fontFamily: %s
    onMessage: %s
    offMessage: %s
}
""" % (super(MEDMButton, self).toQML(), self.text, size, family, self.on, self.off)
        return s


class MEDMChoiceButton(MEDMControl):
    def __init__(self, d, parent=None):
        super(MEDMChoiceButton, self).__init__(d, parent)
        self.stacking = Stacking[d.get('stacking', '"row"')]

    def toQML(self):
        s = """CaChoiceButton {
        %s
        orientation: %s
}
""" % (super(MEDMChoiceButton, self).toQML(), self.stacking)
        return s

class MEDMMenu(MEDMControl):
    def __init__(self, d, parent=None):
        super(MEDMMenu, self).__init__(d, parent)
        self.channel = d['control']['chan']
        self.color = '"#%s"' % color_map[int(d['control']['clr'])]
        self.background = '"#%s"' % color_map[int(d['control']['bclr'])]

    def toQML(self):
        s = """CaMenu {
    %s
}
""" % (super(MEDMMenu, self).toQML())
        return s


class MEDMSlider(MEDMControl):
    def __init__(self, d, parent=None):
        super(MEDMSlider, self).__init__(d, parent)
        self.direction = Direction[d.get('direction', '"right"')]

    def toQML(self):
        s = """CaSlider {
    %s
    direction: %s
}
""" % (super(MEDMSlider, self).toQML(), self.direction)
        return s

class MEDMWheelSwitch(MEDMControl):
    def __init__(self, d, parent=None):
        super(MEDMWheelSwitch, self).__init__(d, parent)
        self.format = d.get('format', '""')

    def toQML(self):
        s = """CaWheelSwitch {
    %s
    format: %s
}
""" % (super(MEDMWheelSwitch, self).toQML(), self.format)
        return s

class MEDMRelatedDisplay(MEDMObject):
    def __init__(self, d, parent=None):
        super(MEDMRelatedDisplay, self).__init__(d, parent)
        self.foreground = '"#%s"' % color_map[int(d['clr'])]
        self.background = '"#%s"' % color_map[int(d['bclr'])]
        self.label = d.get('label', '""')
        self.visual = Visual[d.get('visual', '"menu"')]
        self.displays = []
        for i in range(16):
            name = 'display[%d]' % i
            if name not in d:
                continue
            label = d[name].get('label', '""')
            fname = d[name].get('name', '""')
            args = d[name].get('args', '""')
            if (label == '""' or fname == '""'):
                continue
            if d[name].get('policy') == '"replace display"':
                remove = True
            else:
                remove = False
            self.displays.append((label, fname, args, remove))

    def toQML(self):
        d = 'ListModel {'
        for label,fname,args,remove in self.displays:
            d += """
    ListElement {
        label: %s
        fname: %s
        args: %s
        remove: %s
    }
""" % (label, fname, args, str(remove).lower())
        d += '}'

        size, family = calcBestFont(self.height, True)

        s = """CaRelatedDisplay {
        %s
        foreground: %s
        background: %s
        fontSize: %s
        fontFamily: %s
        label: %s
        visual: %s
        model: %s
}
""" % (super(MEDMRelatedDisplay, self).toQML(), self.foreground, self.background, size, family, self.label, self.visual, d)
        return s

class MEDMShellCommand(MEDMObject):
    def __init__(self, d, parent=None):
        super(MEDMShellCommand, self).__init__(d, parent)
        self.foreground = '"#%s"' % color_map[int(d['clr'])]
        self.background = '"#%s"' % color_map[int(d['bclr'])]
        self.label = d.get('label', '""')
        self.commands = []
        for i in range(16):
            name = 'command[%d]' % i
            if name not in d:
                continue
            label = d[name].get('label', '""')
            command = d[name].get('name', '""')
            args = d[name].get('args', '""')
            if (label == '""' or command == '""'):
                continue
            self.commands.append((label, command, args))

    def toQML(self):
        d = 'ListModel {'
        for label,command,args in self.commands:
            d += """
    ListElement {
        label: %s
        command: %s
        args: %s
    }
""" % (label, command, args)
        d += '}'

        size, family = calcBestFont(self.height, True)

        s = """CaShellCommand {
        %s
        foreground: %s
        background: %s
        fontSize: %s
        fontFamily: %s
        label: %s
        model: %s
}
""" % (super(MEDMShellCommand, self).toQML(), self.foreground, self.background, size, family, self.label, d)
        return s
#
#  Monitors
#
class MEDMMonitor(MEDMObject):
    """
    Monitor type
    """
    def __init__(self, d, parent=None):
        super(MEDMMonitor, self).__init__(d, parent)
        self.foreground = '"#%s"' % color_map[int(d['monitor']['clr'])]
        self.background = '"#%s"' % color_map[int(d['monitor']['bclr'])]
        self.channel =  d['monitor'].get('chan', '""')

    def toQML(self):
        s = """
    channel: %s
    foreground: %s
    background: %s
""" % (self.channel, self.foreground, self.background)

        return super(MEDMMonitor, self).toQML() + s


class MEDMTextUpdate(MEDMMonitor):
    def __init__(self, d, parent=None):
        super(MEDMTextUpdate, self).__init__(d, parent)
        self.align =  '%s' % TextAlign[d.get('align', '"horiz. left"')]
        self.color_mode = d.get('clrmod', '"static"')
        self.prec_src = d.get('limits', {}).get('precSrc', '"channel"')
        self.prec_default = d.get('limits', {}).get('precDefault', 0)
        self.format = d.get('format', '"decimal"')

    def toQML(self):
        size, family = calcBestFont(self.height, False)
        s = """CaTextUpdate {
    %s
    align: %s
    format: %s
    colorMode: %s
    fontSize: %s
    fontFamily: %s
    limits.precSrc: %s
    limits.precDefault: %s
}
""" % (super(MEDMTextUpdate, self).toQML(), self.align, TextFormat[self.format], ColorMode[self.color_mode], size, family, LimitsSource[self.prec_src], self.prec_default)
        return s

class MEDMBar(MEDMMonitor):
    def __init__(self, d, parent=None):
        super(MEDMBar, self).__init__(d, parent)
        self.direction = Direction[d.get('direction', '"right"')]

    def toQML(self):
        s = """CaBar {
    %s
    direction: %s
}
""" % (super(MEDMBar, self).toQML(), self.direction)
        return s

class MEDMMeter(MEDMMonitor):
    def __init__(self, d, parent=None):
        super(MEDMMeter, self).__init__(d, parent)

    def toQML(self):
        s = """CaMeter {
    %s
}
""" % (super(MEDMMeter, self).toQML())
        return s

class MEDMByte(MEDMMonitor):
    def __init__(self, d, parent=None):
        super(MEDMByte, self).__init__(d, parent)
        self.direction = d.get('direction', '"right"')
        self.start = d.get('sbit', 15)
        self.end = d.get('ebit', 0)

    def toQML(self):
        d = {'"right"' : 'Qt.Horizontal',
             '"down"'  : 'Qt.Vertical'}

        s = """CaByte {
    %s
    orientation: %s
    start: %s
    end: %s
}
""" % (super(MEDMByte, self).toQML(), d[self.direction], self.start, self.end)
        return s

#
#  Graphics
#
class MEDMGraphics(MEDMObject):
    def __init__(self, d, parent=None):
        super(MEDMGraphics, self).__init__(d, parent)
        self.foreground  =  '"#%s"' % color_map[int(d['"basic attribute"']['clr'])]
        self.fill = d['"basic attribute"'].get('fill', '"solid"')
        self.edge = d['"basic attribute"'].get('style', '"solid"')
        self.lineWidth = d['"basic attribute"'].get('width', '1')
        if self.lineWidth == '0':
            self.lineWidth = '1'

        if '"dynamic attribute"' in d:
            self.channel = d['"dynamic attribute"'].get('chan', '""')
            self.channelB = d['"dynamic attribute"'].get('chanB', '""')
            self.channelC = d['"dynamic attribute"'].get('chanC', '""')
            self.channelD = d['"dynamic attribute"'].get('chanD', '""')
            self.visMode = d['"dynamic attribute"'].get('vis', '"static"')
            self.visCalc = d['"dynamic attribute"'].get('calc', '""')
            self.colorMode = d['"dynamic attribute"'].get('clr', '"static"')

    def toQML(self):
        s = """
    %s
    foreground: %s
""" % (super(MEDMGraphics, self).toQML(), self.foreground)

        q = ''
        if hasattr(self, 'channel'):
            q = """
    colorMode: %s
    dynamicAttr.channel: %s
    dynamicAttr.channelB: %s
    dynamicAttr.channelC: %s
    dynamicAttr.channelD: %s
    dynamicAttr.visibilityMode: %s
    dynamicAttr.visibilityCalc: %s
""" % (ColorMode[self.colorMode], self.channel, self.channelB, self.channelC, self.channelD, VisMode[self.visMode], self.visCalc)

        return s + q

class MEDMText(MEDMGraphics):
    def __init__(self, d, parent=None):
        super(MEDMText, self).__init__(d, parent)
        self.text = d.get('textix', '""')
        self.align =  '%s' % TextAlign[d.get('align', '"horiz. left"')]

    def toQML(self):
        size, family = calcBestFont(self.height, False)
        s = """CaText {
    %s
    align: %s
    text: %s
    fontSize: %s
    fontFamily: %s
}
""" % (super(MEDMText, self).toQML(), self.align, self.text, size, family)
        return s

class MEDMImage(MEDMObject):
    def __init__(self, d, parent=None):
        super(MEDMImage, self).__init__(d, parent)
        self.fname = d['"image name"']

    def toQML(self):
        s = """CaImage {
    %s
    source: %s
}
""" % (super(MEDMImage, self).toQML(), self.fname)
        return s

class MEDMArc(MEDMGraphics):
    def __init__(self, d, parent=None):
        super(MEDMArc, self).__init__(d, parent)
        self.begin = int(d['begin']) / 64.
        self.span = int(d['path']) / 64.
        # count for the line width
        self.width += 1
        self.height += 1


    def toQML(self):
        s = """CaArc {
    %s
    fill: %s
    edge: %s
    lineWidth: %s
    begin: %s
    span: %s
}
""" % (super(MEDMArc, self).toQML(), FillStyle[self.fill], EdgeStyle[self.edge], self.lineWidth, self.begin, self.span)
        return s

class MEDMOval(MEDMGraphics):
    def __init__(self, d, parent=None):
        super(MEDMOval, self).__init__(d, parent)
        # count for the line width
        self.width += 1
        self.height += 1

    def toQML(self):
        s = """CaOval {
    %s
    fill: %s
    edge: %s
    lineWidth: %s
 }
""" % (super(MEDMOval, self).toQML(), FillStyle[self.fill], EdgeStyle[self.edge], self.lineWidth,)
        return s

class MEDMRect(MEDMGraphics):
    def __init__(self, d, parent=None):
        super(MEDMRect, self).__init__(d, parent)

    def toQML(self):
        s = """CaRect {
    %s
    fill: %s
    edge: %s
    lineWidth: %s
}
""" % (super(MEDMRect, self).toQML(), FillStyle[self.fill], EdgeStyle[self.edge], self.lineWidth,)
        return s


class MEDMPolyline(MEDMGraphics):
    def __init__(self, d, parent=None):
        super(MEDMPolyline, self).__init__(d, parent)
        self.points = d['points']
        # count for the line width
        self.width += 1
        self.height += 1

    def toQML(self):
        p = '['
        for pt in self.points:
            p += 'Qt.point(%s,%s),' % (int(pt[0]) - int(self.x), int(pt[1]) - int(self.y))

        p += ']'
        s = """CaPolyline {
    %s
    edge: %s
    lineWidth: %s
    points: %s
}
""" % (super(MEDMPolyline, self).toQML(), EdgeStyle[self.edge], self.lineWidth, p)
        return s

class MEDMPolygon(MEDMGraphics):
    def __init__(self, d, parent=None):
        super(MEDMPolygon, self).__init__(d, parent)
        # line width is always drawn as 1
        self.lineWidth = 1
        self.points = d['points']
        # count for the line width
        self.width += 1
        self.height += 1

    def toQML(self):
        p = '['
        for pt in self.points:
            p += 'Qt.point(%s,%s),' % (pt[0] - self.x, pt[1] - self.y)
        p += ']'
        s = """CaPolygon {
    %s
    fill: %s
    edge: %s
    lineWidth: %s
    points: %s
}
""" % (super(MEDMPolygon, self).toQML(), FillStyle[self.fill], EdgeStyle[self.edge], self.lineWidth, p)
        return s

class MEDMComposite(MEDMObject):
    def __init__(self, d, parent=None):
        super(MEDMComposite, self).__init__(d, parent)
        self.name = d['"composite name"']
        self.file = d.get('"composite file"')
        if self.file:
            pass
        else:
            for c in d['children']:
                child = convertObject(c, self)
                self.children.append(child)

        if '"dynamic attribute"' in d:
            self.channel = d['"dynamic attribute"'].get('chan', '""')
            self.channelB = d['"dynamic attribute"'].get('chanB', '""')
            self.channelC = d['"dynamic attribute"'].get('chanC', '""')
            self.channelD = d['"dynamic attribute"'].get('chanD', '""')
            self.visMode = d['"dynamic attribute"'].get('vis', '"static"')
            self.visCalc = d['"dynamic attribute"'].get('calc', '""')
            self.colorMode = d['"dynamic attribute"'].get('clr', '"static"')

    def toQML(self):
        q = ""
        if hasattr(self, 'channel'):
            q = """
    colorMode: %s
    dynamicAttr.channel: %s
    dynamicAttr.channelB: %s
    dynamicAttr.channelC: %s
    dynamicAttr.channelD: %s
    dynamicAttr.visibilityMode: %s
    dynamicAttr.visibilityCalc: %s
""" % (ColorMode[self.colorMode], self.channel, self.channelB, self.channelC, self.channelD, VisMode[self.visMode], self.visCalc)

        s = ''
        if self.file:
            s = """ Loader {
    %s
    source: %s
}
 """ % (super(MEDMComposite, self).toQML(), self.file)
            return s

        for c in self.children:
            qml = c.toQML()
            # incomplete types
            if '{' not in qml:
                qml = 'Item {\n' + qml + '}\n'
            s += qml

        return """ CaComposite {
        %s
        %s
        %s
}
""" % (super(MEDMComposite, self).toQML(), q, s)

class MEDMStripChart(MEDMObject):
    def __init__(self, d, parent=None):
        super(MEDMStripChart, self).__init__(d, parent)
        self.title = d['plotcom'].get('title', '""')
        self.foreground = '"#%s"' % color_map[int(d['plotcom']['clr'])]
        self.background = '"#%s"' % color_map[int(d['plotcom']['bclr'])]
        self.unit = d.get('units', '"second"')
        self.period = float(d.get('period', '60'))
        self.pens = []
        for i in range(16):
            pen = d.get('pen[%d]'%i, None)
            if pen is None:
                continue
            limits = MEDMLimits(pen['limits'])
            channel = pen['chan']
            color = '"#%s"' % color_map[int(pen['clr'])]
            self.pens.append((channel, color, limits))

    def toQML(self):
        q = ""
        for pen in self.pens:
            q += """
    ListElement {
        channel: %s
        foreground: %s
        loprSrc: %s
        loprDefault: %s
        hoprSrc: %s
        hoprDefault: %s
    }
""" % (pen[0], pen[1], LimitsSource[pen[2].loprSrc], pen[2].loprDefault, LimitsSource[pen[2].hoprSrc], pen[2].hoprDefault)

        s = """
CaStripChart {
    %s
    title: %s
    foreground: %s
    background: %s
    units: %s
    period: %s
    models: ListModel {
%s
    }
}
""" % (super(MEDMStripChart, self).toQML(), self.title, self.foreground, self.background, TimeUnit[self.unit], self.period, q)

        return s

class MEDMCartesianPlot(MEDMObject):
    def __init__(self, d, parent=None):
        super(MEDMCartesianPlot, self).__init__(d, parent)
        self.title = d['plotcom'].get('title', '""')
        self.foreground = '"#%s"' % color_map[int(d['plotcom']['clr'])]
        self.background = '"#%s"' % color_map[int(d['plotcom']['bclr'])]
        self.style = d.get('style', '"point"')
        self.count = int(d.get('count', '""').strip('"'))
        self.countPvName = d.get('countPvName', '""').strip('"')
        if self.countPvName and self.countPvName[0].isdigit():
            self.countPvName = ''

        self.traces = []
        for i in range(16):
            trace = d.get('trace[%d]'%i, None)
            if trace is None:
                return
            xdata = trace.get('xdata', '""')
            ydata = trace.get('ydata', '""')
            color = '"#%s"' % color_map[int(trace['data_clr'])]
            self.traces.append((xdata, ydata, color))

    def toQML(self):
        q = ''
        for trace in self.traces:
            q += """
        ListElement {
            xchannel: %s
            ychannel: %s
            foreground: %s
        }
""" % (trace[0], trace[1], trace[2])
        s = """
CaCartesianPlot {
    %s
    title: %s
    foreground: %s
    background: %s
    models: ListModel {
    %s
    }
}
""" % (super(MEDMCartesianPlot, self).toQML(), self.title, self.foreground, self.background, q)
        return s

def parseADL(lines):
    objects = []
    parent_object = [objects]
    while lines:
        line = lines.pop().strip()

        # opening
        m = re.match('^(.*) {', line)
        if m:
            name = m.groups()[0]
            if name in ['children', 'colors', 'points']:
                current = []
            else:
                current = OrderedDict()
                current['object_type'] = name

            if isinstance(parent_object[-1], list):
                parent_object[-1].append(current)
            else:
                parent_object[-1][name] = current
            parent_object.append(current)

        # attr = value
        m = re.match('^(.*?)=(.*)', line)
        if m:
            attr = m.groups()[0]
            value= m.groups()[1]
            current[attr] = value

        # color,
        m = re.match('([0-9a-fA-F]{6}),', line)
        if m:
            color = m.groups()[0]
            current.append(color)
        # (x,y) point pair
        m = re.match('\((-?[0-9]+),(-?[0-9]+)\)', line)
        if m:
            x = m.groups()[0]
            y = m.groups()[1]
            current.append((int(x),int(y)))
        # closing
        if line == '}':
            parent_object.pop()
            current = parent_object[-1]
    # restructure to adl dictionary
    adl = {'objects':[]}
    for o in objects:
        type_name = o['object_type']
        if type_name in ['file', '"color map"', 'display']:
            adl[type_name] = o
        else:
            adl['objects'].append(o)
    return adl

def dumpADL(adl, indent=''):
    if isinstance(adl, dict):
        for k,v in adl.items():
            sys.stdout.write(indent + k)
            if isinstance(v, dict) or isinstance(v, list):
                sys.stdout.write('\n')
            else:
                sys.stdout.write('=')
            dumpADL(v, indent + '  ')
    elif isinstance(adl, list):
        for v in adl:
            dumpADL(v, indent + '  ')
    else:
        sys.stdout.write(adl)
        sys.stdout.write('\n')

def convertObject(o, parent=None):
    if o['object_type'] == 'text':
        return MEDMText(o, parent)
    elif o['object_type'] == '"text update"':
        return MEDMTextUpdate(o, parent)
    elif o['object_type'] == 'bar':
        return MEDMBar(o, parent)
    elif o['object_type'] == 'byte':
        return MEDMByte(o, parent)
    elif o['object_type'] == 'indicator':
        return MEDMBar(o, parent)
    elif o['object_type'] == 'meter':
        return MEDMMeter(o, parent)
    elif o['object_type'] == '"text entry"':
        return MEDMTextEntry(o, parent)
    elif o['object_type'] == '"message button"':
        return MEDMButton(o, parent)
    elif o['object_type'] == 'menu':
        return MEDMMenu(o, parent)
    elif o['object_type'] == '"choice button"':
        return MEDMChoiceButton(o, parent)
    elif o['object_type'] == '"wheel switch"':
        return MEDMWheelSwitch(o, parent)
    elif o['object_type'] == '"related display"':
        return MEDMRelatedDisplay(o, parent)
    elif o['object_type'] == '"shell command"':
        return MEDMShellCommand(o, parent)
    elif o['object_type'] == 'valuator':
        return MEDMSlider(o, parent)
    elif o['object_type'] == 'image':
        return MEDMImage(o, parent)
    elif o['object_type'] == 'arc':
        return MEDMArc(o, parent)
    elif o['object_type'] =='oval':
        return MEDMOval(o, parent)
    elif o['object_type'] == 'rectangle':
        return MEDMRect(o, parent)
    elif o['object_type'] == 'polyline':
        return MEDMPolyline(o, parent)
    elif o['object_type'] == 'polygon':
        return MEDMPolygon(o, parent)
    elif o['object_type'] == 'composite':
        return MEDMComposite(o, parent)
    elif o['object_type'] == '"strip chart"':
        return MEDMStripChart(o, parent)
    elif o['object_type'] == '"cartesian plot"':
        return MEDMCartesianPlot(o, parent)
    else:
        print 'Not implemented object type', o['object_type']
        return MEDMObject(o, parent)

if __name__ == '__main__':
    import os.path
    fname = sys.argv[1]
    bname, ext = os.path.splitext(fname)

    adl = parseADL(open(fname).readlines()[::-1])
    color_map = adl['"color map"']['colors']
    display = MEDMDisplay(adl)

    with open(bname + '.qml', 'w') as f:
        f.write(display.toQML())
