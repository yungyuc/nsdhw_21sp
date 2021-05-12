/*
 * Copyright (c) 2020, Yung-Yu Chen <yyc@solvcon.net>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice,
 *   this list of conditions and the following disclaimer.
 * - Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 * - Neither the name of the copyright holder nor the names of its contributors
 *   may be used to endorse or promote products derived from this software
 *   without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <pybind11/pybind11.h> // Must be the first include.
#include <pybind11/attr.h>
#include <pybind11/numpy.h>
#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
#include <numpy/arrayobject.h>

#include <atomic>
#include <iostream>
#include <type_traits>

#include "_matrix.hpp"

#ifdef __GNUG__
#  define PYTHON_WRAPPER_VISIBILITY __attribute__((visibility("hidden")))
#else
#  define PYTHON_WRAPPER_VISIBILITY
#endif

#include "_matrix.hpp"
#include <mkl.h>

#define COL_MAJOR
#define MIN(a,b) (((a)<(b))?(a):(b))

Matrix multiply_naive (const Matrix &left, const Matrix& right) 
{
    if (left.ncol() != right.nrow()) {
        throw std::out_of_range("Error Size");
    }

    Matrix ret(left.nrow(), right.ncol());

    for(size_t i = 0; i < ret.nrow(); ++i) {
        for (size_t j = 0; j < ret.ncol(); ++j) {
            double v = 0;
            for(size_t k = 0; k < left.ncol(); ++k) {
                v += left(i, k) * right(k, j);
            }
            ret(i, j) = v;
        }
    }

    return ret;
}

// buggy when def_static, use explicitly defined function instead
// perhaps due to the life cycle? (Not sure)
// https://github.com/pybind/pybind11/issues/1598
Matrix multiply_mkl (const Matrix &left, const Matrix& right) 
{
    if (left.ncol() != right.nrow()) {
        throw std::out_of_range("Error Size");
    }
    
    Matrix ret(left.nrow(), right.ncol());

    // Ref: https://software.intel.com/en-us/mkl-tutorial-c-multiplying-matrices-using-dgemm

    int m = left.nrow();
    int k = left.ncol();
    int n = right.ncol();

    #ifdef ROW_MAJOR
        cblas_dgemm(        
            CblasRowMajor,        
            CblasNoTrans,   //
            CblasNoTrans,   //     
            m,              // M
            n,              // N
            k,              // K
            1.0,            // ALPHA,
            left.data(),    // A
            left.ncol(),    // LDA,
            right.data(),   // B
            right.ncol(),   // LDB
            0.0,            // BETA,
            ret.data(),     // C
            ret.ncol()      // LDC
        );
    #else
        cblas_dgemm(        
            CblasColMajor,        
            CblasNoTrans,   //
            CblasNoTrans,   //     
            m,              // M
            n,              // N
            k,              // K
            1.0,            // ALPHA,
            left.data(),    // A
            left.nrow(),    // LDA,
            right.data(),   // B
            right.nrow(),   // LDB        
            0.0,            // BETA,
            ret.data(),     // C
            ret.nrow()      // LDC
        );
    #endif
    return ret;
}

Matrix multiply_tile (Matrix &left, Matrix& right, size_t tsize) 
{
    const size_t m = left.nrow();
    const size_t n = right.ncol();
    const size_t p = left.ncol();
    
    if (p != right.nrow()) {
        throw std::out_of_range("Error Size");
    }    

    // tiling size
    const size_t left_row = tsize;
    const size_t left_col = tsize;
    const size_t right_col = tsize;

    Matrix ret(m, n);
    
    // use cache to speedup transpose()
    /*Matrix T(right.ncol(), right.nrow());
    constexpr size_t tblock = 64 / sizeof(double);
    const auto x = right.nrow();
    for (size_t i = 0; i < n; i += tblock) {
        for(size_t j = 0; j < x; ++j) {
            for(size_t b = 0; b < tblock && i + b < n; ++b) {
                T(j, i + b) = right(i + b, j);
            }
        }
    }*/

    for (size_t k = 0; k < p; k += left_col) {
        const size_t tile_k_bound = MIN(k + left_col, p);
        for(size_t i = 0; i < m; i += left_row) {
            const size_t tile_i_bound = MIN(i + left_row, m);
            for (size_t j = 0; j < n; j += right_col) {
                const size_t tile_j_bound = MIN(j + right_col, n);
                for (size_t tile_k = k; tile_k < tile_k_bound; ++tile_k) {                    
                    for (size_t tile_i = i; tile_i < tile_i_bound; ++tile_i) {
                        const double r = left(tile_i, tile_k);                        
                        for (size_t tile_j = j; tile_j < tile_j_bound; ++tile_j) {        
                            ret(tile_i, tile_j) += r * right(tile_k, tile_j); // tiling version
                            
                            // Ref: https://edisonx.pixnet.net/blog/post/91005914
                            // ret(tile_i, tile_j) += r * T(tile_j, tile_k); // transpose version
                        }
                    }
                }
            }
        }
    }

    return ret;
}

