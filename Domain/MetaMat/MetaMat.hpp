/**
 * @class MetaMat
 * @brief A MetaMat class that holds matrices.
 *
 * @author T
 * @date 08/09/2017
 * @version 0.2.0
 * @file MetaMat.hpp
 * @addtogroup MetaMat
 * @{
 */

#ifndef METAMAT_HPP
#define METAMAT_HPP

#include <Toolbox/debug.h>
#include <armadillo>

using namespace arma;

template <typename T> class MetaMat {
public:
    Col<int> IPIV;
    const char TRAN = 'N';
    bool factored = false;
    const unsigned n_rows;
    const unsigned n_cols;
    const unsigned n_elem;

    const T* const memory = nullptr;

    MetaMat();
    MetaMat(const unsigned&, const unsigned&, const unsigned&);
    MetaMat(const MetaMat&);
    MetaMat(MetaMat&&) noexcept;
    MetaMat& operator=(const MetaMat&);
    MetaMat& operator=(MetaMat&&) noexcept;
    virtual ~MetaMat();

    bool is_empty() const;
    void init();
    void zeros();
    void reset();

    T max() const;

    virtual const T& operator()(const uword&, const uword&) const;
    virtual T& at(const uword&, const uword&);

    const T* memptr() const;
    T* memptr();

    virtual MetaMat operator+(const MetaMat&);
    virtual MetaMat operator-(const MetaMat&);
    virtual MetaMat& operator+=(const MetaMat&);
    virtual MetaMat& operator-=(const MetaMat&);

    virtual MetaMat operator*(const T&);
    virtual Mat<T> operator*(const Mat<T>&);

    virtual MetaMat& operator*=(const T&);

    virtual Mat<T> solve(const Mat<T>&);
    virtual int solve(Mat<T>&, const Mat<T>&);

    virtual Mat<T> solve_trs(const Mat<T>&);
    virtual int solve_trs(Mat<T>&, const Mat<T>&);

    virtual MetaMat factorize();

    virtual MetaMat i();
    virtual MetaMat inv();

    virtual void print();
};

template <typename T>
MetaMat<T>::MetaMat()
    : n_rows(0)
    , n_cols(0)
    , n_elem(0) {}

template <typename T>
MetaMat<T>::MetaMat(const unsigned& in_rows, const unsigned& in_cols, const unsigned& in_elem)
    : n_rows(in_rows)
    , n_cols(in_cols)
    , n_elem(in_elem) {
    init();
    zeros();
}

template <typename T>
MetaMat<T>::MetaMat(const MetaMat& old_mat)
    : factored(old_mat.factored)
    , n_rows(old_mat.n_rows)
    , n_cols(old_mat.n_cols)
    , n_elem(old_mat.n_elem) {
    init();
    memcpy(memptr(), old_mat.memptr(), old_mat.n_elem * sizeof(T));
}

template <typename T>
MetaMat<T>::MetaMat(MetaMat&& old_mat) noexcept
    : factored(old_mat.factored)
    , n_rows(old_mat.n_rows)
    , n_cols(old_mat.n_cols)
    , n_elem(old_mat.n_elem) {
    access::rw(memory) = old_mat.memory;
    access::rw(old_mat.memory) = nullptr;
}

template <typename T> MetaMat<T>& MetaMat<T>::operator=(const MetaMat& old_mat) {
    if(this != &old_mat) {
        factored = old_mat.factored;
        access::rw(n_rows) = old_mat.n_rows;
        access::rw(n_cols) = old_mat.n_cols;
        access::rw(n_elem) = old_mat.n_elem;
        init();
        memcpy(memptr(), old_mat.memptr(), old_mat.n_elem * sizeof(T));
    }
    return *this;
}

template <typename T> MetaMat<T>& MetaMat<T>::operator=(MetaMat&& old_mat) noexcept {
    if(this != &old_mat) {
        factored = old_mat.factored;
        access::rw(n_rows) = old_mat.n_rows;
        access::rw(n_cols) = old_mat.n_cols;
        access::rw(n_elem) = old_mat.n_elem;
        access::rw(memory) = old_mat.memory;
        access::rw(old_mat.memory) = nullptr;
    }
    return *this;
}

template <typename T> MetaMat<T>::~MetaMat() {
    if(memory != nullptr) memory::release(access::rw(memory));
}

template <typename T> bool MetaMat<T>::is_empty() const { return n_elem == 0; }

template <typename T> void MetaMat<T>::init() {
    if(memory != nullptr) memory::release(access::rw(memory));
    access::rw(memory) = is_empty() ? nullptr : memory::acquire<T>(n_elem);
}

template <typename T> void MetaMat<T>::zeros() {
    arrayops::fill_zeros(memptr(), n_elem);
    factored = false;
}

