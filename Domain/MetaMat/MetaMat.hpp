/**
 * @class MetaMat
 * @brief A MetaMat class that holds matrices.
 *
 * @author T
 * @date 06/09/2017
 * @version 0.1.0
 * @file MetaMat.hpp
 * @addtogroup MetaMat
 * @{
 */

#ifndef METAMAT_HPP
#define METAMAT_HPP

#include <armadillo>

using namespace arma;

template <typename T1, typename T2>
class MetaMat {
public:
    typedef T2 class_type;

    const char TRAN = 'N';
    const unsigned n_rows;
    const unsigned n_cols;
    const unsigned n_elem;

    const T1* const memory = nullptr;

    MetaMat(const unsigned&, const unsigned&, const unsigned&);
    virtual ~MetaMat();

    void init();
    void zeros();

    virtual const T1& operator()(const unsigned&, const unsigned&) const;
    virtual const T1& at(const unsigned&, const unsigned&) const;
    virtual T1& operator()(const unsigned&, const unsigned&);
    virtual T1& at(const unsigned&, const unsigned&);

    const T1* memptr() const;
    T1* memptr();

    virtual MetaMat& operator+(const MetaMat&);
    virtual MetaMat& operator-(const MetaMat&);
    virtual MetaMat& operator+=(const MetaMat&);
    virtual MetaMat& operator-=(const MetaMat&);

    virtual MetaMat& operator*(const T1&);
    virtual Mat<T1> operator*(const Mat<T1>&);
    virtual Col<T1> operator*(const Col<T1>&);

    virtual Mat<T1> solve(const Mat<T1>&);
    virtual bool solve(Mat<T1>&, const Mat<T1>&);
};

template <typename T1, typename T2>
MetaMat<T1, T2>::MetaMat(const unsigned& in_rows, const unsigned& in_cols, const unsigned& in_elem)
    : n_rows(in_rows)
    , n_cols(in_cols)
    , n_elem(in_elem) {
    init();
    zeros();
}

template <typename T1, typename T2>
MetaMat<T1, T2>::~MetaMat() {
    if(memory != nullptr) memory::release(access::rw(memory));
}

template <typename T1, typename T2>
void MetaMat<T1, T2>::init() {
    if(memory != nullptr) memory::release(access::rw(memory));
    access::rw(memory) = n_elem == 0 ? nullptr : memory::acquire<T1>(n_elem);
}

template <typename T1, typename T2>
void MetaMat<T1, T2>::zeros() {
    arrayops::fill_zeros(memptr(), n_elem);
}

template <typename T1, typename T2>
const T1& MetaMat<T1, T2>::operator()(const unsigned& in_row, const unsigned& in_col) const {
    return memory[in_row + in_col * n_rows];
}

template <typename T1, typename T2>
const T1& MetaMat<T1, T2>::at(const unsigned& in_row, const unsigned& in_col) const {
    return memory[in_row + in_col * n_rows];
}

template <typename T1, typename T2>
T1& MetaMat<T1, T2>::operator()(const unsigned& in_row, const unsigned& in_col) {
    return access::rw(memory[in_row + in_col * n_rows]);
}

template <typename T1, typename T2>
T1& MetaMat<T1, T2>::at(const unsigned& in_row, const unsigned& in_col) {
    return access::rw(memory[in_row + in_col * n_rows]);
}

template <typename T1, typename T2>
const T1* MetaMat<T1, T2>::memptr() const {
    return memory;
}

template <typename T1, typename T2>
T1* MetaMat<T1, T2>::memptr() {
    return const_cast<T1*>(memory);
}

template <typename T1, typename T2>
MetaMat<T1, T2>& MetaMat<T1, T2>::operator+(const MetaMat& M) {
    return *this += M;
}

template <typename T1, typename T2>
MetaMat<T1, T2>& MetaMat<T1, T2>::operator-(const MetaMat& M) {
    return *this -= M;
}

template <typename T1, typename T2>
MetaMat<T1, T2>& MetaMat<T1, T2>::operator+=(const MetaMat& M) {
    if(n_rows == M.n_rows && n_cols == M.n_cols && n_elem == M.n_elem) arrayops::inplace_plus(memptr(), M.memptr(), n_elem);
    return *this;
}

