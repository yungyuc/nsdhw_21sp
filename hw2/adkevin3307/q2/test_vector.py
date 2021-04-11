import pytest
import math
from _vector import angle


def test_zero_length():
    try:
        _angle = angle([0.0, 0.0], [0.0, 0.0])
    except RuntimeError:
        _angle = None

    assert _angle == None


def test_zero_angle():
    _angle = angle([1.0, 1.0], [1.0, 1.0])

    assert pytest.approx(_angle) == 0.0


def test_right_angle():
    _angle = angle([0.0, 1.0], [1.0, 0.0])

    assert pytest.approx(_angle) == pytest.approx(math.pi / 2)


def test_any_angle():
    _angle = angle([0.42739941, 0.85395232], [0.27739064, 0.91144985])

    assert pytest.approx(_angle) == pytest.approx(0.16861037166907683)
