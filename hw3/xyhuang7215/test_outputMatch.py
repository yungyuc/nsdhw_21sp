import _matrix
import numpy as np

def test_allMatch():
    a = _matrix.Matrix(1000, 1000)
    b = _matrix.Matrix(1000, 1000)

    for i in range(a.nrow):
        for j in range(a.ncol):
            a[i, j] = i * a.ncol + j
            b[i, j] = i * a.ncol + j + 1

    mat1 = _matrix.multiply_naive(a, b)
    mat2 = _matrix.multiply_tile(a, b, 8)
    mat3 = _matrix.multiply_mkl(a, b)

    assert mat1.nrow == mat2.nrow
    assert mat1.ncol == mat2.ncol
    assert mat1.nrow == mat3.nrow
    assert mat1.ncol == mat3.ncol
    assert mat2.nrow == mat3.nrow
    assert mat2.ncol == mat3.ncol

    for i in range(mat1.nrow):
        for j in range(mat1.ncol):
            assert mat1[i, j] == mat2[i, j]

    for i in range(mat1.nrow):
        for j in range(mat1.ncol):
            assert mat1[i, j] == mat3[i, j]

    for i in range(mat1.nrow):
        for j in range(mat1.ncol):
            assert mat2[i, j] == mat3[i, j]
