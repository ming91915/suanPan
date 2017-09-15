/**
 * @class BandMat
 * @brief A BandMat class that holds matrices.
 *
 * @author T
 * @date 06/09/2017
 * @version 0.1.0
 * @file BandMat.hpp
 * @addtogroup MetaMat
 * @{
 */

#ifndef BANDMAT_HPP
#define BANDMAT_HPP

#include <Toolbox/debug.h>

template <typename T> class BandMat : public MetaMat<T> {
    using MetaMat<T>::i;
    using MetaMat<T>::inv;

    const unsigned low_bw;
    const unsigned up_bw;
    const unsigned shift_bw;

public:
    using MetaMat<T>::IPIV;
    using MetaMat<T>::TRAN;
    using MetaMat<T>::n_cols;
    using MetaMat<T>::n_rows;
    using MetaMat<T>::n_elem;
    using MetaMat<T>::memory;
    using MetaMat<T>::solve;
    using MetaMat<T>::solve_trs;

    BandMat();
    BandMat(const unsigned&, const unsigned&, const unsigned&);

    const T& operator()(const uword&, const uword&) const override;
    const T& at(const uword&, const uword&) const override;
    T& operator()(const uword&, const uword&) override;
    T& at(const uword&, const uword&) override;

    Mat<T> operator*(const Mat<T>&)override;

    int solve(Mat<T>&, const Mat<T>&) override;
    int solve_trs(Mat<T>&, const Mat<T>&) override;
};

template <typename T> struct is_Band { static const bool value = false; };

template <typename T> struct is_Band<BandMat<T>> { static const bool value = true; };

template <typename T>
BandMat<T>::BandMat()
    : MetaMat<T>()
    , low_bw(0)
    , up_bw(0)
    , shift_bw(0) {}

template <typename T>
BandMat<T>::BandMat(const unsigned& in_size, const unsigned& in_l, const unsigned& in_u)
    : MetaMat<T>(2 * in_l + in_u + 1, in_size, (2 * in_l + in_u + 1) * in_size)
    , low_bw(in_l)
    , up_bw(in_u)
    , shift_bw(low_bw + up_bw) {}

template <typename T> const T& BandMat<T>::operator()(const uword& in_row, const uword& in_col) const { return memory[in_row - in_col + shift_bw + in_col * n_rows]; }

template <typename T> const T& BandMat<T>::at(const uword& in_row, const uword& in_col) const { return memory[in_row - in_col + shift_bw + in_col * n_rows]; }

template <typename T> T& BandMat<T>::operator()(const uword& in_row, const uword& in_col) { return access::rw(memory[in_row - in_col + shift_bw + in_col * n_rows]); }

template <typename T> T& BandMat<T>::at(const uword& in_row, const uword& in_col) { return access::rw(memory[in_row - in_col + shift_bw + in_col * n_rows]); }

template <typename T> Mat<T> BandMat<T>::operator*(const Mat<T>& X) {
    if(X.is_colvec()) {
        auto Y = X;

        int M = n_cols;
        int N = n_cols;
        int KL = low_bw;
        int KU = up_bw;
        T ALPHA = 1.;
        int LDA = n_rows;
        auto INC = 1;
        T BETA = 0.;

        if(std::is_same<T, float>::value) {
            using E = float;
            arma_fortran(arma_sgbmv)(&TRAN, &M, &N, &KL, &KU, (E*)&ALPHA, (E*)this->memptr(), &LDA, (E*)X.memptr(), &INC, (E*)&BETA, (E*)Y.memptr(), &INC);
        } else if(std::is_same<T, double>::value) {
            using E = double;
            arma_fortran(arma_dgbmv)(&TRAN, &M, &N, &KL, &KU, (E*)&ALPHA, (E*)this->memptr(), &LDA, (E*)X.memptr(), &INC, (E*)&BETA, (E*)Y.memptr(), &INC);
        }

        return Y;
    }

    return X;
}

template <typename T> int BandMat<T>::solve(Mat<T>& X, const Mat<T>& B) {
    X = B;

    int N = n_cols;
    int KL = low_bw;
    int KU = up_bw;
    auto NRHS = static_cast<int>(B.n_cols);
    int LDAB = n_rows;
    auto LDB = static_cast<int>(B.n_rows);
    IPIV.zeros(N);
    auto INFO = 0;

    if(std::is_same<T, float>::value) {
        using E = float;
        arma_fortran(arma_sgbsv)(&N, &KL, &KU, &NRHS, (E*)this->memptr(), &LDAB, IPIV.memptr(), (E*)X.memptr(), &LDB, &INFO);
    } else if(std::is_same<T, double>::value) {
        using E = double;
        arma_fortran(arma_dgbsv)(&N, &KL, &KU, &NRHS, (E*)this->memptr(), &LDAB, IPIV.memptr(), (E*)X.memptr(), &LDB, &INFO);
    }

    if(INFO != 0) suanpan_error("solve() receives error code %u from base driver, the matrix is probably singular.\n", INFO);

    return INFO;
}

template <typename T> int BandMat<T>::solve_trs(Mat<T>& X, const Mat<T>& B) {
    if(IPIV.is_empty()) return -1;

    X = B;

    int N = n_cols;
    int KL = low_bw;
    int KU = up_bw;
    auto NRHS = static_cast<int>(B.n_cols);
    int LDAB = n_rows;
    auto LDB = static_cast<int>(B.n_rows);
    auto INFO = 0;

    if(std::is_same<T, float>::value) {
        using E = float;
        arma_fortran(arma_sgbtrs)(&TRAN, &N, &KL, &KU, &NRHS, (E*)this->memptr(), &LDAB, IPIV.memptr(), (E*)X.memptr(), &LDB, &INFO);
    } else if(std::is_same<T, double>::value) {
        using E = double;
        arma_fortran(arma_dgbtrs)(&TRAN, &N, &KL, &KU, &NRHS, (E*)this->memptr(), &LDAB, IPIV.memptr(), (E*)X.memptr(), &LDB, &INFO);
    }

    if(INFO != 0) suanpan_error("solve() receives error code %u from base driver, the matrix is probably singular.\n", INFO);

    return INFO;
}

#endif

//! @}
