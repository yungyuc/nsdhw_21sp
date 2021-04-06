from _matrix import *
import numpy as np
import pytest
import time


def test_multiply_naive_small():
    np.random.seed(5555)
    a = np.random.randint(10, size=(2,2))
    b = np.random.randint(10, size=(2,2))

    mat_a = Matrix(a)
    mat_b = Matrix(b)


    mat_ret = multiply_naive(mat_a, mat_b)
    mat_ans = Matrix(np.matmul(a, b))

    # mat_ret.show()
    # print(np.matmul(a, b))

    assert mat_ret.nrow == mat_a.nrow
    assert mat_ret.ncol == mat_b.ncol

    for i in range(mat_ret.nrow):
        for j in range(mat_ret.ncol):
            assert mat_ret[i,j] == pytest.approx(mat_ans[i,j], abs=1e-2)

def test_multiply_naive_big():
    np.random.seed(5555)
    a = np.random.randint(10, size=(1000, 1000))
    b = np.random.randint(10, size=(1000, 1000))

    mat_a = Matrix(a)
    mat_b = Matrix(b)

    mat_ret = multiply_naive(mat_a, mat_b)
    mat_ans = Matrix(np.matmul(a, b))

    # mat_ret.show()
    # print(np.matmul(a, b))

    assert mat_ret.nrow == mat_a.nrow
    assert mat_ret.ncol == mat_b.ncol

    for i in range(mat_ret.nrow):
        for j in range(mat_ret.ncol):
            assert mat_ret[i,j] == pytest.approx(mat_ans[i,j], abs=1e-2)

def test_multiply_tile_small():
    np.random.seed(5555)
    a = np.random.randint(10, size=(33,11))
    b = np.random.randint(10, size=(11,33))

    mat_a = Matrix(a)
    mat_b = Matrix(b)

    mat_ret = multiply_tile(mat_a, mat_b, 10)
    mat_ans = Matrix(np.matmul(a, b))

    # print()
    # print(np.matmul(a, b))
    # mat_ret.show()

    assert mat_ret.nrow == mat_a.nrow
    assert mat_ret.ncol == mat_b.ncol

    for i in range(mat_ret.nrow):
        for j in range(mat_ret.ncol):
            assert mat_ret[i,j] == pytest.approx(mat_ans[i,j], abs=1e-2)

def test_multiply_tile_big():
    np.random.seed(5555)
    a = np.random.randint(10, size=(1000, 100))
    b = np.random.randint(10, size=(100, 1000))

    mat_a = Matrix(a)
    mat_b = Matrix(b)

    mat_ret = multiply_tile(mat_a, mat_b, 16)
    mat_ans = multiply_mkl(mat_a, mat_b) #Matrix(np.matmul(a, b))

    # mat_ret.show()
    # print(np.matmul(a, b))

    assert mat_ret.nrow == mat_a.nrow
    assert mat_ret.ncol == mat_b.ncol

    for i in range(mat_ret.nrow):
        for j in range(mat_ret.ncol):
            assert mat_ret[i,j] == pytest.approx(mat_ans[i,j], abs=1e-2)


def test_multiply_mkl_small():
    # a = np.array([
    #     [1, 2],
    #     [3, 4],
    # ])
    # b = np.array([
    #     [5, 6],
    #     [7, 8],
    # ])
    np.random.seed(5555)
    a = np.random.randint(10, size=(2,2))
    b = np.random.randint(10, size=(2,2))

    mat_a = Matrix(a)
    mat_b = Matrix(b)


    mat_nai = multiply_naive(mat_a, mat_b)
    mat_ret = multiply_mkl(mat_a, mat_b)
    mat_ans = multiply_naive(mat_a, mat_b)

    # mat_nai.show()
    # mat_ret.show()
    # print(np.matmul(a, b))

    assert mat_ret.nrow == mat_a.nrow
    assert mat_ret.ncol == mat_b.ncol

    for i in range(mat_ret.nrow):
        for j in range(mat_ret.ncol):
            assert mat_ret[i,j] == pytest.approx(mat_ans[i,j], abs=1e-2)

def test_performance():

    np.random.seed(5555)
    a = np.random.randint(10, size=(1000, 1000))
    b = np.random.randint(10, size=(1000, 1000))

    mat_a = Matrix(a)
    mat_b = Matrix(b)

    naive_record = []
    tile_record = []
    mkl_record = []

    for i in range(8):
        timer_a = time.perf_counter()
        multiply_naive(mat_a, mat_b)
        naive_record.append(time.perf_counter() - timer_a)

        timer_b = time.perf_counter()
        multiply_tile(mat_a, mat_b, 16)
        tile_record.append(time.perf_counter() - timer_b)

        timer_c = time.perf_counter()
        multiply_mkl(mat_a, mat_b)
        mkl_record.append(time.perf_counter() - timer_c)


    naive_avg = sum(naive_record) / len(naive_record) 
    tile_avg = sum(tile_record) / len(tile_record)
    mkl_avg = sum(mkl_record) / len(mkl_record)
    
    with open("performance.txt", "w") as f:
        f.write("[Average execution time]\n")
        f.write("multiply_naive: {} secs\n".format("%.4f"%naive_avg))
        f.write("multiply_tile : {} secs\n".format("%.4f"%tile_avg))
        f.write("multiply_mkl  : {} secs\n".format("%.4f"%mkl_avg))
        f.write("The tiling version is {:.1%} faster than naive version\n".format(naive_avg / tile_avg))
        f.write("The mkl version is {:.1%} faster than naive version\n".format(naive_avg / mkl_avg))
        f.close()

    assert tile_avg / naive_avg <= 0.8