template <typename T1, typename T2>
MetaMat<T1, T2>& MetaMat<T1, T2>::operator-=(const MetaMat& M) {
    if(n_rows == M.n_rows && n_cols == M.n_cols && n_elem == M.n_elem) arrayops::inplace_minus(memptr(), M.memptr(), n_elem);
    return *this;
}

template <typename T1, typename T2>
MetaMat<T1, T2>& MetaMat<T1, T2>::operator*(const T1& value) {
    arrayops::inplace_mul(memptr(), value, n_elem);
    return *this;
}

template <typename T1, typename T2>
Mat<T1> MetaMat<T1, T2>::operator*(const Mat<T1>& B) {
    auto C = B;

    auto M = static_cast<int>(n_rows);
    auto N = static_cast<int>(B.n_cols);
    auto K = static_cast<int>(n_cols);
    T1 ALPHA = 1.0;
    auto LDA = M;
    auto LDB = K;
    T1 BETA = 0.;
    auto LDC = M;

    if(std::is_same<T1, float>::value) {
        using E = float;
        // ReSharper disable once CppCStyleCast
        arma_fortran(arma_sgemm)(&TRAN, &TRAN, &M, &N, &K, reinterpret_cast<E*>(&ALPHA), reinterpret_cast<E*>(memptr()), &LDA, (E*)(B.memptr()), &LDB, reinterpret_cast<E*>(&BETA), reinterpret_cast<E*>(C.memptr()), &LDC);
    } else if(std::is_same<T1, double>::value) {
        using E = double;
        // ReSharper disable once CppCStyleCast
        arma_fortran(arma_dgemm)(&TRAN, &TRAN, &M, &N, &K, reinterpret_cast<E*>(&ALPHA), reinterpret_cast<E*>(memptr()), &LDA, (E*)(B.memptr()), &LDB, reinterpret_cast<E*>(&BETA), reinterpret_cast<E*>(C.memptr()), &LDC);
    }

    return C;
}

template <typename T1, typename T2>
Col<T1> MetaMat<T1, T2>::operator*(const Col<T1>& X) {
    auto Y = X;

    int M = n_rows;
    int N = n_cols;
    T1 ALPHA = 1.;
    auto LDA = M;
    auto INCX = 1;
    T1 BETA = 0.;
    auto INCY = 1;

    if(std::is_same<T1, float>::value) {
        using E = float;
        // ReSharper disable once CppCStyleCast
        arma_fortran(arma_sgemv)(&TRAN, &M, &N, reinterpret_cast<E*>(&ALPHA), reinterpret_cast<E*>(memptr()), &LDA, (E*)(X.memptr()), &INCX, reinterpret_cast<E*>(&BETA), reinterpret_cast<E*>(Y.memptr()), &INCY);
    } else if(std::is_same<T1, double>::value) {
        using E = double;
        // ReSharper disable once CppCStyleCast
        arma_fortran(arma_dgemv)(&TRAN, &M, &N, reinterpret_cast<E*>(&ALPHA), reinterpret_cast<E*>(memptr()), &LDA, (E*)(X.memptr()), &INCX, reinterpret_cast<E*>(&BETA), reinterpret_cast<E*>(Y.memptr()), &INCY);
    }

    return Y;
}

template <typename T1, typename T2>
Mat<T1> MetaMat<T1, T2>::solve(const Mat<T1>& B) {
    Mat<T1> X;

    if(!solve(X, B)) X.reset();

    return X;
}

template <typename T1, typename T2>
bool MetaMat<T1, T2>::solve(Mat<T1>& X, const Mat<T1>& B) {
    X = B;

    int N = n_rows;
    auto NRHS = static_cast<int>(B.n_cols);
    auto LDA = N;
    auto LDB = N;
    const auto IPIV = new int[N];
    auto INFO = 0;

    if(std::is_same<T1, float>::value) {
        using E = float;
        arma_fortran(arma_sgesv)(&N, &NRHS, reinterpret_cast<E*>(memptr()), &LDA, IPIV, reinterpret_cast<E*>(X.memptr()), &LDB, &INFO);
    } else if(std::is_same<T1, double>::value) {
        using E = double;
        arma_fortran(arma_dgesv)(&N, &NRHS, reinterpret_cast<E*>(memptr()), &LDA, IPIV, reinterpret_cast<E*>(X.memptr()), &LDB, &INFO);
    }

    delete[] IPIV;

    return INFO == 0;
}

#endif

//! @}