namespace python
{
    struct calltag {};
} /* end namespace python */

namespace pybind11
{
    namespace detail
    {
        template<> 
        struct process_attribute<python::calltag> : process_attribute_default<python::calltag>
        {
            static void postcall(function_call & call, handle &) 
            {
                mark(call);
            }

        private:

            static void mark(function_call const & call)
            {
                object o = module::import("_matrix");
                std::string name = std::string(str(call.func.scope.attr("__name__")));
                name += std::string(call.func.name) + "_post_count";
                const size_t count = getattr(o, name.c_str(), int_(0)).cast<size_t>() + 1;
                setattr(o, name.c_str(), int_(count));
            }

        };

    } /* end namespace detail */
} /* end namespace pybind11 */

namespace python
{

    /**
     * Helper template for pybind11 class wrappers.
     */
    template<class Wrapper, class Wrapped, class Holder = std::unique_ptr<Wrapped> , class WrappedBase = Wrapped>
    /*
    * Use CRTP to detect type error during compile time.
    */
    class
    PYTHON_WRAPPER_VISIBILITY
    WrapBase
    {

        public:

            using wrapper_type = Wrapper;
            using wrapped_type = Wrapped;
            using wrapped_base_type = WrappedBase;
            using holder_type = Holder;
            using root_base_type = WrapBase
            <
                wrapper_type
            , wrapped_type
            , holder_type
            , wrapped_base_type
            >;
            using class_ = typename std::conditional_t <
                std::is_same< Wrapped, WrappedBase >::value, // if
                pybind11::class_< wrapped_type, holder_type >, // return int
                pybind11::class_< wrapped_type, wrapped_base_type, holder_type > // else
            >;

            static wrapper_type & commit(pybind11::module & mod)
            {
                static wrapper_type derived(mod);
                return derived;
            }

            static wrapper_type & commit(pybind11::module & mod, char const * pyname, char const * pydoc)
            {
                static wrapper_type derived(mod, pyname, pydoc);
                return derived;
            }

            WrapBase() = delete;
            WrapBase(WrapBase const & ) = default;
            WrapBase(WrapBase       &&) = delete;
            WrapBase & operator=(WrapBase const & ) = default;
            WrapBase & operator=(WrapBase       &&) = delete;
            ~WrapBase() = default;

        #define DECL_PYBIND_CLASS_METHOD_UNTAGGED(METHOD) \
            template< class... Args > \
            /* NOLINTNEXTLINE(bugprone-macro-parentheses) */ \
            wrapper_type & METHOD(Args&&... args) \
            { \
                m_cls.METHOD(std::forward<Args>(args)...); \
                return *static_cast<wrapper_type*>(this); \
            }

        #define DECL_PYBIND_CLASS_METHOD_TAGGED(METHOD) \
            template< class... Args > \
            /* NOLINTNEXTLINE(bugprone-macro-parentheses) */ \
            wrapper_type & METHOD ## _tagged(Args&&... args) \
            { \
                m_cls.METHOD(std::forward<Args>(args)..., calltag()); \
                return *static_cast<wrapper_type*>(this); \
            }

        #define DECL_PYBIND_CLASS_METHOD(METHOD) \
            DECL_PYBIND_CLASS_METHOD_UNTAGGED(METHOD) \
            DECL_PYBIND_CLASS_METHOD_TAGGED(METHOD)

            DECL_PYBIND_CLASS_METHOD(def)
            DECL_PYBIND_CLASS_METHOD(def_static)

            DECL_PYBIND_CLASS_METHOD(def_readwrite)
            DECL_PYBIND_CLASS_METHOD(def_readonly)
            DECL_PYBIND_CLASS_METHOD(def_readwrite_static)
            DECL_PYBIND_CLASS_METHOD(def_readonly_static)

            DECL_PYBIND_CLASS_METHOD(def_property)
            DECL_PYBIND_CLASS_METHOD(def_property_static)
            DECL_PYBIND_CLASS_METHOD(def_property_readonly)
            DECL_PYBIND_CLASS_METHOD(def_property_readonly_static)

        #undef DECL_PYBIND_CLASS_METHOD
        #undef DECL_PYBIND_CLASS_METHOD_TAGGED
        #undef DECL_PYBIND_CLASS_METHOD_UNTAGGED

            class_ & cls() { return m_cls; }

        protected:

            template <typename... Extra>
            WrapBase(pybind11::module & mod, char const * pyname, char const * pydoc, const Extra & ... extra)
            : m_cls(mod, pyname, pydoc, extra ...)
            {}

        private:

            class_ m_cls;

    }; /* end class WrapBase */

