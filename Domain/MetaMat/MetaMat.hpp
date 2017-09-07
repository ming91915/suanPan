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
    const char TRAN = 'N';
    const unsigned n_rows;
    const unsigned n_cols;
    const unsigned n_elem;

    const T* const memory = nullptr;

    explicit MetaMat(const unsigned& = 0, const unsigned& = 0, const unsigned& = 0);
    virtual ~MetaMat();

    void init();
    void zeros();

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
};

template <typename T>
MetaMat<T>::MetaMat(const unsigned& in_rows, const unsigned& in_cols, const unsigned& in_elem)
    : n_rows(in_rows)
    , n_cols(in_cols)
    , n_elem(in_elem) {
    init();
    zeros();
}

template <typename T>
MetaMat<T>::~MetaMat() {
    if(memory != nullptr) memory::release(access::rw(memory));
}

template <typename T>
void MetaMat<T>::init() {
    if(memory != nullptr) memory::release(access::rw(memory));
    access::rw(memory) = n_elem == 0 ? nullptr : memory::acquire<T>(n_elem);
}

template <typename T>
void MetaMat<T>::zeros() {
    arrayops::fill_zeros(memptr(), n_elem);
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
        auto M = static_cast<int>(n_rows);
        auto N = static_cast<int>(B.n_cols);
        auto K = static_cast<int>(n_cols);
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
    const auto IPIV = new int[N];
    auto INFO = 0;

    if(std::is_same<T, float>::value) {
        using E = float;
        arma_fortran(arma_sgesv)(&N, &NRHS, (E*)memptr(), &LDA, IPIV, (E*)X.memptr(), &LDB, &INFO);
    } else if(std::is_same<T, double>::value) {
        using E = double;
        arma_fortran(arma_dgesv)(&N, &NRHS, (E*)memptr(), &LDA, IPIV, (E*)X.memptr(), &LDB, &INFO);
    }

    delete[] IPIV;

    return INFO;
}

#endif

//! @}
