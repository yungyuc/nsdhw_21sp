import numpy as np

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