template <typename T> void MetaMat<T>::reset() {
    access::rw(n_rows) = 0;
    access::rw(n_cols) = 0;
    access::rw(n_elem) = 0;
    if(memory != nullptr) memory::release(access::rw(memory));
    access::rw(memory) = nullptr;
    factored = false;
}

template <typename T> T MetaMat<T>::max() const { return op_max::direct_max(memptr(), n_elem); }

template <typename T> const T& MetaMat<T>::operator()(const uword& in_row, const uword& in_col) const { return memory[in_row + in_col * n_rows]; }

template <typename T> T& MetaMat<T>::at(const uword& in_row, const uword& in_col) { return access::rw(memory[in_row + in_col * n_rows]); }

template <typename T> const T* MetaMat<T>::memptr() const { return memory; }

template <typename T> T* MetaMat<T>::memptr() { return const_cast<T*>(memory); }

template <typename T> MetaMat<T> MetaMat<T>::operator+(const MetaMat& M) {
    auto N = *this;
    N += M;
    return N;
}

template <typename T> MetaMat<T> MetaMat<T>::operator-(const MetaMat& M) {
    auto N = *this;
    N -= M;
    return N;
}

template <typename T> MetaMat<T>& MetaMat<T>::operator+=(const MetaMat& M) {
    if(n_rows == M.n_rows && n_cols == M.n_cols && n_elem == M.n_elem) {
        arrayops::inplace_plus(memptr(), M.memptr(), n_elem);
        factored = false;
    }
    return *this;
}

template <typename T> MetaMat<T>& MetaMat<T>::operator-=(const MetaMat& M) {
    if(n_rows == M.n_rows && n_cols == M.n_cols && n_elem == M.n_elem) {
        arrayops::inplace_minus(memptr(), M.memptr(), n_elem);
        factored = false;
    }
    return *this;
}

template <typename T> MetaMat<T> MetaMat<T>::operator*(const T& value) {
    auto new_mat = *this;
    arrayops::inplace_mul(new_mat.memptr(), value, new_mat.n_elem);
    return new_mat;
}

template <typename T> Mat<T> MetaMat<T>::operator*(const Mat<T>& B) {
    auto C = B;

    if(B.n_cols == 1) {
        int M = n_rows;
        int N = n_cols;
        T ALPHA = 1.;
        auto LDA = M;
        auto INCX = 1;
        T BETA = 0.;
        auto INCY = 1;

        if(std::is_same<T, float>::value) {
            using E = float;
            arma_fortran(arma_sgemv)(&TRAN, &M, &N, (E*)&ALPHA, (E*)memptr(), &LDA, (E*)B.memptr(), &INCX, (E*)&BETA, (E*)C.memptr(), &INCY);
        } else if(std::is_same<T, double>::value) {
            using E = double;
            arma_fortran(arma_dgemv)(&TRAN, &M, &N, (E*)&ALPHA, (E*)memptr(), &LDA, (E*)B.memptr(), &INCX, (E*)&BETA, (E*)C.memptr(), &INCY);
        }
    } else {
        int M = n_rows;
        auto N = int(B.n_cols);
        int K = n_cols;
        T ALPHA = 1.;
        auto LDA = M;
        auto LDB = K;
        T BETA = 0.;
        auto LDC = M;

        if(std::is_same<T, float>::value) {
            using E = float;
            arma_fortran(arma_sgemm)(&TRAN, &TRAN, &M, &N, &K, (E*)&ALPHA, (E*)memptr(), &LDA, (E*)B.memptr(), &LDB, (E*)&BETA, (E*)C.memptr(), &LDC);
        } else if(std::is_same<T, double>::value) {
            using E = double;
            arma_fortran(arma_dgemm)(&TRAN, &TRAN, &M, &N, &K, (E*)&ALPHA, (E*)memptr(), &LDA, (E*)B.memptr(), &LDB, (E*)&BETA, (E*)C.memptr(), &LDC);
        }
    }

    return C;
}

template <typename T> MetaMat<T>& MetaMat<T>::operator*=(const T& value) {
    arrayops::inplace_mul(memptr(), value, n_elem);
    return *this;
}

template <typename T> Mat<T> MetaMat<T>::solve(const Mat<T>& B) {
    Mat<T> X;
    if(solve(X, B) != 0) X.reset();
    return X;
}

