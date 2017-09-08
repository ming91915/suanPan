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

#include <armadillo>

using namespace arma;

template <typename T>
class MetaMat {
public:
    Col<int> IPIV;
    const char TRAN = 'N';
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

    virtual const T& operator()(const unsigned&, const unsigned&) const;
    virtual const T& at(const unsigned&, const unsigned&) const;
    virtual T& operator()(const unsigned&, const unsigned&);
    virtual T& at(const unsigned&, const unsigned&);

    const T* memptr() const;
    T* memptr();

    virtual MetaMat& operator+(const MetaMat&);
    virtual MetaMat& operator-(const MetaMat&);
    virtual MetaMat& operator+=(const MetaMat&);
    virtual MetaMat& operator-=(const MetaMat&);

    virtual MetaMat& operator*(const T&);
    virtual Mat<T> operator*(const Mat<T>&);

    virtual Mat<T> solve(const Mat<T>&);
    virtual int solve(Mat<T>&, const Mat<T>&);

    virtual Mat<T> solve_trs(const Mat<T>&);
    virtual int solve_trs(Mat<T>&, const Mat<T>&);

    virtual MetaMat i();
    virtual MetaMat inv();
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
    : n_rows(old_mat.n_rows)
    , n_cols(old_mat.n_cols)
    , n_elem(old_mat.n_elem) {
    init();
    memcpy(memptr(), old_mat.memptr(), old_mat.n_elem * sizeof(T));
}

template <typename T>
MetaMat<T>::MetaMat(MetaMat&& old_mat) noexcept
    : n_rows(old_mat.n_rows)
    , n_cols(old_mat.n_cols)
    , n_elem(old_mat.n_elem) {
    access::rw(memory) = old_mat.memory;
    access::rw(old_mat.memory) = nullptr;
}

template <typename T>
MetaMat<T>& MetaMat<T>::operator=(const MetaMat& old_mat) {
    if(this != &old_mat) {
        access::rw(n_rows) = old_mat.n_rows;
        access::rw(n_cols) = old_mat.n_cols;
        access::rw(n_elem) = old_mat.n_elem;
        init();
        memcpy(memptr(), old_mat.memptr(), old_mat.n_elem * sizeof(T));
    }
    return *this;
}

template <typename T>
MetaMat<T>& MetaMat<T>::operator=(MetaMat&& old_mat) noexcept {
    if(this != &old_mat) {
        access::rw(n_rows) = old_mat.n_rows;
        access::rw(n_cols) = old_mat.n_cols;
        access::rw(n_elem) = old_mat.n_elem;
        access::rw(memory) = old_mat.memory;
        access::rw(old_mat.memory) = nullptr;
    }
    return *this;
}

template <typename T>
MetaMat<T>::~MetaMat() {
    if(memory != nullptr) memory::release(access::rw(memory));
}

template <typename T>
bool MetaMat<T>::is_empty() const {
    return n_elem == 0;
}

template <typename T>
void MetaMat<T>::init() {
    if(memory != nullptr) memory::release(access::rw(memory));
    access::rw(memory) = is_empty() ? nullptr : memory::acquire<T>(n_elem);
}

template <typename T>
void MetaMat<T>::zeros() {
    arrayops::fill_zeros(memptr(), n_elem);
}

template <typename T>
void MetaMat<T>::reset() {
    access::rw(n_rows) = 0;
    access::rw(n_cols) = 0;
    access::rw(n_elem) = 0;
    if(memory != nullptr) memory::release(access::rw(memory));
    access::rw(memory) = nullptr;
}

template <typename T>
const T& MetaMat<T>::operator()(const unsigned& in_row, const unsigned& in_col) const {
    return memory[in_row + in_col * n_rows];
}

template <typename T>
const T& MetaMat<T>::at(const unsigned& in_row, const unsigned& in_col) const {
    return memory[in_row + in_col * n_rows];
}

template <typename T>
T& MetaMat<T>::operator()(const unsigned& in_row, const unsigned& in_col) {
    return access::rw(memory[in_row + in_col * n_rows]);
}

template <typename T>
T& MetaMat<T>::at(const unsigned& in_row, const unsigned& in_col) {
    return access::rw(memory[in_row + in_col * n_rows]);
}

template <typename T>
const T* MetaMat<T>::memptr() const {
    return memory;
}

template <typename T>
T* MetaMat<T>::memptr() {
    return const_cast<T*>(memory);
}

template <typename T>
MetaMat<T>& MetaMat<T>::operator+(const MetaMat& M) {
    return *this += M;
}

template <typename T>
MetaMat<T>& MetaMat<T>::operator-(const MetaMat& M) {
    return *this -= M;
}

template <typename T>
MetaMat<T>& MetaMat<T>::operator+=(const MetaMat& M) {
    if(n_rows == M.n_rows && n_cols == M.n_cols && n_elem == M.n_elem) arrayops::inplace_plus(memptr(), M.memptr(), n_elem);
    return *this;
}

template <typename T>
MetaMat<T>& MetaMat<T>::operator-=(const MetaMat& M) {
    if(n_rows == M.n_rows && n_cols == M.n_cols && n_elem == M.n_elem) arrayops::inplace_minus(memptr(), M.memptr(), n_elem);
    return *this;
}

template <typename T>
MetaMat<T>& MetaMat<T>::operator*(const T& value) {
    arrayops::inplace_mul(memptr(), value, n_elem);
    return *this;
}

template <typename T>
Mat<T> MetaMat<T>::operator*(const Mat<T>& B) {
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
            arma_fortran(arma_sgemv)(&TRAN, &M, &N, reinterpret_cast<E*>(&ALPHA), reinterpret_cast<E*>(memptr()), &LDA, (E*)B.memptr(), &INCX, reinterpret_cast<E*>(&BETA), reinterpret_cast<E*>(C.memptr()), &INCY);
        } else if(std::is_same<T, double>::value) {
            using E = double;
            arma_fortran(arma_dgemv)(&TRAN, &M, &N, reinterpret_cast<E*>(&ALPHA), reinterpret_cast<E*>(memptr()), &LDA, (E*)B.memptr(), &INCX, reinterpret_cast<E*>(&BETA), reinterpret_cast<E*>(C.memptr()), &INCY);
        }
    } else {
        int M = n_rows;
        auto N = static_cast<int>(B.n_cols);
        int K = n_cols;
        T ALPHA = 1.;
        auto LDA = M;
        auto LDB = K;
        T BETA = 0.;
        auto LDC = M;

        if(std::is_same<T, float>::value) {
            using E = float;
            arma_fortran(arma_sgemm)(&TRAN, &TRAN, &M, &N, &K, reinterpret_cast<E*>(&ALPHA), reinterpret_cast<E*>(memptr()), &LDA, (E*)B.memptr(), &LDB, reinterpret_cast<E*>(&BETA), reinterpret_cast<E*>(C.memptr()), &LDC);
        } else if(std::is_same<T, double>::value) {
            using E = double;
            arma_fortran(arma_dgemm)(&TRAN, &TRAN, &M, &N, &K, reinterpret_cast<E*>(&ALPHA), reinterpret_cast<E*>(memptr()), &LDA, (E*)B.memptr(), &LDB, reinterpret_cast<E*>(&BETA), reinterpret_cast<E*>(C.memptr()), &LDC);
        }
    }

    return C;
}

