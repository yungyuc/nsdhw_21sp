import _vector as vector
import math

import pytest

from pytest import approx

@pytest.mark.parametrize(
    'v1,v2,ans',
    [
        ([0, 1], [0, 1], 0),
        ([0, 2], [0, 42], 0),
        ([-1, 2], [-24, 48], 0),

        ([0, 1], [1, 0], math.pi / 2),
        ([0, -1], [1, 0], math.pi / 2),
        ([0, 2], [5, 0], math.pi / 2),

        ([0, 1], [0, -1], math.pi),
        ([0, 42], [0, -1], math.pi),
        ([1, 0], [-1, 0], math.pi),
        ([42, 0], [-5, 0], math.pi),
    ]
)
def test_angle(v1, v2, ans):
    assert vector.angle(*v1, *v2) == approx(ans)
    assert vector.angle(*v2, *v1) == approx(ans)


@pytest.mark.parametrize(
    'v1, v2',
    [
        ([0, 0], [1, 2]),
        ([1, 2], [0, 0]),
        ([0, 0], [0, 0]),
    ]
)
def test_invalid(v1, v2):
    with pytest.raises(ValueError):
        vector.angle(*v1, *v2)
