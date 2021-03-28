import pytest
import math
from _vector import angle


def test_zero_length():
    try:
        rad = angle([0.0, 0.0], [0.0, 0.0])
        assert False
    except ValueError:
        pass


def test_zero_angle():
    rad = angle([5.0, 5.0], [1.0, 1.0])

    assert pytest.approx(rad) == 0.0


def test_right_angle():
    rad = angle([0.0, 5.0], [5.0, 0.0])

    assert pytest.approx(rad) == pytest.approx(math.pi / 2)



