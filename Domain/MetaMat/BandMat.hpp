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

template <typename T>
class BandMat : public MetaMat<T> {
    const unsigned low_bw;
    const unsigned up_bw;
    const unsigned shift_bw;

public:
    using MetaMat<T>::TRAN;
    using MetaMat<T>::n_cols;
    using MetaMat<T>::n_rows;
    using MetaMat<T>::n_elem;
    using MetaMat<T>::memory;

    BandMat(const unsigned&, const unsigned&, const unsigned&);

    const T& operator()(const unsigned&, const unsigned&) const override;
    const T& at(const unsigned&, const unsigned&) const override;
    T& operator()(const unsigned&, const unsigned&) override;
    T& at(const unsigned&, const unsigned&) override;

    Mat<T> operator*(const Mat<T>&)override;

    Mat<T> solve(const Mat<T>&) override;
    int solve(Mat<T>&, const Mat<T>&) override;
};

template <typename T>
struct is_Band {
    static const bool value = false;
};

template <typename T>
struct is_Band<BandMat<T>> {
    static const bool value = true;
};

template <typename T>
BandMat<T>::BandMat(const unsigned& in_size, const unsigned& in_l, const unsigned& in_u)
    : MetaMat<T>(2 * in_l + in_u + 1, in_size, (2 * in_l + in_u + 1) * in_size)
    , low_bw(in_l)
    , up_bw(in_u)
    , shift_bw(low_bw + up_bw) {}

template <typename T>
const T& BandMat<T>::operator()(const unsigned& in_row, const unsigned& in_col) const {
    return memory[in_row - in_col + shift_bw + in_col * n_rows];
}

template <typename T>
const T& BandMat<T>::at(const unsigned& in_row, const unsigned& in_col) const {
    return memory[in_row - in_col + shift_bw + in_col * n_rows];
}

template <typename T>
T& BandMat<T>::operator()(const unsigned& in_row, const unsigned& in_col) {
    return access::rw(memory[in_row - in_col + shift_bw + in_col * n_rows]);
}

template <typename T>
T& BandMat<T>::at(const unsigned& in_row, const unsigned& in_col) {
    return access::rw(memory[in_row - in_col + shift_bw + in_col * n_rows]);
}

template <typename T>
Mat<T> BandMat<T>::operator*(const Mat<T>& X) {
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
            arma_fortran(arma_sgbmv)(&TRAN, &M, &N, &KL, &KU, reinterpret_cast<E*>(&ALPHA), reinterpret_cast<E*>(this->memptr()), &LDA, (E*)X.memptr(), &INC, &BETA, reinterpret_cast<E*>(Y.memptr()), &INC);
        } else if(std::is_same<T, double>::value) {
            using E = double;
            arma_fortran(arma_dgbmv)(&TRAN, &M, &N, &KL, &KU, reinterpret_cast<E*>(&ALPHA), reinterpret_cast<E*>(this->memptr()), &LDA, (E*)X.memptr(), &INC, &BETA, reinterpret_cast<E*>(Y.memptr()), &INC);
        }

        return Y;
    }

    return X;
}

template <typename T>
Mat<T> BandMat<T>::solve(const Mat<T>& B) {
    Mat<T> X;
    if(solve(X, B) != 0) X.reset();
    return X;
}

template <typename T>
int BandMat<T>::solve(Mat<T>& X, const Mat<T>& B) {
    X = B;

    int N = n_cols;
    int KL = low_bw;
    int KU = up_bw;
    auto NRHS = static_cast<int>(B.n_cols);
    int LDAB = n_rows;
    auto LDB = static_cast<int>(B.n_rows);
    const auto IPIV = new int[N];
    auto INFO = 0;

    if(std::is_same<T, float>::value) {
        using E = float;
        arma_fortran(arma_sgbsv)(&N, &KL, &KU, &NRHS, (E*)this->memptr(), &LDAB, IPIV, (E*)X.memptr(), &LDB, &INFO);
    } else if(std::is_same<T, double>::value) {
        using E = double;
        arma_fortran(arma_dgbsv)(&N, &KL, &KU, &NRHS, (E*)this->memptr(), &LDAB, IPIV, (E*)X.memptr(), &LDB, &INFO);
    }

    delete[] IPIV;

    return INFO;
}

#endif

//! @}