template <typename T> int MetaMat<T>::solve(Mat<T>& X, const Mat<T>& B) {
    if(factored) {
        suanpan_warning("the matrix is factored.\n");
        return solve_trs(X, B);
    }

    X = B;

    int N = n_rows;
    auto NRHS = int(B.n_cols);
    auto LDA = N;
    auto LDB = int(B.n_rows);
    IPIV.zeros(N);
    auto INFO = 0;

    if(std::is_same<T, float>::value) {
        using E = float;
        arma_fortran(arma_sgesv)(&N, &NRHS, (E*)memptr(), &LDA, IPIV.memptr(), (E*)X.memptr(), &LDB, &INFO);
    } else if(std::is_same<T, double>::value) {
        using E = double;
        arma_fortran(arma_dgesv)(&N, &NRHS, (E*)memptr(), &LDA, IPIV.memptr(), (E*)X.memptr(), &LDB, &INFO);
    }

    if(INFO != 0)
        suanpan_error("solve() receives error code %u from the base driver, the matrix is probably singular.\n", INFO);
    else
        factored = true;

    return INFO;
}

template <typename T> Mat<T> MetaMat<T>::solve_trs(const Mat<T>& B) {
    Mat<T> X;
    if(solve_trs(X, B) != 0) X.reset();
    return X;
}

template <typename T> int MetaMat<T>::solve_trs(Mat<T>& X, const Mat<T>& B) {
    if(!factored) {
        suanpan_warning("the matrix is not factored.\n");
        return solve(X, B);
    }

    if(IPIV.is_empty()) return -1;

    X = B;

    auto TRANS = 'N';
    int N = n_rows;
    auto NRHS = int(B.n_cols);
    auto LDA = N;
    auto LDB = int(B.n_rows);
    auto INFO = 0;

    if(std::is_same<T, float>::value) {
        using E = float;
        arma_fortran(arma_sgetrs)(&TRANS, &N, &NRHS, (E*)memptr(), &LDA, IPIV.memptr(), (E*)X.memptr(), &LDB, &INFO);
    } else if(std::is_same<T, double>::value) {
        using E = double;
        arma_fortran(arma_dgetrs)(&TRANS, &N, &NRHS, (E*)memptr(), &LDA, IPIV.memptr(), (E*)X.memptr(), &LDB, &INFO);
    }

    return INFO;
}

template <typename T> MetaMat<T> MetaMat<T>::factorize() {
    auto X = *this;

    if(factored) {
        suanpan_warning("the matrix is factored.\n");
        return X;
    }

    arma_debug_check(X.n_rows != X.n_cols, "i() only accepts sqaure matrix.");

    int M = X.n_rows;
    auto N = M;
    auto LDA = M;
    X.IPIV.zeros(N);
    auto INFO = 0;

    if(std::is_same<T, float>::value) {
        using E = float;
        arma_fortran(arma_sgetrf)(&M, &N, (E*)X.memptr(), &LDA, X.IPIV.memptr(), &INFO);
    } else if(std::is_same<T, double>::value) {
        using E = double;
        arma_fortran(arma_dgetrf)(&M, &N, (E*)X.memptr(), &LDA, X.IPIV.memptr(), &INFO);
    }

    if(INFO != 0) {
        suanpan_error("factorize() fails.\n");
        X.reset();
    } else
        X.factored = true;

    return X;
}

template <typename T> MetaMat<T> MetaMat<T>::i() {
    auto X = *this;

    arma_debug_check(X.n_rows != X.n_cols, "i() only accepts sqaure matrix.");

    int M = X.n_rows;
    auto N = M;
    auto LDA = M;
    X.IPIV.zeros(N);
    auto INFO = 0;

    if(std::is_same<T, float>::value) {
        using E = float;
        arma_fortran(arma_sgetrf)(&M, &N, (E*)X.memptr(), &LDA, X.IPIV.memptr(), &INFO);
    } else if(std::is_same<T, double>::value) {
        using E = double;
        arma_fortran(arma_dgetrf)(&M, &N, (E*)X.memptr(), &LDA, X.IPIV.memptr(), &INFO);
    }

    if(INFO != 0) {
        X.reset();
        return X;
    }

    auto LWORK = 8 * M;
    const auto WORK = new T[LWORK];

    if(std::is_same<T, float>::value) {
        using E = float;
        arma_fortran(arma_sgetri)(&N, (E*)X.memptr(), &LDA, X.IPIV.memptr(), (E*)WORK, &LWORK, &INFO);
    } else if(std::is_same<T, double>::value) {
        using E = double;
        arma_fortran(arma_dgetri)(&N, (E*)X.memptr(), &LDA, X.IPIV.memptr(), (E*)WORK, &LWORK, &INFO);
    }

    delete[] WORK;

    if(INFO != 0) X.reset();

    return X;
}

template <typename T> MetaMat<T> MetaMat<T>::inv() { return i(); }

template <typename T> void MetaMat<T>::print() {
    for(unsigned I = 0; I < n_rows; ++I) {
        for(unsigned J = 0; J < n_cols; ++J) suanpan_info("%+.3E\t", operator()(I, J));
        suanpan_info("\n");
    }
    suanpan_info("\n");
}

#endif

//! @}
