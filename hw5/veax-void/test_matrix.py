import timeit

import _matrix
import numpy as np
import pytest

def test_check_naive_answer_by_numpy():
    # first matrix
    na = np.array([
        [1, 2, 3],
        [4, 5, 6],
    ])
    print('na', na)

    ma = _matrix.Matrix(2, 3)
    ma.load(na)
    print('ma', ma)

    # second matrix
    nb = np.array([
        [1, 2, 3, 4],
        [5, 6, 7, 8],
        [9, 10, 11, 12],
    ])
    print('nb', nb)

    mb = _matrix.Matrix(3, 4)
    mb.load(nb)
    print('mb', mb)

    # multiply
    ndot = np.dot(na, nb)
    print('ndot', ndot)
    mdot = _matrix.multiply_naive(ma, mb)
    print('mdot', mdot)

    for i in range(ndot.shape[0]):
        for j in range(ndot.shape[1]):
            assert ndot[i, j] == mdot[i, j]


def test_check_answer_between_three_method():
    ma = _matrix.Matrix(5, 10)
    for i in range(ma.nrow):
        for j in range(ma.ncol):
            ma[i, j] = i * ma.ncol + j + 1
    print('ma', ma)
    mb = _matrix.Matrix(10, 20)
    for i in range(mb.nrow):
        for j in range(mb.ncol):
            mb[i, j] = i * mb.ncol + j + 1
    print('mb', mb)

    dot1 = _matrix.multiply_naive(ma, mb)
    print('dot1', dot1)
    dot2 = _matrix.multiply_tile(ma, mb, 7)
    print('dot2', dot2)
    dot3 = _matrix.multiply_mkl(ma, mb)
    print('dot3', dot3)

    assert dot1 == dot2
    assert dot1 == dot3


def test_memory():
    base_alloc = _matrix.allocated()
    base_dealloc = _matrix.deallocated()

    size = 100
    ma = _matrix.Matrix(size, size)

    assert _matrix.bytes() == 8 * size * size
    assert _matrix.allocated() == base_alloc + 8 * size * size
    assert _matrix.deallocated() == base_dealloc

    mb = _matrix.Matrix(size, size)

    assert _matrix.bytes() == 8 * size * size * 2
    assert _matrix.allocated() == base_alloc + 8 * size * size * 2
    assert _matrix.deallocated() == base_dealloc

    ma = None

    assert _matrix.bytes() == 8 * size * size
    assert _matrix.allocated() == base_alloc + 8 * size * size * 2
    assert _matrix.deallocated() == base_dealloc + 8 * size * size

    mb = None

    assert _matrix.bytes() == 0
    assert _matrix.allocated() == base_alloc + 8 * size * size * 2
    assert _matrix.deallocated() == base_dealloc + 8 * size * size * 2