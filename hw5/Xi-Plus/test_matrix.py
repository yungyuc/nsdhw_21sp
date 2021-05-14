import _matrix
import numpy as np
import pytest


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
