import pytest
import math
from _vector import _angle


def test_both_zero_length():
    with pytest.raises(ValueError) as e:
        theta = _angle((0,0), (0,0))
    assert "vector length : 0" in str(e.value)

def test_one_zero_length():
    with pytest.raises(ValueError) as e:
        theta = _angle((1,2), (0,0))
    assert "vector length : 0" in str(e.value)  

def test_zero_angle():
    theta = _angle((1,2), (2,4))
    assert theta == 0.0

def test_right_angle():
    theta = _angle((0,1), (1,0))
    assert theta == pytest.approx(math.pi / 2)

def test_30_angle():
    theta = _angle((1,0), (1,1))
    assert theta == pytest.approx(math.pi / 4)


