import pytest
import math
from _vector import _angle


def test_both_zero_length():
    with pytest.raises(ValueError) as e:
        theta = _angle((1,1),(2,2))
    assert "vector length : 0" in str(e.value)

def test_one_zero_length():
    with pytest.raises(ValueError) as e:
        theta = _angle((1,2),(2,2))
    assert "vector length : 0" in str(e.value)    
