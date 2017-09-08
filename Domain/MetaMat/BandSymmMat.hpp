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

template <typename T>
class BandSymmMat : public MetaMat<T> {
    const char UPLO = 'L';
    const unsigned bw;

public:
    using MetaMat<T>::n_cols;
    using MetaMat<T>::n_rows;
    using MetaMat<T>::n_elem;
    using MetaMat<T>::memory;
    using MetaMat<T>::solve;
    using MetaMat<T>::solve_trs;

    BandSymmMat();
    BandSymmMat(const unsigned&, const unsigned&);

    const T& operator()(const unsigned&, const unsigned&) const override;
    const T& at(const unsigned&, const unsigned&) const override;
    T& operator()(const unsigned&, const unsigned&) override;
    T& at(const unsigned&, const unsigned&) override;

    Mat<T> operator*(const Mat<T>&)override;

    int solve(Mat<T>&, const Mat<T>&) override;
    int solve_trs(Mat<T>&, const Mat<T>&) override;

    MetaMat<T> i() override;
    MetaMat<T> inv() override;
};

template <typename T>
struct is_BandSymm {
    static const bool value = false;
};

template <typename T>
struct is_BandSymm<BandSymmMat<T>> {
    static const bool value = true;
};

template <typename T>
BandSymmMat<T>::BandSymmMat()
    : MetaMat<T>()
    , bw(0) {}

template <typename T>
BandSymmMat<T>::BandSymmMat(const unsigned& in_size, const unsigned& in_bandwidth)
    : MetaMat<T>(in_bandwidth + 1, in_size, (in_bandwidth + 1) * in_size)
    , bw(in_bandwidth) {}

template <typename T>
const T& BandSymmMat<T>::operator()(const unsigned& in_row, const unsigned& in_col) const {
    return memory[in_row > in_col ? in_row - in_col + in_col * n_rows : in_col - in_row + in_row * n_rows];
}

template <typename T>
const T& BandSymmMat<T>::at(const unsigned& in_row, const unsigned& in_col) const {
    return memory[in_row > in_col ? in_row - in_col + in_col * n_rows : in_col - in_row + in_row * n_rows];
}

template <typename T>
T& BandSymmMat<T>::operator()(const unsigned& in_row, const unsigned& in_col) {
    return access::rw(memory[in_row > in_col ? in_row - in_col + in_col * n_rows : in_col - in_row + in_row * n_rows]);
}

template <typename T>
T& BandSymmMat<T>::at(const unsigned& in_row, const unsigned& in_col) {
    return access::rw(memory[in_row > in_col ? in_row - in_col + in_col * n_rows : in_col - in_row + in_row * n_rows]);
}

template <typename T>
Mat<T> BandSymmMat<T>::operator*(const Mat<T>& X) {
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
            arma_fortran(arma_ssbmv)(&UPLO, &N, &K, reinterpret_cast<E*>(&ALPHA), reinterpret_cast<E*>(this->memptr()), &LDA, (E*)X.memptr(), &INC, reinterpret_cast<E*>(&BETA), reinterpret_cast<E*>(Y.memptr()), &INC);
        } else if(std::is_same<T, double>::value) {
            using E = double;
            arma_fortran(arma_dsbmv)(&UPLO, &N, &K, reinterpret_cast<E*>(&ALPHA), reinterpret_cast<E*>(this->memptr()), &LDA, (E*)X.memptr(), &INC, reinterpret_cast<E*>(&BETA), reinterpret_cast<E*>(Y.memptr()), &INC);
        }

        return Y;
    }

    return X;
}

template <typename T>
int BandSymmMat<T>::solve(Mat<T>& X, const Mat<T>& B) {
    X = B;

    int N = n_cols;
    int KD = bw;
    auto NRHS = static_cast<int>(B.n_cols);
    int LDAB = n_rows;
    auto LDB = static_cast<int>(B.n_rows);
    auto INFO = 0;

    if(std::is_same<T, float>::value) {
        using E = float;
        arma_fortran(arma_spbsv)(&UPLO, &N, &KD, &NRHS, (E*)this->memptr(), &LDAB, (E*)X.memptr(), &LDB, &INFO);
    } else if(std::is_same<T, double>::value) {
        using E = double;
        arma_fortran(arma_dpbsv)(&UPLO, &N, &KD, &NRHS, (E*)this->memptr(), &LDAB, (E*)X.memptr(), &LDB, &INFO);
    }

    return INFO;
}

template <typename T>
int BandSymmMat<T>::solve_trs(Mat<T>& X, const Mat<T>& B) {
    X = B;

    int N = n_cols;
    int KD = bw;
    auto NRHS = static_cast<int>(B.n_cols);
    int LDAB = n_rows;
    auto LDB = static_cast<int>(B.n_rows);
    auto INFO = 0;

    if(std::is_same<T, float>::value) {
        using E = float;
        arma_fortran(arma_spbtrs)(&UPLO, &N, &KD, &NRHS, (E*)this->memptr(), &LDAB, (E*)X.memptr(), &LDB, &INFO);
    } else if(std::is_same<T, double>::value) {
        using E = double;
        arma_fortran(arma_dpbtrs)(&UPLO, &N, &KD, &NRHS, (E*)this->memptr(), &LDAB, (E*)X.memptr(), &LDB, &INFO);
    }

    return INFO;
}

template <typename T>
MetaMat<T> BandSymmMat<T>::i() {
    // ReSharper disable once CppStaticAssertFailure
    static_assert(false, "Inverse is not available for band symmetric matrix.");
    MetaMat<T> X;
    return X;
}

template <typename T>
MetaMat<T> BandSymmMat<T>::inv() {
    // ReSharper disable once CppStaticAssertFailure
    static_assert(false, "Inverse is not available for band symmetric matrix.");
    MetaMat<T> X;
    return X;
}

#endif

//! @}
