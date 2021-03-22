from _vector import angle
import pytest, math

# angle(0, 4, 4, 0) # first two parameters defines the x,y of point1 # the remaining parameters defines the x,y of point2

def test_90_degree():
	assert pytest.approx(math.pi / 2) == abs(angle(0, 4, 4, 0))

def test_0_degree():
	assert pytest.approx(0) == abs(angle(1, 1, 1, 1))

def test_0_length():
	try:
		radians = abs(angle(0, 0, 0, 0))
	except:
		# c++ program will throw InvalidValue exception when the vector is 0 length
		radians = None
		assert radians is None
