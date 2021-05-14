import _matrix


def test_correctness():
    size = 500

    m1 = _matrix.Matrix(size, size)
    m2 = _matrix.Matrix(size, size)

    m3_naive = _matrix.multiply_naive(m1, m2)
    m3_tile = _matrix.multiply_tile(m1, m2, 32)
    m3_mkl = _matrix.multiply_mkl(m1, m2)

    assert (m3_naive == m3_tile)
    assert (m3_naive == m3_mkl)
