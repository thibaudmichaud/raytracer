#!/usr/bin/python3

import argparse
import math

diff = 1
refl = 0.35
spec = 0.3
shin = 50
refr = 1
opac = 1

def draw_line(r, num, x, y, z, col_r, col_g, col_b):
    spheres = []
    for i in range(num):
        spheres.append('sphere {} {} {} {} {} {} {} {} {} {} {} {} {}'.format
                (r, x + (2* r * i), y, -z, diff, refl, spec, shin, col_r, col_g,
                 col_b, refr, opac))
    return spheres

def pyramid(floor, r, x_st, y_st, z_st, color_r, color_g, color_b):
    num = floor

    x_d = r
    z_d = (3 * z_st + r * math.sqrt(3)) / 3 - z_st
    y_d = math.sqrt(3*r*r - z_d * z_d)

    lines = []
    while (floor > 0):
        num = floor
        x, y, z = x_st, y_st, z_st
        print('floor ' + str(floor))
        while(num > 0):
            print('num ' + str(num))
            lines.extend(draw_line(r, num, x, y, z, color_r, color_g, color_b))
            num -= 1
            x += r
            z += r * math.sqrt(3)
        floor -= 1

        x_st += x_d
        z_st += z_d
        y_st -= y_d

    return lines

def main():
    lines = ['screen 800 800',
             'camera 0 0 0 1 0 0 0 1 0',
             'plane 0 -1 0 2 1 0.5 0 0 120 120 120 1 1',
             'alight 100 100 80',
             'dlight 0 0 1 100 100 100']

    lines.extend(pyramid(2, 1, -7, 1, -20, 255, 0, 0))
    lines.extend(pyramid(3, 1, -7, 1, -27, 0, 255, 0))
    lines.extend(pyramid(4, 1, -4, 1, -34, 0, 0, 255))
    lines.extend(pyramid(5, 1, 2, 1, -27, 255, 255, 0))

    with open('pyramid', 'w') as f:
        for line in lines:
            f.write(line + '\n')

if __name__ == '__main__':
    main()
