/**
 * @class SymmPackMat
 * @brief A SymmPackMat class that holds matrices.
 *
 * @author T
 * @date 06/09/2017
 * @version 0.1.0
 * @file SymmPackMat.hpp
 * @addtogroup MetaMat
 * @{
 */

#ifndef SYMMPACKMAT_HPP
#define SYMMPACKMAT_HPP

template <typename T> class SymmPackMat : public MetaMat<T> {
    static T bin;
    const char SIDE = 'R';
    const char UPLO = 'U';

public:
    using MetaMat<T>::IPIV;
    using MetaMat<T>::TRAN;
    using MetaMat<T>::n_cols;
    using MetaMat<T>::n_rows;
    using MetaMat<T>::n_elem;
    using MetaMat<T>::memory;
    using MetaMat<T>::solve;
    using MetaMat<T>::solve_trs;
    using MetaMat<T>::factorize;

    SymmPackMat();
    explicit SymmPackMat(const unsigned&);

    const T& operator()(const uword&, const uword&) const override;
    T& at(const uword&, const uword&) override;

    Mat<T> operator*(const Mat<T>&)override;

    int solve(Mat<T>&, const Mat<T>&) override;
    int solve_trs(Mat<T>&, const Mat<T>&) override;

    MetaMat<T> factorize() override;

    MetaMat<T> i() override;
};

template <typename T> struct is_SymmPack { static const bool value = false; };

template <typename T> struct is_SymmPack<SymmPackMat<T>> { static const bool value = true; };

template <typename T> T SymmPackMat<T>::bin = 0.;

template <typename T>
SymmPackMat<T>::SymmPackMat()
    : MetaMat<T>() {}

template <typename T>
SymmPackMat<T>::SymmPackMat(const unsigned& in_size)
    : MetaMat<T>(in_size, in_size, (in_size + 1) * in_size / 2) {}

template <typename T> const T& SymmPackMat<T>::operator()(const uword& in_row, const uword& in_col) const { return memory[in_col > in_row ? (in_col * in_col + in_col) / 2 + in_row : (in_row * in_row + in_row) / 2 + in_col]; }

template <typename T> T& SymmPackMat<T>::at(const uword& in_row, const uword& in_col) {
    if(in_col < in_row) return bin;
    return access::rw(memory[(in_col * in_col + in_col) / 2 + in_row]);
}

template <const char S, const char T, typename T1> Mat<T1> spmm(const SymmPackMat<T1>& A, const Mat<T1>& B);

template <typename T> Mat<T> SymmPackMat<T>::operator*(const Mat<T>& X) {
    if(X.is_colvec()) {
        auto Y = X;

        int N = n_rows;
        T ALPHA = 1.;
        auto INC = 1;
        T BETA = 0.;

        if(std::is_same<T, float>::value) {
            using E = float;
            arma_fortran(arma_sspmv)(&UPLO, &N, (E*)&ALPHA, (E*)this->memptr(), (E*)X.memptr(), &INC, (E*)&BETA, (E*)Y.memptr(), &INC);
        } else if(std::is_same<T, double>::value) {
            using E = double;
            arma_fortran(arma_dspmv)(&UPLO, &N, (E*)&ALPHA, (E*)this->memptr(), (E*)X.memptr(), &INC, (E*)&BETA, (E*)Y.memptr(), &INC);
        }

        return Y;
    }

    return spmm<'R', 'N'>(*this, X);
}

template <typename T> int SymmPackMat<T>::solve(Mat<T>& X, const Mat<T>& B) {
    X = B;

    int N = n_rows;
    auto NRHS = int(B.n_cols);
    auto LDB = int(B.n_rows);
    auto INFO = 0;

    if(std::is_same<T, float>::value) {
        using E = float;
        arma_fortran(arma_sppsv)(&UPLO, &N, &NRHS, (E*)this->memptr(), (E*)X.memptr(), &LDB, &INFO);
    } else if(std::is_same<T, double>::value) {
        using E = double;
        arma_fortran(arma_dppsv)(&UPLO, &N, &NRHS, (E*)this->memptr(), (E*)X.memptr(), &LDB, &INFO);
    }

    return INFO;
}

template <typename T> int SymmPackMat<T>::solve_trs(Mat<T>& X, const Mat<T>& B) {
    X = B;

    int N = n_rows;
    auto NRHS = int(B.n_cols);
    auto LDB = int(B.n_rows);
    auto INFO = 0;

    if(std::is_same<T, float>::value) {
        using E = float;
        arma_fortran(arma_spptrs)(&UPLO, &N, &NRHS, (E*)this->memptr(), (E*)X.memptr(), &LDB, &INFO);
    } else if(std::is_same<T, double>::value) {
        using E = double;
        arma_fortran(arma_dpptrs)(&UPLO, &N, &NRHS, (E*)this->memptr(), (E*)X.memptr(), &LDB, &INFO);
    }

    return INFO;
}

template <typename T> MetaMat<T> SymmPackMat<T>::factorize() {
    auto X = *this;

    int N = n_rows;
    auto INFO = 0;

    if(std::is_same<T, float>::value) {
        using E = float;
        arma_fortran(arma_spptrf)(&UPLO, &N, (E*)X.memptr(), &INFO);
    } else if(std::is_same<T, double>::value) {
        using E = double;
        arma_fortran(arma_dpptrf)(&UPLO, &N, (E*)X.memptr(), &INFO);
    }

    if(INFO != 0) {
        suanpan_error("factorize() fails.\n");
        X.reset();
    }

    return X;
}

template <typename T> MetaMat<T> SymmPackMat<T>::i() {
    auto X = *this;

    int N = X.n_rows;
    auto INFO = 0;

    if(std::is_same<T, float>::value) {
        using E = float;
        arma_fortran(arma_spptrf)(&X.UPLO, &N, (E*)X.memptr(), &INFO);
    } else if(std::is_same<T, double>::value) {
        using E = double;
        arma_fortran(arma_dpptrf)(&X.UPLO, &N, (E*)X.memptr(), &INFO);
    }

    if(INFO != 0) {
        X.reset();
        return X;
    }

    const auto WORK = new T[N];

    if(std::is_same<T, float>::value) {
        using E = float;
        arma_fortran(arma_spptri)(&X.UPLO, &N, (E*)X.memptr(), (E*)WORK, &INFO);
    } else if(std::is_same<T, double>::value) {
        using E = double;
        arma_fortran(arma_dpptri)(&X.UPLO, &N, (E*)X.memptr(), (E*)WORK, &INFO);
    }

    if(INFO != 0) X.reset();

    delete[] WORK;

    return X;
}

#endif

//! @}
