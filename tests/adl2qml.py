#!/usr/bin/env python
"""
A module to parse adl display file created by MEDM.


"""


import re
import sys
from collections import OrderedDict

TextAlign = {
'"horiz. left"' : '"AlignLeft"',
'"horiz. centered"' : '"AlignHCenter"',
'"horiz. right"' : '"AlignRight"',
'"vert. left"' : '"AlignLeft"',
'"vert. centered"' : '"AlignHCenter"',
'"vert. right"' : '"AlignRight"',
}

class MEDMObject(object):
    def __init__(self, d, parent=None):
        self.parent = parent
        self.children = []

        self.x = d['object']['x']
        self.y = d['object']['y']
        self.width = d['object']['width']
        self.height = d['object']['height']

    def toQML(self):
        s = """Item {
    x: %s
    y: %s
    width: %s
    height: %s
}
""" % (self.x, self.y, self.width, self.height)
        return s

class MEDMDisplay(MEDMObject):
    def __init__(self, d, parent=None):
        self.parent = parent
        self.children = []
        self.x = d['display']['object']['x']
        self.y = d['display']['object']['y']
        self.width = d['display']['object']['width']
        self.height = d['display']['object']['height']
        for o in d['objects']:
            child = convertObject(o, self)
            self.children.append(child)

    def toQML(self):
        s = ''
        for c in self.children:
            s += c.toQML()

        return """import QtQuick 2.0
import QtQuick.Controls 1.0
import PvComponents 1.0

ApplicationWindow {
    x: %s
    y: %s
    width: %s
    height: %s

    %s

}
""" % (self.x, self.y, self.width, self.height, s)

class MEDMText(MEDMObject):
    def __init__(self, d, parent=None):
        super(MEDMText, self).__init__(d, parent)
        self.text = d['textix']
        self.color  =  '"#%s"' % color_map[int(d['"basic attribute"']['clr'])]
        self.align =  '%s' % TextAlign[d.get('align', '"horiz. left"')]

    def toQML(self):
        s = """CaText {
    x: %s
    y: %s
    width: %s
    height: %s
    color: %s
    horizontalAlignment: %s
}
""" % (self.x, self.y, self.width, self.height, self.color, self.align)
        return s


class MEDMTextUpdate(MEDMObject):
    def __init__(self, d, parent=None):
        super(MEDMTextUpdate, self).__init__(d, parent)
        self.align =  '%s' % TextAlign[d.get('align', '"horiz. left"')]
        self.color = '"#%s"' % color_map[int(d['monitor']['clr'])]
        self.background = '"#%s"' % color_map[int(d['monitor']['bclr'])]
        self.channel =  d['monitor']['chan']
        self.color_mode = d.get('clrmod', 'static')
        self.prec_src = d['limits'].get('precSrc', 'channel')
        self.prec_default = d['limits'].get('precDefault', 0)
        self.format = d.get('format', 'decimal')

    def toQML(self):
        s = """CaTextLabel {
    x: %s
    y: %s
    width: %s
    height: %s
    color: %s
//    background: %s
//    horizontalAlignment: %s
    channel: %s
    alarmColor: %s
}
""" % (self.x, self.y, self.width, self.height, self.color, self.background, self.align, self.channel, str(self.color_mode=='alarm').lower())
        return s

class MEDMTextEntry(MEDMObject):
    def __init__(self, d, parent=None):
        super(MEDMTextEntry, self).__init__(d, parent)
        self.align =  '%s' % TextAlign[d.get('align', '"horiz. left"')]
        self.color = '"#%s"' % color_map[int(d['control']['clr'])]
        self.background = '"#%s"' % color_map[int(d['control']['bclr'])]
        self.channel = d['control']['chan']
        self.color_mode =  d.get('clrmod', 'static')
        self.prec_src = d['limits'].get('precSrc', 'channel')
        self.prec_default = d['limits'].get('precDefault', 0)
        self.format = d.get('format', 'decimal')

    def toQML(self):
        s = """CaTextEntry {
    x: %s
    y: %s
    width: %s
    height: %s
    color: %s
//    background: %s
//    horizontalAlignment: %s
    channel: %s
    alarmColor: %s
}
""" % (self.x, self.y, self.width, self.height, self.color, self.background, self.align, self.channel, str(self.color_mode=='alarm').lower())
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
                child = convertObject(c, parent)
                self.children.append(child)

    def toQML(self):
        s = ''
        for c in self.children:
            s += c.toQML()

        return """ GroupBox {
        x: %s
        y: %s
        width: %s
        height: %s

        %s

}
""" % (self.x, self.y, self.width, self.height, s)

def parseADL(lines):
    objects = []
    parent_object = [objects]
    while lines:
        line = lines.pop().strip()

        # opening
        m = re.match('^(.*) {', line)
        if m:
            name = m.groups()[0]
            if name in ['children', 'colors']:
                current = []
            else:
                current = OrderedDict()
                current['type'] = name

            if isinstance(parent_object[-1], list):
                parent_object[-1].append(current)
            else:
                parent_object[-1][name] = current
            parent_object.append(current)

        # attr = value
        m = re.match('^(.*)=(.*)', line)
        if m:
            attr = m.groups()[0]
            value= m.groups()[1]
            current[attr] = value

        # color,
        m = re.match('([0-9a-fA-F]{6}),', line)
        if m:
            color = m.groups()[0]
            current.append(color)

        # closing
        if line == '}':
            parent_object.pop()
            current = parent_object[-1]
    # restructure to adl dictionary
    adl = {'objects':[]}
    for o in objects:
        type_name = o['type']
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
    if o['type'] == 'text':
        return MEDMText(o, parent)
    elif o['type'] == '"text update"':
        return MEDMTextUpdate(o, parent)
    elif o['type'] == '"text entry"':
        return MEDMTextEntry(o, parent)
    elif o['type'] == 'composite':
        return MEDMComposite(o, parent)
    else:
        return MEDMObject(o, parent)

adl = parseADL(open('example.adl').readlines()[::-1])
color_map = adl['"color map"']['colors']
display = MEDMDisplay(adl)

with open('example.qml', 'w') as f:
    f.write(display.toQML())
