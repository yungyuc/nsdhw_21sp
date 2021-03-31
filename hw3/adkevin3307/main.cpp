#include <iostream>
#include <cassert>
#include <random>
#include <functional>
#include <ctime>

#include "Matrix.h"

using namespace std;

void initialize(Matrix<int>& m)
{
    random_device device;
    mt19937 generator(device());
    uniform_int_distribution<int> distribution(-10, 10);
    auto random = std::bind(distribution, generator);

    for (size_t i = 0; i < m.rows(); i++) {
        for (size_t j = 0; j < m.cols(); j++) {
            m(i, j) = random();
        }
    }
}

void initialize(Matrix<float>& m)
{
    random_device device;
    mt19937 generator(device());
    uniform_real_distribution<float> distribution(-10, 10);
    auto random = std::bind(distribution, generator);

    for (size_t i = 0; i < m.rows(); i++) {
        for (size_t j = 0; j < m.cols(); j++) {
            m(i, j) = random();
        }
    }
}

template<typename T>
Matrix<T> multiply_naive(Matrix<T>& m1, Matrix<T>& m2)
{
    assert(m1.cols() == m2.rows());

    Matrix<T> m3(m1.rows(), m2.cols());

    size_t rows = m1.rows();
    size_t cols = m2.cols();
    size_t inners = m1.cols();

    for (size_t row = 0; row < rows; row++) {
        for (size_t col = 0; col < cols; col++) {
            for (size_t inner = 0; inner < inners; inner++) {
                m3(row, col) += m1(row, inner) * m2(inner, col);
            }
        }
    }

    return m3;
}

template<typename T, size_t size>
Matrix<T> multiply_tile(Matrix<T>& m1, Matrix<T>& m2)
{
    assert(m1.cols() == m2.rows());

    Matrix<T> m3(m1.rows(), m2.cols());

    size_t tile_rows = m1.rows() / size;
    size_t tile_cols = m2.cols() / size;
    size_t tile_inners = m1.cols() / size;

    Matrix<T> tile_m1(size, size);
    Matrix<T> tile_m2(size, size);
    Matrix<T> tile_m3(size, size);

    for (size_t row = 0; row < tile_rows; row++) {
        for (size_t col = 0; col < tile_cols; col++) {
            for (size_t inner = 0; inner < tile_inners; inner++) {
                for (size_t i = 0; i < size; i++) {
                    for (size_t j = 0; j < size; j++) {
                        tile_m1(i, j) = m1(row * size + i, inner * size + j);
                        tile_m2(i, j) = m2(inner * size + j, col * size + i);
                    }
                }

                for (size_t i = 0; i < size; i++) {
                    for (size_t j = 0; j < size; j++) {
                        for (size_t k = 0; k < size; k++) {
                            m3(row * size + i, col * size + j) += tile_m1(i, k) * tile_m2(j, k);
                        }
                    }
                }
            }
        }
    }

    return m3;
}

template<typename T>
Matrix<T> multiply_mkl(Matrix<T>& m1, Matrix<T>& m2)
{
}

int main()
{
    Matrix<int> m1(1000, 1000);
    Matrix<int> m2(1000, 1000);

    initialize(m1);
    initialize(m2);

    // cout << m1 << '\n';
    // cout << m2 << '\n';

    clock_t start, stop;

    start = clock();
    Matrix<int> m3_naive = multiply_naive<int>(m1, m2);
    stop = clock();
    cout << "naive: " << (stop - start) << '\n';

    start = clock();
    Matrix<int> m3_tile_2 = multiply_tile<int, 2>(m1, m2);
    stop = clock();
    cout << "tile 2: " << (stop - start) << '\n';

    start = clock();
    Matrix<int> m3_tile_4 = multiply_tile<int, 4>(m1, m2);
    stop = clock();
    cout << "tile 4: " << (stop - start) << '\n';

    start = clock();
    Matrix<int> m3_tile_8 = multiply_tile<int, 8>(m1, m2);
    stop = clock();
    cout << "tile 8: " << (stop - start) << '\n';

    start = clock();
    Matrix<int> m3_tile_16 = multiply_tile<int, 16>(m1, m2);
    stop = clock();
    cout << "tile 16: " << (stop - start) << '\n';

    start = clock();
    Matrix<int> m3_tile_32 = multiply_tile<int, 32>(m1, m2);
    stop = clock();
    cout << "tile 32: " << (stop - start) << '\n';

    start = clock();
    Matrix<int> m3_tile_64 = multiply_tile<int, 64>(m1, m2);
    stop = clock();
    cout << "tile 64: " << (stop - start) << '\n';

    // cout << m3_naive << '\n';
    // cout << m3_tile << '\n';
}
