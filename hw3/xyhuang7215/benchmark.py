import sys
import os
import timeit
import unittest

# The python module that wraps the matrix code.
import _matrix

class Writer:

    def __init__(self, streams):

        self.streams = streams

    def write(self, msg):

        for stream in self.streams:

            stream.write(msg)


def benchmark():

    setup = '''
import _matrix
size = 1000
mat1 = _matrix.Matrix(size,size)
mat2 = _matrix.Matrix(size,size)
for it in range(size):
    for jt in range(size):
        mat1[it, jt] = it * size + jt + 1
        mat2[it, jt] = it * size + jt + 1
'''

    naive = timeit.Timer('_matrix.multiply_naive(mat1, mat2)', setup=setup)
    tile1 = timeit.Timer('_matrix.multiply_tile(mat1, mat2, 16)', setup=setup)
    tile2 = timeit.Timer('_matrix.multiply_tile(mat1, mat2, 32)', setup=setup)
    tile3 = timeit.Timer('_matrix.multiply_tile(mat1, mat2, 64)', setup=setup)
    mkl = timeit.Timer('_matrix.multiply_mkl(mat1, mat2)', setup=setup)

    repeat = 5

    with open('performance.txt', 'w') as fobj:

        w = Writer([sys.stdout, fobj])

        w.write(f'Start multiply_naive (repeat={repeat}), take min = ')
        naivesec = minsec = min(naive.repeat(repeat=repeat, number=1))
        w.write(f'{minsec} seconds\n')

        w.write(f'Start multiply_tile (blockSize 16) (repeat={repeat}), take min = ')
        tilesec1 = minsec = min(tile1.repeat(repeat=repeat, number=1))
        w.write(f'{minsec} seconds\n')

        w.write(f'Start multiply_tile (blockSize 32) (repeat={repeat}), take min = ')
        tilesec2 = minsec = min(tile2.repeat(repeat=repeat, number=1))
        w.write(f'{minsec} seconds\n')

        w.write(f'Start multiply_tile (blockSize 64) (repeat={repeat}), take min = ')
        tilesec3 = minsec = min(tile3.repeat(repeat=repeat, number=1))
        w.write(f'{minsec} seconds\n')

        w.write('tile 16 speed-up over naive: %g x\n' % (naivesec/tilesec1))
        w.write('tile 32 speed-up over naive: %g x\n' % (naivesec/tilesec2))
        w.write('tile 64 speed-up over naive: %g x\n' % (naivesec/tilesec3))
        
if __name__ == '__main__':
    benchmark()