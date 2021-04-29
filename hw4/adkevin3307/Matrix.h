#pragma once

#include <iostream>
#include <vector>
#include <limits>

class ByteCounter {
private:
    std::size_t m_allocated;
    std::size_t m_deallocated;

public:
    ByteCounter()
        : m_allocated(0), m_deallocated(0)
    {
    }

    ByteCounter(const ByteCounter& rhs)
        : m_allocated(rhs.m_allocated), m_deallocated(rhs.m_deallocated)
    {
    }

    ByteCounter(ByteCounter&& rhs)
        : m_allocated(rhs.m_allocated), m_deallocated(rhs.m_deallocated)
    {
    }

    ~ByteCounter()
    {
    }

    ByteCounter& operator=(const ByteCounter& rhs) = delete;
    ByteCounter& operator=(ByteCounter&& rhs) = delete;

    void increase(std::size_t n)
    {
        this->m_allocated += n;
    }

    void decrease(std::size_t n)
    {
        this->m_deallocated += n;
    }

    const std::size_t bytes() const
    {
        return (this->m_allocated - this->m_deallocated);
    }

    const std::size_t allocated() const
    {
        return this->m_allocated;
    }

    const std::size_t deallocated() const
    {
        return this->m_deallocated;
    }
};

template<class T>
class Allocator {
private:
    ByteCounter m_counter;

public:
    Allocator() = default;

    template<class U>
    Allocator(const Allocator<U>& rhs)
        : m_counter(rhs.m_counter)
    {
    }

    ~Allocator()
    {
    }

    T* allocate(std::size_t n)
    {
        if (n > std::numeric_limits<std::size_t>::max() / sizeof(T)) {
            throw std::bad_alloc();
        }

        const std::size_t bytes = n * sizeof(T);
        T* ptr = static_cast<T*>(std::malloc(bytes));

        if (!ptr) {
            throw std::bad_alloc();
        }

        this->m_counter.increase(bytes);

        return ptr;
    }

    void deallocate(T* ptr, std::size_t n)
    {
        std::free(ptr);

        this->m_counter.decrease(n * sizeof(T));
    }
};

template<typename T>
class Matrix {
private:
    size_t m_rows, m_cols;
    Allocator<T> m_allocator;
    std::vector<T, Allocator<T>> m_matrix;

public:
    Matrix()
        : m_rows(0), m_cols(0)
    {
        this->m_matrix = std::vector<T, Allocator<T>>(this->m_allocator);
    }

    Matrix(size_t rows, size_t cols)
        : m_rows(rows), m_cols(cols)
    {
        this->m_matrix.resize(this->m_rows * this->m_cols, 0);
    }

    Matrix(Matrix const& rhs)
        : m_rows(rhs.m_rows), m_cols(rhs.m_cols), m_matrix(rhs.m_matrix)
    {
    }

    Matrix(Matrix&& rhs)
        : m_rows(rhs.m_rows), m_cols(rhs.m_cols), m_matrix(std::move(rhs.m_matrix))
    {
    }

    ~Matrix()
    {
        this->m_matrix.clear();
        this->m_matrix.shrink_to_fit();
    }

    Matrix& operator=(Matrix const& rhs)
    {
        if (this != &rhs) {
            this->m_rows = rhs.m_rows;
            this->m_cols = rhs.m_cols;
            this->m_matrix = rhs.m_matrix;
        }

        return *this;
    }

    Matrix& operator=(Matrix&& rhs)
    {
        if (this != &rhs) {
            this->m_rows = rhs.m_rows;
            this->m_cols = rhs.m_cols;
            this->m_matrix = std::move(rhs.m_matrix);
        }

        return *this;
    }

    T const& operator()(size_t i, size_t j) const
    {
        return this->m_matrix[i * this->m_cols + j];
    }

    T& operator()(size_t i, size_t j)
    {
        return this->m_matrix[i * this->m_cols + j];
    }

    const T* data() const
    {
        return &(this->m_matrix[0]);
    }

    T* data()
    {
        return &(this->m_matrix[0]);
    }

    constexpr bool operator==(const Matrix& rhs) const
    {
        return (this->m_matrix == rhs.m_matrix);
    }

    constexpr size_t rows() const
    {
        return this->m_rows;
    }

    constexpr size_t cols() const
    {
        return this->m_cols;
    }

    friend std::ostream& operator<<(std::ostream& os, const Matrix<T>& matrix)
    {
        for (size_t i = 0; i < matrix.rows(); i++) {
            for (size_t j = 0; j < matrix.cols(); j++) {
                os << matrix(i, j) << (j == matrix.cols() - 1 ? "" : " ");
            }

            if (i < matrix.rows() - 1) os << '\n';
        }

        return os;
    }
};