    /**
     * Wrapper for the Matrix class.
     */
    class
    PYTHON_WRAPPER_VISIBILITY
    WrapMatrix : public WrapBase< WrapMatrix, Matrix >
    {
        friend root_base_type;

        WrapMatrix(pybind11::module & mod, char const * pyname, char const * pydoc)
        : root_base_type(mod, pyname, pydoc, pybind11::buffer_protocol())
        {
            namespace py = pybind11;

            (*this)
                // The Python constructor will be counted!
                .def_tagged(py::init<size_t, size_t>())
                .def_property_readonly("nrow", &Matrix::nrow)
                .def_property_readonly("ncol", &Matrix::nrow)
                // .def("__eq__", [](Matrix const & self, Matrix const & other) { return self == other; })

                .def("__eq__", [](const Matrix &m1, const Matrix &m2){
                    if (m1.ncol() != m2.ncol() || m1.nrow() != m2.nrow()) {
                        return false;
                    }
                    
                    for(size_t i = 0; i < m1.ncol(); ++i) {
                        for(size_t j = 0; j < m1.nrow(); ++j) {
                            if ( m1(j, i) != m2(j, i)) {
                                return false;
                            }
                        }
                    }
                    return true;
                })

                .def
                (
                    "__getitem__"
                , [](Matrix const & self, std::tuple<size_t, size_t> idx)
                    { return self(std::get<0>(idx), std::get<1>(idx)); }
                )
                .def
                (
                    "__setitem__"
                , [](Matrix & self, std::tuple<size_t, size_t> idx, double value)
                    { return self(std::get<0>(idx), std::get<1>(idx)) = value; }
                )
            ;

            mod.def("multiply_mkl", &multiply_mkl);
            mod.def("multiply_naive", &multiply_naive);
            mod.def("multiply_tile", &multiply_tile);

        }

    }; /* end class WrapMatrix */
} /* end namespace python */

PYBIND11_MODULE(_matrix, mod)
{
    python::WrapMatrix::commit(mod, "Matrix", "Matrix");
}

// vim: set ff=unix fenc=utf8 et sw=4 ts=4 sts=4:
