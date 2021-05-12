import _matrix as m
import random
import numpy as np
import time
import unittest

smallsize = 100
largesize = 1024

unittest.util._MAX_LENGTH=2000

def test_insert():
    
    mat1 = m.Matrix(smallsize, smallsize)
    mat1[ 0,0 ] = 100
    
    assert mat1[ 0,0 ] == 100

def test_copy():
    
    mat1 = m.Matrix(smallsize, smallsize)

    for x in range( smallsize ):
        for y in range( smallsize ):
            mat1[x,y] = x*y
    
    mat2 = mat1
    
    assert mat2[ smallsize - 1 , smallsize - 1 ] == ( smallsize - 1  )*( smallsize - 1 )

def test_compare():
    
    mat1 = m.Matrix(smallsize, smallsize)

    for x in range( smallsize ):
        for y in range( smallsize ):
            mat1[x,y] = x*y
    
    mat2 = mat1
    
    assert mat1 == mat2
    
def test_correct():
    
    mat1 = m.Matrix(smallsize, smallsize)
    mat2 = m.Matrix(smallsize, smallsize)

    for x in range( smallsize ):
        for y in range( smallsize ):
            mat1[x,y] = x* y
            mat2[x,y] = x * y

    naive = m.multiply_naive(mat1, mat2)
    tile = m.multiply_tile(mat1, mat2, 32)
    mkl = m.multiply_mkl(mat1, mat2)

    for x in range( smallsize ):
        for y in range( smallsize ):
           print( tile[x,y] )

    
    assert tile == naive
    assert naive == mkl
    assert tile == mkl

def test_speed():
    
    mat1 = m.Matrix(largesize, largesize)
    mat2 = m.Matrix(largesize, largesize)

    fp = open("performance.txt", "a")

    for x in range( largesize ):
        for y in range( largesize ):
            mat1[x,y] = random.random()
            mat2[x,y] = random.random()

    t_naive = time.time()
    naive = m.multiply_naive(mat1, mat2)
    t_naive = time.time() - t_naive
    fp.write('multiply_naive use ' + str(t_naive) + 's\n' )

    tilesize = 1
    t_tilemin  = t_naive * 2
    while tilesize * tilesize <= largesize :
        tilesize *= 2
        t_tile = time.time()
        tile = m.multiply_tile(mat1, mat2, tilesize)
        t_tile = time.time() - t_tile
        fp.write('multiply_tile use ' + str(t_tile) +'s with tile size ' + str(tilesize) + '\n' )
        if t_tilemin > t_tile :
            t_tilemin = t_tile

    t_mkl = time.time()
    mkl = m.multiply_mkl(mat1, mat2)
    t_mkl = time.time() - t_mkl
    fp.write('multiply_mkl use ' + str(t_mkl) + 's\n' )

    fp.close()
    assert t_naive * 0.8 > t_tilemin