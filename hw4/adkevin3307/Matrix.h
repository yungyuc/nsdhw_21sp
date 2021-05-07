#pragma once

#include <iostream>
#include <vector>
#include <limits>

struct ByteCounterImplement {
    size_t allocated = 0;
    size_t deallocated = 0;
};

class ByteCounter {
private:
    ByteCounterImplement* m_implement;

public:
    ByteCounter()
        : m_implement(new ByteCounterImplement)
    {
    }

    ByteCounter(const ByteCounter& rhs)
        : m_implement(rhs.m_implement)
    {
    }

    ByteCounter(ByteCounter&& rhs)
        : m_implement(rhs.m_implement)
    {
    }

    ~ByteCounter()
    {
    }

    ByteCounter& operator=(const ByteCounter& rhs) = delete;
    ByteCounter& operator=(ByteCounter&& rhs) = delete;

    void increase(size_t n)
    {
        this->m_implement->allocated += n;
    }

    void decrease(size_t n)
    {
        this->m_implement->deallocated += n;
    }

    size_t bytes() const
    {
        return (this->m_implement->allocated - this->m_implement->deallocated);
    }

    size_t allocated() const
    {
        return this->m_implement->allocated;
    }

    size_t deallocated() const
    {
        return this->m_implement->deallocated;
    }

    constexpr bool operator==(const ByteCounter& rhs) const
    {
        return (this->m_implement == rhs.m_implement);
    }
};

template<class T>
class Allocator {
private:
    ByteCounter m_counter;

public:
    using value_type = T;

    Allocator() = default;

    template<class U>
    Allocator(const Allocator<U>& rhs) noexcept
        : m_counter(rhs.m_counter)
    {
    }

    ~Allocator()
    {
    }

    T* allocate(size_t n)
    {
        if (n > std::numeric_limits<size_t>::max() / sizeof(T)) {
            throw std::bad_alloc();
        }

        const size_t bytes = n * sizeof(T);
        T* ptr = static_cast<T*>(std::malloc(bytes));

        if (!ptr) {
            throw std::bad_alloc();
        }

        this->m_counter.increase(bytes);
        std::cout << "increase: " << this->m_counter.bytes() << '\n';

        return ptr;
    }

    void deallocate(T* ptr, size_t n)
    {
        std::free(ptr);

        this->m_counter.decrease(n * sizeof(T));
    }

    constexpr size_t bytes() const
    {
        return this->m_counter.bytes();
    }

    constexpr size_t allocated() const
    {
        return this->m_counter.allocated();
    }

    constexpr size_t deallocated() const
    {
        return this->m_counter.deallocated();
    }

    constexpr bool operator==(const Allocator<T>& rhs) const
    {
        return (this->m_counter == rhs.m_counter);
    }

    friend std::ostream& operator<<(std::ostream& os, const Allocator<T>& rhs)
    {
        os << "allocator: bytes = " << rhs.bytes();
        os << " allocated = " << rhs.allocated();
        os << " deallocated = " << rhs.deallocated();

        return os;
    }
};

Allocator<double> m_allocator;

constexpr size_t bytes()
{
    return m_allocator.bytes();
}

constexpr size_t allocated()
{
    return m_allocator.allocated();
}

constexpr size_t deallocated()
{
    return m_allocator.deallocated();
}

template<typename T>
class Matrix {
private:
    size_t m_rows, m_cols;
    std::vector<T, Allocator<double>> m_matrix = std::vector<T, Allocator<double>>(m_allocator);

public:
    Matrix()
        : m_rows(0), m_cols(0)
    {
        std::cout << m_allocator << '\n';
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
