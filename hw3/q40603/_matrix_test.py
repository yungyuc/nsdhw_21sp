from _matrix import *
import numpy as np
import pytest
import time


# def test_multiply_naive_small():
#     np.random.seed(5555)
#     a = np.random.random((2,2))
#     b = np.random.random((2,2))

#     mat_a = Matrix(a)
#     mat_b = Matrix(b)

#     mat_ret = multiply_naive(mat_a, mat_b)
#     mat_ans = Matrix(np.matmul(a, b))

#     # mat_ret.show()
#     # print(np.matmul(a, b))

#     assert mat_ret.nrow == mat_a.nrow
#     assert mat_ret.ncol == mat_b.ncol

#     for i in range(mat_ret.nrow):
#         for j in range(mat_ret.ncol):
#             assert mat_ret[i,j] == pytest.approx(mat_ans[i,j], 0.1)

# def test_multiply_naive_big():
#     np.random.seed(5555)
#     a = np.random.random((1000, 1000))
#     b = np.random.random((1000, 1000))

#     mat_a = Matrix(a)
#     mat_b = Matrix(b)

#     mat_ret = multiply_naive(mat_a, mat_b)
#     mat_ans = Matrix(np.matmul(a, b))

#     # mat_ret.show()
#     # print(np.matmul(a, b))

#     assert mat_ret.nrow == mat_a.nrow
#     assert mat_ret.ncol == mat_b.ncol

#     for i in range(mat_ret.nrow):
#         for j in range(mat_ret.ncol):
#             assert mat_ret[i,j] == pytest.approx(mat_ans[i,j], 0.1)

# def test_multiply_tile_small():
#     np.random.seed(5555)
#     a = np.random.random((2,2))
#     b = np.random.random((2,2))

#     mat_a = Matrix(a)
#     mat_b = Matrix(b)

#     mat_ret = multiply_tile(mat_a, mat_b, 1)
#     mat_ans = Matrix(np.matmul(a, b))

#     # mat_ret.show()
#     # print(np.matmul(a, b))

#     assert mat_ret.nrow == mat_a.nrow
#     assert mat_ret.ncol == mat_b.ncol

#     for i in range(mat_ret.nrow):
#         for j in range(mat_ret.ncol):
#             assert mat_ret[i,j] == pytest.approx(mat_ans[i,j], 0.1)

# def test_multiply_tile_big():
#     np.random.seed(5555)
#     a = np.random.random((1000, 1000))
#     b = np.random.random((1000, 1000))

#     mat_a = Matrix(a)
#     mat_b = Matrix(b)

#     mat_ret = multiply_tile(mat_a, mat_b, 1)
#     mat_ans = Matrix(np.matmul(a, b))

#     # mat_ret.show()
#     # print(np.matmul(a, b))

#     assert mat_ret.nrow == mat_a.nrow
#     assert mat_ret.ncol == mat_b.ncol

#     for i in range(mat_ret.nrow):
#         for j in range(mat_ret.ncol):
#             assert mat_ret[i,j] == pytest.approx(mat_ans[i,j], 0.1)


def test_performance():

    np.random.seed(5555)
    a = np.random.random((1000, 1000))
    b = np.random.random((1000, 1000))

    mat_a = Matrix(a)
    mat_b = Matrix(b)

    naive_record = []
    tile_record = []

    for i in range(8):
        timer = time.time()
        multiply_naive(mat_a, mat_b)
        naive_record.append(time.time() - timer)

        timer = time.time()
        multiply_tile(mat_a, mat_b, 64)
        tile_record.append(time.time() - timer)

    naive_avg = sum(naive_record) / len(naive_record) 
    tile_avg = sum(tile_record) / len(tile_record)
    with open("performance.txt", "w") as f:
        f.write("[Average execution time]\n")
        f.write("multiply_naive: {} secs\n".format("%.4f"%naive_avg))
        f.write("multiply_tile : {} secs\n".format("%.4f"%tile_avg))
        # f.write("multiply_mkl  : {} secs\n".format("%.4f"%mkl_avg))
        f.write("The tiling version is {:.1%} faster than naive version\n".format(naive_avg / tile_avg))
        # f.write("The mkl version is {:.1%} faster than naive version\n".format(naive_avg / mkl_avg))
        f.close()

    assert tile_avg / naive_avg <= 0.8