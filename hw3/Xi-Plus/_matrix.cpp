// Ref: https://pybind11.readthedocs.io/en/stable/basics.html#creating-bindings-for-a-simple-function
#include <mkl.h>
#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>

#include <iostream>
#include <string>

// Ref: https://github.com/yungyuc/nsd/blob/master/notebook/current/05_matrix/matrix.ipynb
class Matrix {
   public:
	Matrix(size_t nrow, size_t ncol)
		: m_nrow(nrow), m_ncol(ncol) {
		size_t nelement = nrow * ncol;
		m_buffer = new double[nelement];
		memset(m_buffer, 0, nrow * ncol * sizeof(double));
	}

	// ~Matrix() {
	// 	delete[] m_buffer;
	// }

	// No bound check.
	double* data() const { return m_buffer; }
	double operator()(size_t row, size_t col) const { return m_buffer[row * m_ncol + col]; }
	double& operator()(size_t row, size_t col) { return m_buffer[row * m_ncol + col]; }

	// Ref: https://pybind11.readthedocs.io/en/stable/advanced/pycpp/numpy.html
	void load(pybind11::array_t<double> input) {
		pybind11::buffer_info buf = input.request();

		memcpy(m_buffer, buf.ptr, m_nrow * m_ncol * sizeof(double));
		// for (size_t i = 0; i < size; i++) {
		// 	m_buffer[i] = buf[i];
		// }
	}

	void load2(double* input) {
		memcpy(m_buffer, input, m_nrow * m_ncol * sizeof(double));
	}

	std::string asstring() const {
		std::stringstream ss;
		ss << "[";
		for (size_t row = 0; row < m_nrow; row++) {
			if (row > 0) {
				ss << " ";
			}

			ss << "[";
			for (size_t col = 0; col < m_ncol; col++) {
				ss << (*this)(row, col) << " ";
			}
			ss << "]";
			if (row < m_nrow - 1) {
				ss << std::endl;
			}
		}
		ss << "]";

		return ss.str();
	}

	size_t nrow() const { return m_nrow; }
	size_t ncol() const { return m_ncol; }

   private:
	size_t m_nrow;
	size_t m_ncol;
	double* m_buffer;
};

Matrix multiply_naive(Matrix& ma, Matrix& mb) {
	if (ma.ncol() != mb.nrow()) {
		throw pybind11::value_error("Given matrices cannot be multiplied");
	}

	Matrix res(ma.nrow(), mb.ncol());
	for (size_t row = 0; row < ma.nrow(); row++) {
		for (size_t col = 0; col < mb.ncol(); col++) {
			for (size_t i = 0; i < ma.ncol(); i++) {
				res(row, col) += ma(row, i) * mb(i, col);
			}
		}
	}

	return res;
}

Matrix multiple_tile(Matrix& ma, Matrix& mb, size_t tsize) {
	if (ma.ncol() != mb.nrow()) {
		throw pybind11::value_error("Given matrices cannot be multiplied");
	}

	Matrix res(ma.nrow(), mb.ncol());

	for (size_t rowL = 0; rowL < ma.nrow(); rowL += tsize) {
		size_t rowR = rowL + tsize;
		rowR = rowR > ma.nrow() ? ma.nrow() : rowR;	 // min

		// for (size_t iL = 0; iL < ma.ncol(); iL += tsize) {
		// 	size_t iR = iL + tsize;
		// 	iR = iR > ma.ncol() ? ma.ncol() : iR;  // min

		for (size_t colL = 0; colL < mb.ncol(); colL += tsize) {
			size_t colR = colL + tsize;
			colR = colR > mb.ncol() ? mb.ncol() : colR;	 // min

			// for (size_t i = iL; i < iR; i++) {
			for (size_t i = 0; i < ma.ncol(); i++) {
				for (size_t row = rowL; row < rowR; row++) {
					for (size_t col = colL; col < colR; col++) {
						res(row, col) += ma(row, i) * mb(i, col);
					}
				}
			}
			// }
		}
	}

	return res;
}

Matrix multiply_mkl(Matrix& A, Matrix& B) {
	if (A.ncol() != B.nrow()) {
		throw pybind11::value_error("Given matrices cannot be multiplied");
	}

	// double* A = new double[ma.nrow() * ma.ncol()];
	// double* B = new double[mb.nrow() * mb.ncol()];
	double* C = new double[A.nrow() * B.ncol()];

	int m = A.nrow();
	int k = A.ncol();
	int n = B.ncol();
	double alpha = 1.0, beta = 0.0;

	cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans,
				m, n, k, alpha, A.data(), k, B.data(), n, beta, C, n);

	Matrix res(A.nrow(), B.ncol());
	res.load2(C);

	delete[] C;

	return res;
}

// Ref: https://github.com/pybind/pybind11/issues/1408#issuecomment-391121386
PYBIND11_MODULE(_matrix, m) {
	m.doc() = "_matrix";

	// Ref: https://pybind11.readthedocs.io/en/stable/classes.html
	pybind11::class_<Matrix>(m, "Matrix")
		.def(pybind11::init<size_t, size_t>())
		.def("__getitem__", [](Matrix& mat, std::pair<size_t, size_t> index) {
			return mat(index.first, index.second);
		})
		.def("__setitem__", [](Matrix& mat, std::pair<size_t, size_t> index, double val) {
			mat(index.first, index.second) = val;
		})
		.def("__str__", [](Matrix& mat) {
			return mat.asstring();
		})
		.def("load", &Matrix::load)
		.def("nrow", &Matrix::nrow)
		.def("ncol", &Matrix::ncol);

	m.def("multiply_naive", &multiply_naive);
	m.def("multiple_tile", &multiple_tile);
	m.def("multiply_mkl", &multiply_mkl);
}
