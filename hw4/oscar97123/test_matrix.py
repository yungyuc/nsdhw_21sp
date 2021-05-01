import _matrix
import pytest


def test_for_the_3_approaches():
    n = 1000

    mat_1 = _matrix.Matrix(n, n)
    mat_2 = _matrix.Matrix(n, n)

    naive = _matrix.multiply_naive(mat_1, mat_2)
    tile = _matrix.multiply_tile(mat_1, mat_2, 64)
    mkl = _matrix.multiply_mkl(mat_1, mat_2)

    assert tile == naive
    assert mkl == naive
