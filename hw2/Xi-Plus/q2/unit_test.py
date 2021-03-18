#!/usr/bin/env python3

import pytest
import math
from _vector import vector_angle


def test_zero_length():
    with pytest.raises(ValueError):
        vector_angle((0, 0), (1, 1))
    with pytest.raises(ValueError):
        vector_angle((1, 1), (0, 0))
    with pytest.raises(ValueError):
        vector_angle((0, 0), (0, 0))


def test_zero_angle():
    assert vector_angle((0, 1), (0, 1)) == pytest.approx(0)
    assert vector_angle((0, 1), (0, 2)) == pytest.approx(0)
    assert vector_angle((1, 1), (1, 1)) == pytest.approx(0)
    assert vector_angle((1, 1), (2, 2)) == pytest.approx(0)


def test_right_angle():
    deg90 = math.pi / 2
    assert vector_angle((0, 1), (1, 0)) == pytest.approx(deg90)
    assert vector_angle((1, 2), (-2, 1)) == pytest.approx(deg90)
    assert vector_angle((1, 2), (2, -1)) == pytest.approx(deg90)


def test_other_angle():
    assert vector_angle((1, 2), (-3, -6)) == pytest.approx(math.pi)  # 180
    assert vector_angle((1, 0), (5, 5)) == pytest.approx(math.pi / 4)  # 45
    assert vector_angle((1, 0), (math.sqrt(3), 1)) == pytest.approx(math.pi / 6)  # 30
    assert vector_angle((1, 0), (1, math.sqrt(3))) == pytest.approx(math.pi / 3)  # 60
