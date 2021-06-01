#include <pybind11/numpy.h>
#include <pybind11/operators.h>
#include <pybind11/pybind11.h>

#include <vector>

// Ref: https://github.com/yungyuc/nsd/blob/master/notebook/current/05_matrix/matrix.ipynb
class Matrix {
   public:
	size_t nrow;
	size_t ncol;

	Matrix(size_t _nrow, size_t _ncol)
		: nrow(_nrow), ncol(_ncol) {
		size_t nelement = nrow * ncol;
		m_buffer.resize(nelement);
	}

	~Matrix() = default;

	double *data() { return (double *)&m_buffer[0]; }
	// No bound check.
	double operator()(size_t row, size_t col) const { return m_buffer[row * ncol + col]; }
	double &operator()(size_t row, size_t col) { return m_buffer[row * ncol + col]; }

	bool operator==(const Matrix &rhs) const {
		for (size_t row = 0; row < nrow; row++) {
			for (size_t col = 0; col < ncol; col++) {
				if ((*this)(row, col) != rhs(row, col)) {
					return false;
				}
			}
		}
		return true;
	}

   private:
	std::vector<double> m_buffer = std::vector<double>();
};

Matrix multiply_naive(Matrix &, Matrix &);
Matrix multiply_mkl(Matrix &, Matrix &);
Matrix multiply_tile(Matrix &, Matrix &, size_t);

// Ref: https://github.com/pybind/pybind11/issues/1408#issuecomment-391121386
PYBIND11_MODULE(_matrix, m) {
	m.doc() = "_matrix";

	// Ref: https://pybind11.readthedocs.io/en/stable/classes.html
	pybind11::class_<Matrix>(m, "Matrix")
		.def(pybind11::init<size_t, size_t>())
		.def("__getitem__", [](Matrix &mat, std::pair<size_t, size_t> index) {
			return mat(index.first, index.second);
		})
		.def("__setitem__", [](Matrix &mat, std::pair<size_t, size_t> index, double val) {
			mat(index.first, index.second) = val;
		})
		// Ref: https://pybind11.readthedocs.io/en/stable/advanced/classes.html#operator-overloading
		.def(pybind11::self == pybind11::self)
		// Ref: https://pybind11.readthedocs.io/en/stable/classes.html#instance-and-static-fields
		.def_readonly("nrow", &Matrix::nrow)
		.def_readonly("ncol", &Matrix::ncol)
		// Ref: https://pybind11.readthedocs.io/en/stable/advanced/pycpp/numpy.html#arrays
		// Ref: https://stackoverflow.com/a/44682603/13509181
		.def_property(
			"array", [](Matrix &m) {
				return pybind11::array_t<double>(
					{m.nrow, m.ncol},
					{sizeof(double) * m.ncol, sizeof(double)},
					m.data(),
					pybind11::cast(m));
			},
			nullptr);

	m.def("multiply_naive", &multiply_naive);
	m.def("multiply_tile", &multiply_tile);
	m.def("multiply_mkl", &multiply_mkl);
}
