import numpy as np
import pytest
from _matrix import Matrix, multiply_mkl, multiply_naive, multiply_tile


def test_check_naive_answer_by_numpy():
    # first matrix
    na = np.array([
        [1, 2, 3],
        [4, 5, 6],
    ])
    print('na', na)

    ma = Matrix(2, 3)
    ma.load(na)
    print('ma', ma)

    # second matrix
    nb = np.array([
        [1, 2, 3, 4],
        [5, 6, 7, 8],
        [9, 10, 11, 12],
    ])
    print('nb', nb)

    mb = Matrix(3, 4)
    mb.load(nb)
    print('mb', mb)

    # multiply
    ndot = np.dot(na, nb)
    print('ndot', ndot)
    mdot = multiply_naive(ma, mb)
    print('mdot', mdot)

    for i in range(ndot.shape[0]):
        for j in range(ndot.shape[1]):
            assert ndot[i, j] == mdot[i, j]


def test_check_answer_between_three_method():
    ma = Matrix(5, 10)
    for i in range(ma.nrow):
        for j in range(ma.ncol):
            ma[i, j] = i * ma.ncol + j + 1
    print('ma', ma)
    mb = Matrix(10, 20)
    for i in range(mb.nrow):
        for j in range(mb.ncol):
            mb[i, j] = i * mb.ncol + j + 1
    print('mb', mb)

    dot1 = multiply_naive(ma, mb)
    print('dot1', dot1)
    dot2 = multiply_tile(ma, mb, 7)
    print('dot2', dot2)
    dot3 = multiply_mkl(ma, mb)
    print('dot3', dot3)

    assert dot1 == dot2
    assert dot1 == dot3
