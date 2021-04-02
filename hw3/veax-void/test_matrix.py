#!/usr/bin/env python3

import numpy as np
import random
import time
import _matrix

from pytest import approx


class TestMatrix:
    def is_close_to_equal(self, m1, m2, m, n):
        for i in range(m):
            for j in range(n):
                assert m1[i, j] == approx(m2[i, j])
        return True

    def calculation_helper(self, p, q, r, tsize):
        m1 = np.random.rand(p, q)
        m2 = np.random.rand(q, r)
        m3 = np.matmul(m1, m2)

        _m1 = _matrix.Matrix(m1)
        _m2 = _matrix.Matrix(m2)

        tstart = time.clock()
        ret_naive = _matrix.multiply_naive(_m1, _m2)
        tnaive = time.clock() - tstart
        tstart = time.clock()
        ret_tile = _matrix.multiply_tile(_m1, _m2, tsize)
        ttile = time.clock() - tstart
        tstart = time.clock()
        ret_mkl = _matrix.multiply_mkl(_m1, _m2)
        tmkl = time.clock() - tstart

        assert m3.shape[0] == ret_naive.nrow and m3.shape[1] == ret_naive.ncol
        assert m3.shape[0] == ret_tile.nrow and m3.shape[1] == ret_tile.ncol
        assert m3.shape[0] == ret_mkl.nrow and m3.shape[1] == ret_mkl.ncol
        assert(self.is_close_to_equal(m3, ret_naive, p, r))
        assert(self.is_close_to_equal(m3, ret_tile, p, r))
        assert(self.is_close_to_equal(m3, ret_mkl, p, r))

        with open('performance.txt', 'w') as f:
            f.write('multiply_naive: {}\n'.format(tnaive))
            f.write('multiply_tile: {}\n'.format(ttile))
            f.write('multiply_mkl: {}\n'.format(tmkl))
            f.write('tile speed-up over naive: {}\n'.format(tnaive / ttile))
            f.write('MKL speed-up over naive: {}\n'.format(tnaive / tmkl))

    def test_1000(self):
        self.calculation_helper(1000, 1000, 1000, 16)

    def test_random(self):
        p = random.randint(1, 1000)
        q = random.randint(1, 1000)
        r = random.randint(1, 1000)
        tsize = random.randint(1, 1000)
        self.calculation_helper(p, q, r, tsize)
