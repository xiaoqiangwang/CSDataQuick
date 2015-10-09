from PIL import Image
import numpy
import re

def readxpm(fname):
    incomment = False
    f = open(fname)

    # skip comments
    while True:
        line = f.readline().strip()
        # c block comment
        if line.startswith('/*'):
            incomment = True
        elif line.endswith('*/'):
            incomment = False
        if incomment:
            continue
        # black line
        if not line:
            continue

        if line.startswith('#define'):
            break

    while True:
        m = re.match("#define ([^_]*)_width ([0-9]+)", line)
        if not m:
            raise SyntaxError("Invalid xbm format")
        name = m.groups()[0]
        width = int(m.groups()[1])

        line = f.readline().strip()
        m = re.match("#define %s_height ([0-9]+)"%(name), line)
        if not m:
            raise SyntaxError("Invalid xbm format")
        height = int(m.groups()[0])
       
        line = f.readline().strip()
        m = re.match("static unsigned char %s_bits\\[\\] = {"%name, line)
        if not m:
            raise SyntaxError("Invalid xbm format")
        bitstring = ''
        while True:
            line = f.readline().strip()
            if line.endswith('};'):
                bitstring += line[:-2]
                break
            else:
                bitstring += line

        bytelist = [~int(byte, 16) for byte in bitstring.split(',')]
        bits = []

        cnt = 0
        for byte in bytelist:
            for i in range(8):
                # one row ended as soon as width reached, skip the rest of bits
                if cnt >= width:
                    cnt = 0
                    break
                cnt += 1
                bits.append(((byte >> i) & 1) * 255)

        bits = numpy.array(bits, dtype=numpy.uint8) 
        bits.shape = (width, height)

        im = Image.fromarray(bits)
        im.save('%s.png'%re.sub('[0-9]+$', '', name))
       
        # read the next image
        line = f.readline().strip()
        if not line:
            break

if __name__ == '__main__':
    readxpm('medmPix25.xpm')
