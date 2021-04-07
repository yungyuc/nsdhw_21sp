import _matrix
import numpy as np
import pytest
import timeit


def test_multiply_naive():
    np.random.seed(5555)
    a = np.random.random((2,2))
    b = np.random.random((2,2))

    mat_a = _matrix.Matrix(a)
    mat_b = _matrix.Matrix(b)

    mat_ret = _matrix.multiply_naive(mat_a, mat_b)

    assert mat_ret.nrow == mat_a.nrow
    assert mat_ret.ncol == mat_b.ncol
    assert np.array(mat_ret) == pytest.approx(np.matmul(a, b),abs=1e-09)