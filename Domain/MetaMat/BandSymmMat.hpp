/**
 * @class BandSymmMat
 * @brief A BandSymmMat class that holds matrices.
 *
 * @author T
 * @date 06/09/2017
 * @version 0.1.0
 * @file BandSymmMat.hpp
 * @addtogroup MetaMat
 * @{
 */

#ifndef BANDSYMMMAT_HPP
#define BANDSYMMMAT_HPP

#include <Toolbox/debug.h>

template <typename T> class BandSymmMat : public MetaMat<T> {
    static T bin;
    using MetaMat<T>::i;
    using MetaMat<T>::inv;

    const char UPLO = 'L';
    const unsigned bw;

public:
    using MetaMat<T>::factored;
    using MetaMat<T>::n_cols;
    using MetaMat<T>::n_rows;
    using MetaMat<T>::n_elem;
    using MetaMat<T>::memory;
    using MetaMat<T>::solve;
    using MetaMat<T>::solve_trs;
    using MetaMat<T>::factorize;

    BandSymmMat();
    BandSymmMat(const unsigned&, const unsigned&);

    const T& operator()(const uword&, const uword&) const override;
    T& at(const uword&, const uword&) override;

    Mat<T> operator*(const Mat<T>&)override;

    int solve(Mat<T>&, const Mat<T>&) override;
    int solve_trs(Mat<T>&, const Mat<T>&) override;

    MetaMat<T> factorize() override;
};

template <typename T> struct is_BandSymm { static const bool value = false; };

template <typename T> struct is_BandSymm<BandSymmMat<T>> { static const bool value = true; };

template <typename T> T BandSymmMat<T>::bin = 0.;

template <typename T>
BandSymmMat<T>::BandSymmMat()
    : MetaMat<T>()
    , bw(0) {}

template <typename T>
BandSymmMat<T>::BandSymmMat(const unsigned& in_size, const unsigned& in_bandwidth)
    : MetaMat<T>(in_bandwidth + 1, in_size, (in_bandwidth + 1) * in_size)
    , bw(in_bandwidth) {}

template <typename T> const T& BandSymmMat<T>::operator()(const uword& in_row, const uword& in_col) const {
    if(std::abs(int(in_row) - int(in_col)) > int(bw)) {
        bin = 0.;
        return bin;
    }
    return memory[in_row > in_col ? in_row - in_col + in_col * n_rows : in_col - in_row + in_row * n_rows];
}

template <typename T> T& BandSymmMat<T>::at(const uword& in_row, const uword& in_col) {
#ifdef SUANPAN_DEBUG
    if(std::abs(int(in_row) - int(in_col)) > int(bw)) throw logic_error("index overflows.");
#endif

    if(in_row < in_col) return bin;
    return access::rw(memory[in_row - in_col + in_col * n_rows]);
}

template <typename T> Mat<T> BandSymmMat<T>::operator*(const Mat<T>& X) {
    if(X.is_colvec()) {
        auto Y = X;

        int N = n_cols;
        int K = bw;
        T ALPHA = 1.;
        int LDA = n_rows;
        auto INC = 1;
        T BETA = 0.;

        if(std::is_same<T, float>::value) {
            using E = float;
            arma_fortran(arma_ssbmv)(&UPLO, &N, &K, (E*)&ALPHA, (E*)this->memptr(), &LDA, (E*)X.memptr(), &INC, (E*)&BETA, (E*)Y.memptr(), &INC);
        } else if(std::is_same<T, double>::value) {
            using E = double;
            arma_fortran(arma_dsbmv)(&UPLO, &N, &K, (E*)&ALPHA, (E*)this->memptr(), &LDA, (E*)X.memptr(), &INC, (E*)&BETA, (E*)Y.memptr(), &INC);
        }

        return Y;
    }

    return X;
}

template <typename T> int BandSymmMat<T>::solve(Mat<T>& X, const Mat<T>& B) {
    if(factored) {
        suanpan_warning("the matrix is factored.\n");
        return this->solve_trs(X, B);
    }

    X = B;

    int N = n_cols;
    int KD = bw;
    auto NRHS = int(B.n_cols);
    int LDAB = n_rows;
    auto LDB = int(B.n_rows);
    auto INFO = 0;

    if(std::is_same<T, float>::value) {
        using E = float;
        arma_fortran(arma_spbsv)(&UPLO, &N, &KD, &NRHS, (E*)this->memptr(), &LDAB, (E*)X.memptr(), &LDB, &INFO);
    } else if(std::is_same<T, double>::value) {
        using E = double;
        arma_fortran(arma_dpbsv)(&UPLO, &N, &KD, &NRHS, (E*)this->memptr(), &LDAB, (E*)X.memptr(), &LDB, &INFO);
    }

    if(INFO != 0)
        suanpan_error("solve() receives error code %u from the base driver, the matrix is probably singular.\n", INFO);
    else
        factored = true;

    return INFO;
}

template <typename T> int BandSymmMat<T>::solve_trs(Mat<T>& X, const Mat<T>& B) {
    if(!factored) {
        suanpan_warning("the matrix is not factored.\n");
        return this->solve(X, B);
    }

    X = B;

    int N = n_cols;
    int KD = bw;
    auto NRHS = int(B.n_cols);
    int LDAB = n_rows;
    auto LDB = int(B.n_rows);
    auto INFO = 0;

    if(std::is_same<T, float>::value) {
        using E = float;
        arma_fortran(arma_spbtrs)(&UPLO, &N, &KD, &NRHS, (E*)this->memptr(), &LDAB, (E*)X.memptr(), &LDB, &INFO);
    } else if(std::is_same<T, double>::value) {
        using E = double;
        arma_fortran(arma_dpbtrs)(&UPLO, &N, &KD, &NRHS, (E*)this->memptr(), &LDAB, (E*)X.memptr(), &LDB, &INFO);
    }

    if(INFO != 0) suanpan_error("solve() receives error code %u from the base driver, the matrix is probably singular.\n", INFO);

    return INFO;
}

template <typename T> MetaMat<T> BandSymmMat<T>::factorize() {
    auto X = *this;

    if(factored) {
        suanpan_warning("the matrix is factored.\n");
        return X;
    }

    int N = n_cols;
    int KD = bw;
    int LDAB = n_rows;
    auto INFO = 0;

    if(std::is_same<T, float>::value) {
        using E = float;
        arma_fortran(arma_spbtrf)(&UPLO, &N, &KD, (E*)X.memptr(), &LDAB, &INFO);
    } else if(std::is_same<T, double>::value) {
        using E = double;
        arma_fortran(arma_dpbtrf)(&UPLO, &N, &KD, (E*)X.memptr(), &LDAB, &INFO);
    }

    if(INFO != 0) {
        suanpan_error("factorize() fails.\n");
        X.reset();
    } else
        X.factored = true;

    return X;
}

#endif

//! @}
