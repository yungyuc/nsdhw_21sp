import _vector
import numpy as np

def testRightAngle():
    # 90 degree
    radians = _vector.getAngle([0, 1], [1, 0])
    assert round(radians, 8) == round(np.pi / 2, 8)


def testOtherAngle():
    # 180 degree
    radians = _vector.getAngle([-1, 0], [1, 0])
    assert round(radians, 8) == round(np.pi, 8)