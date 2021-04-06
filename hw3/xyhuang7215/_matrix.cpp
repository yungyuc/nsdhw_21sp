#include <vector>
#include <cblas.h>
#include <malloc.h>
#include <iostream>
#include <algorithm>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/operators.h>

namespace py = pybind11;

class Matrix {
    public:
        Matrix(int, int);
        Matrix(int m, int n, double val);
        void getTile(Matrix &, int, int);
        void getTileTrans(Matrix &, int, int);
        void addBlockByTile(const Matrix &, int, int) ;
        Matrix operator+=(const Matrix &);
        bool operator==(const Matrix &) const;
        std::vector<double> & operator[](int);

        void show(){
            for(int i = 0; i < nrow; ++i){
                for(int j = 0; j < ncol; ++j){
                    std::cout << mat[i][j] << " ";
                }
                std::cout << std::endl;
            }
            std::cout << std::endl;
        }

        int nrow, ncol;
        std::vector<std::vector<double>> mat;
};


Matrix::Matrix(int m, int n){
    nrow = m;
    ncol = n;
    mat.resize(m);
    for(int i = 0; i < m; ++i)
        mat[i].resize(n);
}

Matrix::Matrix(int m, int n, double val){
    nrow = m;
    ncol = n;
    mat.resize(m);
    for(int i = 0; i < m; ++i)
        mat[i].resize(n, val);
}


void Matrix::getTile(Matrix & tile, int iOffset, int jOffset){
    for(int i = 0; i < tile.nrow; ++i)
        for(int j = 0; j < tile.ncol; ++j)
            tile.mat[i][j] = mat[ i + iOffset ][ j + jOffset ];
}


void Matrix::getTileTrans(Matrix & tile, int iOffset, int jOffset){
    for(int i = 0; i < tile.nrow; ++i)
        for(int j = 0; j < tile.ncol; ++j)
            tile.mat[i][j] = mat[ j + iOffset ][ i + jOffset ];
};


void Matrix::addBlockByTile(const Matrix & tile, int iOffset, int jOffset){
    for(int i = 0; i < tile.nrow; ++i)
        for(int j = 0; j < tile.ncol; ++j)
            mat[ i + iOffset ][ j + jOffset ] += tile.mat[i][j];
}

Matrix Matrix::operator+=(const Matrix & M){
    for(int i = 0; i < nrow; ++i)
        for(int j = 0; j < ncol; ++j)
            mat[i][j] += M.mat[i][j];
    return *this;
}

bool Matrix::operator==(const Matrix & M) const{
    bool equal = true;

    if(ncol != M.ncol || nrow != M.nrow){
        equal = false;
        return equal;
    }

    for(int i = 0; i < nrow; ++i)
        for(int j = 0; j < ncol; ++j)
            if(mat[i][j] != M.mat[i][j]){
                equal = false;
                break;
            }

    return equal;
}

std::vector<double> & Matrix::operator[](int m){
    return mat[m];
}



double* flatten(Matrix A){
    double *vec = (double*) malloc(sizeof(double) * A.nrow * A.ncol);
    for(int i = 0; i < A.nrow; ++i)
        for(int j = 0; j < A.ncol; ++j)
            vec[i * A.ncol + j] = A[i][j];
    return vec;
}


Matrix reshape(double* vec, int nrow, int ncol){
    Matrix mat(nrow, ncol);
    for(int i = 0; i < nrow; ++i)
        for(int j = 0; j < ncol; ++j)
            mat[i][j] = vec[i * ncol + j];
    return mat;
}


Matrix multiply_Btranspose(Matrix& A, Matrix& B){
    int m = A.nrow, n = B.nrow;
    Matrix out(m, n, 0);

    for(int i = 0; i < m; ++i)
        for(int j = 0; j < n; ++j)
            for(int k = 0; k < A.ncol; ++k)
                out[i][j] += A[i][k] * B[j][k];
        
    return out;
}



Matrix multiply_naive(Matrix Mat1, Matrix Mat2){
    int m = Mat1.nrow, n = Mat2.ncol;
    Matrix out(m, n, 0);

    for(int i = 0; i < m; ++i)
        for(int j = 0; j < n; ++j)
            for(int k = 0; k < Mat1.ncol; ++k)
                out[i][j] += Mat1[i][k] * Mat2[k][j];
        
    return out;
}


Matrix multiply_tile(Matrix & Mat1, Matrix & Mat2, int Bsize){    
    int m = Mat1.nrow, n = Mat2.ncol;
    Matrix out(m, n, 0);

    for(int i = 0; i < m; i += Bsize){
        int nrow = std::min(Bsize, m - i);
        for(int j = 0; j < n; j += Bsize){
            int ncol = std::min(Bsize, n - j);
            for(int k = 0; k < m; k += Bsize){
                int nmid = std::min(Bsize, m - k);
                Matrix A = Matrix(nrow, nmid);
                Matrix B = Matrix(ncol, nmid);
                Mat1.getTile(A, i, k);
                Mat2.getTileTrans(B, k, j);
                out.addBlockByTile(multiply_Btranspose(A, B),  i, j);
            }
        }
    }
    
    
    return out;
}



Matrix multiply_mkl(Matrix Mat1, Matrix Mat2){
    Matrix out(Mat1.nrow, Mat2.ncol);

    double* A = flatten(Mat1);
    double* B = flatten(Mat2);
    double* C = flatten(out);

    cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, Mat1.nrow, Mat2.ncol, Mat1.nrow, 1, 
                A, Mat1.nrow, 
                B, Mat2.nrow, 
                0, C, Mat1.nrow);

    out = reshape(C, Mat1.nrow, Mat2.nrow);

    free(A);
    free(B);
    free(C);

    return out;
}


PYBIND11_MODULE(_matrix, m) {
    m.doc() = "pybind11 example plugin"; // optional module docstring

    py::class_<Matrix>(m, "Matrix")
        .def(py::init<int, int>())
        .def_readwrite("nrow", &Matrix::nrow)
        .def_readwrite("ncol", &Matrix::ncol)
        .def(pybind11::self == pybind11::self)
        .def("__getitem__", [](Matrix &m, const int row) {
            return m.mat[row];
        })
        .def("__getitem__", [](Matrix &m, const std::pair<size_t, size_t> idx) {
            return m.mat[idx.first][idx.second];
        })
        .def("__setitem__", [](Matrix &m, const std::pair<size_t, size_t> idx, const double val) {
            m.mat[idx.first][idx.second] = val;
        });
        
    m.def("multiply_naive", &multiply_naive, "");
    m.def("multiply_tile", &multiply_tile, "");
    m.def("multiply_mkl", &multiply_mkl, "");
}
