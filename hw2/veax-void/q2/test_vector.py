#!/usr/bin/env python3
import numpy as np
import pytest
import math
from _vector import cartesian_angle

def test_invalid():
    with pytest.raises(TypeError):
        cartesian_angle((0, 0), ())
    with pytest.raises(TypeError):
        cartesian_angle((), (0, 0))
    with pytest.raises(TypeError):
        cartesian_angle((), ())
        
def test_zero_len():
    with pytest.raises(ValueError):
        cartesian_angle((1, 1), (0, 0))
    with pytest.raises(ValueError):
        cartesian_angle((0, 0), (0, 0))
    with pytest.raises(ValueError):
        cartesian_angle((0, 0), (1, 1))
        
def test_zero_angle():
    for i in range(10):
        mult = np.random.randint(1, 10)
        vec1 = np.random.randint(1, 10, 2)
        vec2 = vec1 * mult
        assert cartesian_angle(vec1, vec2) == pytest.approx(0)

def test_right_angle():
    for i in range(10):
        mult = np.random.randint(1, 10)
        vec1 = np.random.randint(1, 10)
        vec1 = np.array((vec1, vec1))
        vec2 = vec1 * np.array((-mult, mult))
        assert cartesian_angle(vec1, vec2) == pytest.approx(math.pi / 2)

def test_opposite():
    for i in range(10):
        mult = np.random.randint(-10, -1)
        vec1 = np.random.randint(1, 10, 2)
        vec2 = vec1 * mult
        assert cartesian_angle(vec1, vec2) == pytest.approx(math.pi)