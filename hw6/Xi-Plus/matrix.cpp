#include <mkl.h>

#include "matrix.hpp"

Matrix multiply_naive(Matrix& ma, Matrix& mb) {
	Matrix res(ma.nrow, mb.ncol);
	for (size_t row = 0; row < ma.nrow; row++) {
		for (size_t col = 0; col < mb.ncol; col++) {
			for (size_t i = 0; i < ma.ncol; i++) {
				res(row, col) += ma(row, i) * mb(i, col);
			}
		}
	}

	return res;
}

Matrix multiply_tile(Matrix& ma, Matrix& mb, size_t tsize) {
	Matrix res(ma.nrow, mb.ncol);

	for (size_t rowL = 0; rowL < ma.nrow; rowL += tsize) {
		size_t rowR = rowL + tsize;
		rowR = rowR > ma.nrow ? ma.nrow : rowR;	 // min

		for (size_t colL = 0; colL < mb.ncol; colL += tsize) {
			size_t colR = colL + tsize;
			colR = colR > mb.ncol ? mb.ncol : colR;	 // min

			for (size_t i = 0; i < ma.ncol; i++) {
				for (size_t row = rowL; row < rowR; row++) {
					for (size_t col = colL; col < colR; col++) {
						res(row, col) += ma(row, i) * mb(i, col);
					}
				}
			}
		}
	}

	return res;
}

Matrix multiply_mkl(Matrix& A, Matrix& B) {
	int m = A.nrow;
	int k = A.ncol;
	int n = B.ncol;
	double alpha = 1.0, beta = 0.0;

	Matrix res(A.nrow, B.ncol);

	// Ref: https://software.intel.com/content/www/us/en/develop/documentation/mkl-tutorial-c/top/multiplying-matrices-using-dgemm.html
	cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans,
				m, n, k, alpha, A.data(), k, B.data(), n, beta, res.data(), n);

	return res;
}
