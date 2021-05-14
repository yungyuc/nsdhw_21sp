#include <vector>

// Ref: https://github.com/yungyuc/nsd/blob/master/notebook/current/05_matrix/matrix.ipynb
class Matrix {
   public:
	size_t m_nrow;
	size_t m_ncol;

	Matrix(size_t _nrow, size_t _ncol)
		: m_nrow(_nrow), m_ncol(_ncol) {
		size_t nelement = m_nrow * m_ncol;
		m_buffer.resize(nelement);
	}

	~Matrix() = default;

	double *data() const { return (double *)&m_buffer[0]; }
	// No bound check.
	double operator()(size_t row, size_t col) const { return m_buffer[row * m_ncol + col]; }
	double &operator()(size_t row, size_t col) { return m_buffer[row * m_ncol + col]; }

	bool operator==(const Matrix &rhs) const {
		for (size_t row = 0; row < m_nrow; row++) {
			for (size_t col = 0; col < m_ncol; col++) {
				if ((*this)(row, col) != rhs(row, col)) {
					return false;
				}
			}
		}
		return true;
	}

	size_t nrow() const { return m_nrow; }
	size_t ncol() const { return m_ncol; }

   private:
	std::vector<double> m_buffer = std::vector<double>();
};

Matrix multiply_naive(Matrix &, Matrix &);
Matrix multiply_mkl(Matrix &, Matrix &);
Matrix multiply_tile(Matrix &, Matrix &, size_t);
