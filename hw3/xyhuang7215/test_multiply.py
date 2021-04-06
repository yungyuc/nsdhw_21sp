import _matrix
import numpy as np

def test_multiplyNaive():
    m = _matrix.Matrix(4, 4)

    for i in range(m.nrow):
        for j in range(m.ncol):
            m[i, j] = i * m.ncol + j

    m = _matrix.multiply_naive(m, m)

    ans = np.array([[56.0, 62.0, 68.0, 74.0],
                    [152.0, 174.0, 196.0, 218.0],
                    [248.0, 286.0, 324.0, 362.0],
                    [344.0, 398.0, 452.0, 506.0]], dtype='float64')

    for i in range(m.nrow):
        for j in range(m.ncol):
            assert m[i, j] == ans[i, j]


def test_multiplyTile():
    m = _matrix.Matrix(4, 4)

    for i in range(m.nrow):
        for j in range(m.ncol):
            m[i, j] = i * m.ncol + j

    m = _matrix.multiply_tile(m, m, 3)

    ans = np.array([[56.0, 62.0, 68.0, 74.0],
                    [152.0, 174.0, 196.0, 218.0],
                    [248.0, 286.0, 324.0, 362.0],
                    [344.0, 398.0, 452.0, 506.0]], dtype='float64')

    for i in range(m.nrow):
        for j in range(m.ncol):
            assert m[i, j] == ans[i, j]


def test_multiplyMkl():
    m = _matrix.Matrix(4, 4)

    for i in range(m.nrow):
        for j in range(m.ncol):
            m[i, j] = i * m.ncol + j

    m = _matrix.multiply_mkl(m, m)

    ans = np.array([[56.0, 62.0, 68.0, 74.0],
                    [152.0, 174.0, 196.0, 218.0],
                    [248.0, 286.0, 324.0, 362.0],
                    [344.0, 398.0, 452.0, 506.0]], dtype='float64')

    for i in range(m.nrow):
        for j in range(m.ncol):
            assert m[i, j] == ans[i, j]