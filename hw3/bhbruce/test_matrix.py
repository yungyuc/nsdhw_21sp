import pytest
import _matrix as M

def test_case1():
    a = M.Matrix(5,10)
    b = M.Matrix(10,8)

    for i in range(5):
        for j in range(10):
            a[i,j] = i*10+j;
    for i in range(10):
        for j in range(8):
            b[i,j] = i+j;

    result_naive = M.multiply_naive(a,b)
    result_tile_8 = M.multiply_tile(a,b,8)
    result_mkl = M.multiply_mkl(a,b)
    print(result_naive)
    assert(result_naive == result_tile_8)
    assert(result_naive == result_mkl)



