#include <iostream>
#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <mkl.h>
#include <assert.h>

namespace py = pybind11;

class Matrix {
   public:
        size_t nrow;
        size_t ncol;
        double* m_buffer;

        Matrix(size_t _nrow, size_t _ncol)
                : nrow(_nrow), ncol(_ncol) {
                size_t nelement = nrow * ncol;
                m_buffer = new double[nelement];
                memset(m_buffer, 0, nelement * sizeof(double));
        }


        ~Matrix() {}

        double* data() const { return m_buffer; }
        double operator()(size_t row, size_t col) const { return m_buffer[row * ncol + col]; }
        double& operator()(size_t row, size_t col) { return m_buffer[row * ncol + col]; }

        size_t get_nrow() const { return nrow; }
        size_t get_ncol() const { return ncol; }


        bool operator==(const Matrix& rhs) const {
                for (size_t row = 0; row < nrow; row++) {
                        for (size_t col = 0; col < ncol; col++) {
                                if ((*this)(row, col) != rhs(row, col)) {
                                        return false;
                                }
                        }
                }
                return true;
        }
};

Matrix multiply_naive(Matrix& mat_1, Matrix& mat_2) {
	assert (mat_1.nrow == mat_2.ncol);

        Matrix res(mat_1.nrow, mat_2.ncol);
        for (size_t row = 0; row < mat_1.nrow; row++) {
                for (size_t col = 0; col < mat_2.ncol; col++) {
                        for (size_t i = 0; i < mat_1.ncol; i++) {
                                res(row, col) += mat_1(row, i) * mat_2(i, col);
                        }
                }
        }
        return res;
}

Matrix multiply_tile(Matrix& mat_1, Matrix& mat_2, size_t block_size) {
	assert (mat_1.nrow == mat_2.ncol);

	Matrix res(mat_1.nrow, mat_2.ncol);

// Not working code
/*
	for (size_t i0 = 0; i0 < mat_1.nrow; i0 += block_size) {
        	size_t imax = i0 + block_size > mat_1.nrow ? mat_1.nrow : i0 + block_size;

        	for (size_t j0 = 0; j0 < mat_2.ncol; j0 += block_size) {
            		size_t jmax = j0 + block_size > mat_2.ncol ? mat_2.ncol : j0 + block_size;

			for (size_t i = 0; i < mat_1.ncol; i++) {
				for (size_t row = i0; i0 < imax; row++) {
                                        for (size_t col = j0; j0 < jmax; col++) {
                                                res(row, col) += mat_1(row, i) * mat_2(i, col);
                        }
                    }
                }
            }
        }
*/
	return res;
}

Matrix multiply_mkl(Matrix& mat_1, Matrix& mat_2) {
        assert (mat_1.nrow == mat_2.ncol);

        double* C = new double[mat_1.nrow * mat_2.ncol];

        int m = mat_1.nrow;
        int k = mat_1.ncol;
        int n = mat_2.ncol;
        double alpha = 1.0, beta = 0.0;

        cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans,
                                m, n, k, alpha, mat_1.data(), k, mat_2.data(), n, beta, C, n);

        Matrix res(mat_1.nrow, mat_2.ncol);
        memcpy(res.data(), C, res.get_nrow() * res.get_ncol() * sizeof(double));

        return res;
}

PYBIND11_MODULE(_matrix, m) {
        m.def("multiply_naive", &multiply_naive, "naive matrix-matrix multiplication");
        m.def("multiply_tile", &multiply_tile, "matrix-matrix multiplication with tiling");
        m.def("multiply_mkl", &multiply_mkl, "uses DGEMM for matrix-matrix multiplication");
    
        py::class_<Matrix>(m, "Matrix")
                .def(pybind11::init<size_t, size_t>())
                .def("__getitem__", [](Matrix& mat, std::pair<size_t, size_t> index) {
                        return mat(index.first, index.second);
                })
                .def("__setitem__", [](Matrix& mat, std::pair<size_t, size_t> index, double val) {
                        mat(index.first, index.second) = val;
                })
                .def(py::self == py::self)
                .def_readonly("nrow", &Matrix::nrow)
                .def_readonly("ncol", &Matrix::ncol);
}

