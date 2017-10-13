#/usr/bin/env python
"""
This script creates an 25x25 image to display a 2d gaussian profile,
which represents the result of a 2d sscan.
"""
import numpy
import Image

def sscan2d():
    x = numpy.linspace(-1, 1, 20)
    y = numpy.linspace(-1, 1, 20)
    xv, yv = numpy.meshgrid(x, y)
    z = numpy.sin(numpy.sqrt(xv**2+yv**2))
    z = numpy.exp(-(xv**2+yv**2)/0.8)
    z = (z) * 255
    d = numpy.zeros((25,25))
    d[2:22,1:21] = z
    # color scale
    d[2:22:, 22] = numpy.linspace(255, 0, 20)
    d[2:22:, 23] = numpy.linspace(255, 0, 20)

    img = Image.fromarray(d.astype(numpy.uint8))
    img.save('2dsscan.png', transparency=0)

if __name__ == '__main__':
    sscan2d()
