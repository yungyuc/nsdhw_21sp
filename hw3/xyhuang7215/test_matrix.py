import _matrix
import numpy as np

def test_createMatrix():
    m = _matrix.Matrix(3, 3)

    assert m.ncol == 3
    assert m.nrow == 3

    for i in range(m.nrow):
        for j in range(m.ncol):
            assert m[i, j] == 0


def test_setMatrixItem():
    m = _matrix.Matrix(3, 3)

    for i in range(m.nrow):
        for j in range(m.ncol):
            m[i, j] = i * m.ncol + j

    ans = np.array([[0, 1, 2],
                    [3, 4, 5],
                    [6, 7, 8]], dtype='float64')
    
    for i in range(m.nrow):
        for j in range(m.ncol):
            assert m[i, j] == ans[i, j]
