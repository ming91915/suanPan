
#ifndef BANDSYMMMAT_HPP
#define BANDSYMMMAT_HPP

template <typename T>
class BandSymmMat : public MetaMat<T> {
    static const char UPLO = 'L';

public:
    using MetaMat<T>::n_cols;
    using MetaMat<T>::n_rows;
    using MetaMat<T>::n_elem;
    using MetaMat<T>::memory;

    BandSymmMat(const unsigned& in_size, const unsigned& in_bandwidth)
        : MetaMat<T>(in_bandwidth + 1, in_size, (in_bandwidth + 1) * in_size) {}

    const T& operator()(const unsigned& in_row, const unsigned& in_col) const override;
    const T& at(const unsigned& in_row, const unsigned& in_col) const override;
    T& operator()(const unsigned& in_row, const unsigned& in_col) override;
    T& at(const unsigned& in_row, const unsigned& in_col) override;
    Mat<T> operator*(const Mat<T>& B)override;
    Col<T> operator*(const Col<T>& X)override;
    Mat<T> solve(const Mat<T>& B) override;
    bool solve(Mat<T>& X, const Mat<T>& B) override;
};

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
Mat<T> BandSymmMat<T>::operator*(const Mat<T>& B) {
    return B;
}

template <typename T>
Col<T> BandSymmMat<T>::operator*(const Col<T>& X) {
    auto Y = X;

    int N = n_cols;
    int K = n_rows - 1;
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

template <typename T>
Mat<T> BandSymmMat<T>::solve(const Mat<T>& B) {
    Mat<T> X;

    if(!solve(X, B)) X.reset();

    return X;
}

template <typename T>
bool BandSymmMat<T>::solve(Mat<T>& X, const Mat<T>& B) {
    X = B;

    int N = n_cols;
    int KD = n_rows - 1;
    auto NRHS = static_cast<int>(B.n_cols);
    auto LDAB = n_rows;
    auto LDB = static_cast<int>(B.n_rows);
    auto INFO = 0;

    if(std::is_same<T, float>::value) {
        using E = float;
        arma_fortran(arma_spbsv)(&UPLO, &N, &KD, &NRHS, reinterpret_cast<E*>(this->memptr()), &LDAB, reinterpret_cast<E*>(X.memptr()), &LDB, &INFO);
    } else if(std::is_same<T, double>::value) {
        using E = double;
        arma_fortran(arma_dpbsv)(&UPLO, &N, &KD, &NRHS, reinterpret_cast<E*>(this->memptr()), &LDAB, reinterpret_cast<E*>(X.memptr()), &LDB, &INFO);
    }

    return INFO == 0;
}
#endif
