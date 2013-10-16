#!/usr/bin/env python

import re
from collections import OrderedDict

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
    return objects

adl = parseADL(open('example.adl').readlines()[::-1])

print adl