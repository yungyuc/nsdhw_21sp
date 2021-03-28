import math

import pytest
import _vector

def test_zero_len():
    with pytest.raises(Exception) as e_info:
         _vector.cal_angle([0,0],[0,1])

def test_zero_angle():
    assert _vector.cal_angle([5,3],[5,3]) == 0 

def test_right_angle():
    assert _vector.cal_angle([1,0],[0,1]) == pytest.approx(math.pi / 2)

def test_normal():
    assert _vector.cal_angle([1,2],[3,4]) == pytest.approx(0.17985349893569946)
