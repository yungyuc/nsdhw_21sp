import numpy as np
import os
import pytest
import timeit

from _matrix import Matrix, multiply_naive, multiply_mkl, multiply_tile


def test_multiply():
    A = Matrix(1, 5)
    A.from_list([1, 2, 3, 4, 5])

    B = Matrix(5, 1)
    B.from_list([1, -2, 3, -4, 5])

    C = multiply_naive(A, B)

    assert C.nrow, C.ncol == (1, 1)
    assert C[0, 0] == 15

    C = multiply_mkl(A, B)
    assert C.nrow, C.ncol == (1, 1)
    assert C[0, 0] == 15

    C = multiply_tile(A, B, 64)
    assert C.nrow, C.ncol == (1, 1)
    assert C[0, 0] == 15


def test_eq():
    A = Matrix(1, 5)
    B = Matrix(5, 1)

    assert A != B

    A = Matrix(1, 5)
    A.from_list([1, 2, 3, 4, 5])
    B = Matrix(1, 5)
    B.from_list([1, 2, 3, 4, 5])

    assert A == B

    A.from_list([1, 2, 3, 4, 5])
    B.from_list([1, 2, 3, 4, -5])

    assert A != B


@pytest.fixture
def outfile():
    fname = os.path.join(os.path.dirname(__file__), 'performance.txt')
    f = open(fname, 'w')
    yield f
    f.close()


@pytest.mark.parametrize('size', [1000])
def test_performance(size, outfile):
    outfile.write(f'matrix size = {size}x{size}\n')

    A = Matrix(size, size)
    B = Matrix(size, size)
    ls = list(range(1, size * size + 1))
    A.from_list(ls)
    B.from_list(ls)

    # baseline
    d = dict(A=A, B=B, f=multiply_naive)
    t0 = t = min(timeit.repeat('f(A, B)', repeat=5, number=1, globals=d))
    outfile.write(f'multiply_naive time: {t} sec\n')

    # tiled
    for T in [8, 16, 32, 64]:
        d = dict(A=A, B=B, T=T, f=multiply_tile)
        t = min(timeit.repeat('f(A, B, T)', repeat=5, number=1, globals=d))
        outfile.write(f'multiply_tile time: {t} sec, speed-up: {t0/t} x\n')
