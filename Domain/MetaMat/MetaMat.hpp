/**
 * @class MetaMat
 * @brief A MetaMat class that holds matrices.
 *
 * @author T
 * @date 06/09/2017
 * @version 0.1.0
 * @file MetaMat.hpp
 * @{
 */

#ifndef METAMAT_HPP
#define METAMAT_HPP

#include <armadillo>

using namespace arma;

template <typename T>
class MetaMat {
public:
    const unsigned n_rows;
    const unsigned n_cols;
    const unsigned n_elem;

    const T* const memory = nullptr;

    MetaMat(const unsigned& in_rows, const unsigned& in_cols, const unsigned& in_elem)
        : n_rows(in_rows)
        , n_cols(in_cols)
        , n_elem(in_elem) {
        init();
        zeros();
    }

    virtual ~MetaMat() {
        if(memory != nullptr) memory::release(access::rw(memory));
    }

    void init() {
        if(memory != nullptr) memory::release(access::rw(memory));
        access::rw(memory) = n_elem == 0 ? nullptr : memory::acquire<T>(n_elem);
    }

    void zeros() { arrayops::fill_zeros(memptr(), n_elem); }

    virtual const T& operator()(const unsigned& in_row, const unsigned& in_col) const { return memory[in_row + in_col * n_rows]; }
    virtual const T& at(const unsigned& in_row, const unsigned& in_col) const { return memory[in_row + in_col * n_rows]; }
    virtual T& operator()(const unsigned& in_row, const unsigned& in_col) { return access::rw(memory[in_row + in_col * n_rows]); }
    virtual T& at(const unsigned& in_row, const unsigned& in_col) { return access::rw(memory[in_row + in_col * n_rows]); }

    const T* memptr() const { return memory; }
    T* memptr() { return const_cast<T*>(memory); }

    virtual MetaMat& operator+(const MetaMat& M) { return *this += M; }
    virtual MetaMat& operator-(const MetaMat& M) { return *this -= M; }
    virtual MetaMat& operator+=(const MetaMat& M) {
        if(n_rows == M.n_rows && n_cols == M.n_cols && n_elem == M.n_elem) arrayops::inplace_plus(memptr(), M.memptr(), n_elem);
        return *this;
    }
    virtual MetaMat& operator-=(const MetaMat& M) {
        if(n_rows == M.n_rows && n_cols == M.n_cols && n_elem == M.n_elem) arrayops::inplace_minus(memptr(), M.memptr(), n_elem);
        return *this;
    }

    virtual MetaMat& operator*(const T& value) {
        arrayops::inplace_mul(memptr(), value, n_elem);
        return *this;
    }
    virtual Mat<T> operator*(const Mat<T>& B) {
        auto C = B;

        auto M = static_cast<int>(n_rows);
        auto N = static_cast<int>(B.n_cols);
        auto K = static_cast<int>(n_cols);
        T ALPHA = 1.0;
        auto LDA = M;
        auto LDB = K;
        T BETA = 0.;
        auto LDC = M;

        if(std::is_same<T, float>::value) {
            using E = float;
            arma_fortran(arma_sgemm)("N", "N", &M, &N, &K, reinterpret_cast<E*>(&ALPHA), reinterpret_cast<E*>(memptr()), &LDA, (E*)(B.memptr()), &LDB, reinterpret_cast<E*>(&BETA), reinterpret_cast<E*>(C.memptr()), &LDC);
        } else if(std::is_same<T, double>::value) {
            using E = double;
            arma_fortran(arma_dgemm)("N", "N", &M, &N, &K, reinterpret_cast<E*>(&ALPHA), reinterpret_cast<E*>(memptr()), &LDA, (E*)(B.memptr()), &LDB, reinterpret_cast<E*>(&BETA), reinterpret_cast<E*>(C.memptr()), &LDC);
        }

        return C;
    }
    virtual Col<T> operator*(const Col<T>& X) {
        auto Y = X;

        int M = n_rows;
        int N = n_cols;
        T ALPHA = 1.;
        auto LDA = M;
        auto INCX = 1;
        T BETA = 0.;
        auto INCY = 1;

        if(std::is_same<T, float>::value) {
            using E = float;
            arma_fortran(arma_sgemv)("N", &M, &N, reinterpret_cast<E*>(&ALPHA), reinterpret_cast<E*>(memptr()), &LDA, (E*)(X.memptr()), &INCX, reinterpret_cast<E*>(&BETA), reinterpret_cast<E*>(Y.memptr()), &INCY);
        } else if(std::is_same<T, double>::value) {
            using E = double;
            arma_fortran(arma_dgemv)("N", &M, &N, reinterpret_cast<E*>(&ALPHA), reinterpret_cast<E*>(memptr()), &LDA, (E*)(X.memptr()), &INCX, reinterpret_cast<E*>(&BETA), reinterpret_cast<E*>(Y.memptr()), &INCY);
        }

        return Y;
    }

    virtual Mat<T> solve(const Mat<T>& B) {
        Mat<T> X;

        if(!solve(X, B)) X.reset();

        return X;
    }
    virtual bool solve(Mat<T>& X, const Mat<T>& B) {
        X = B;

        int N = n_rows;
        auto NRHS = static_cast<int>(B.n_cols);
        auto LDA = N;
        auto LDB = N;
        const auto IPIV = new int[N];
        auto INFO = 0;

        if(std::is_same<T, float>::value) {
            using E = float;
            arma_fortran(arma_sgesv)(&N, &NRHS, reinterpret_cast<E*>(memptr()), &LDA, IPIV, reinterpret_cast<E*>(X.memptr()), &LDB, &INFO);
        } else if(std::is_same<T, double>::value) {
            using E = double;
            arma_fortran(arma_dgesv)(&N, &NRHS, reinterpret_cast<E*>(memptr()), &LDA, IPIV, reinterpret_cast<E*>(X.memptr()), &LDB, &INFO);
        }

        delete[] IPIV;

        return INFO == 0;
    }
};

#endif

//! @}