template <typename T>
Mat<T> MetaMat<T>::solve(const Mat<T>& B) {
    Mat<T> X;
    if(solve(X, B) != 0) X.reset();
    return X;
}

template <typename T>
int MetaMat<T>::solve(Mat<T>& X, const Mat<T>& B) {
    X = B;

    int N = n_rows;
    auto NRHS = static_cast<int>(B.n_cols);
    auto LDA = N;
    auto LDB = static_cast<int>(B.n_rows);
    IPIV.zeros(N);
    auto INFO = 0;

    if(std::is_same<T, float>::value) {
        using E = float;
        arma_fortran(arma_sgesv)(&N, &NRHS, (E*)memptr(), &LDA, IPIV.memptr(), (E*)X.memptr(), &LDB, &INFO);
    } else if(std::is_same<T, double>::value) {
        using E = double;
        arma_fortran(arma_dgesv)(&N, &NRHS, (E*)memptr(), &LDA, IPIV.memptr(), (E*)X.memptr(), &LDB, &INFO);
    }

    return INFO;
}

template <typename T>
Mat<T> MetaMat<T>::solve_trs(const Mat<T>& B) {
    Mat<T> X;
    if(solve_trs(X, B) != 0) X.reset();
    return X;
}

template <typename T>
int MetaMat<T>::solve_trs(Mat<T>& X, const Mat<T>& B) {
    if(IPIV.is_empty()) return -1;

    X = B;

    auto TRANS = 'N';
    int N = n_rows;
    auto NRHS = static_cast<int>(B.n_cols);
    auto LDA = N;
    auto LDB = static_cast<int>(B.n_rows);
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

template <typename T>
MetaMat<T> MetaMat<T>::i() {
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
    const auto WORK = new double[LWORK];

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

template <typename T>
MetaMat<T> MetaMat<T>::inv() {
    return i();
}

#endif

//! @}
