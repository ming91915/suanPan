
#ifndef SYMMPACKEDMAT_HPP
#define SYMMPACKEDMAT_HPP

template <typename T>
class SymmPackedMat : public MetaMat<T> {
    static const auto SIDE = 'R';
    static const auto UPLO = 'U';
    static const auto TRAN = 'N';

public:
    using MetaMat<T>::n_cols;
    using MetaMat<T>::n_rows;
    using MetaMat<T>::n_elem;
    using MetaMat<T>::memory;

    explicit SymmPackedMat(const unsigned& in_size)
        : MetaMat<T>(in_size, in_size, (in_size + 1) * in_size / 2) {}

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
const T& SymmPackedMat<T>::operator()(const unsigned& in_row, const unsigned& in_col) const {
    return memory[in_col > in_row ? (in_col * in_col + in_col) / 2 + in_row : (in_row * in_row + in_row) / 2 + in_col];
}

template <typename T>
const T& SymmPackedMat<T>::at(const unsigned& in_row, const unsigned& in_col) const {
    return memory[in_col > in_row ? (in_col * in_col + in_col) / 2 + in_row : (in_row * in_row + in_row) / 2 + in_col];
}

template <typename T>
T& SymmPackedMat<T>::operator()(const unsigned& in_row, const unsigned& in_col) {
    return access::rw(memory[in_col > in_row ? (in_col * in_col + in_col) / 2 + in_row : (in_row * in_row + in_row) / 2 + in_col]);
}

template <typename T>
T& SymmPackedMat<T>::at(const unsigned& in_row, const unsigned& in_col) {
    return access::rw(memory[in_col > in_row ? (in_col * in_col + in_col) / 2 + in_row : (in_row * in_row + in_row) / 2 + in_col]);
}

template <typename T>
Mat<T> SymmPackedMat<T>::operator*(const Mat<T>& B) {
    auto Y = B;

    int M = n_rows;
    auto N = static_cast<int>(B.n_cols);
    T ALPHA = 1.;
    auto LDB = M;
    T BETA = 0.;
    auto LDC = M;

    if(std::is_same<T, float>::value) {
        using E = float;
        arma_fortran(arma_sspmm)(&SIDE, &UPLO, &TRAN, &M, &N, reinterpret_cast<E*>(this->memptr()), reinterpret_cast<E*>(&ALPHA), (E*)(B.memptr()), &LDB, reinterpret_cast<E*>(&BETA), reinterpret_cast<E*>(Y.memptr()), &LDC);
    } else if(std::is_same<T, double>::value) {
        using E = double;
        arma_fortran(arma_dspmm)(&SIDE, &UPLO, &TRAN, &M, &N, reinterpret_cast<E*>(this->memptr()), reinterpret_cast<E*>(&ALPHA), (E*)(B.memptr()), &LDB, reinterpret_cast<E*>(&BETA), reinterpret_cast<E*>(Y.memptr()), &LDC);
    }

    return Y;
}

template <typename T>
Col<T> SymmPackedMat<T>::operator*(const Col<T>& X) {
    auto Y = X;

    int N = n_rows;
    T ALPHA = 1.;
    auto INC = 1;
    T BETA = 0.;

    if(std::is_same<T, float>::value) {
        using E = float;
        arma_fortran(arma_sspmv)(&UPLO, &N, reinterpret_cast<E*>(&ALPHA), reinterpret_cast<E*>(this->memptr()), (E*)(X.memptr()), &INC, reinterpret_cast<E*>(&BETA), reinterpret_cast<E*>(Y.memptr()), &INC);
    } else if(std::is_same<T, double>::value) {
        using E = double;
        arma_fortran(arma_dspmv)(&UPLO, &N, reinterpret_cast<E*>(&ALPHA), reinterpret_cast<E*>(this->memptr()), (E*)(X.memptr()), &INC, reinterpret_cast<E*>(&BETA), reinterpret_cast<E*>(Y.memptr()), &INC);
    }

    return Y;
}

template <typename T>
Mat<T> SymmPackedMat<T>::solve(const Mat<T>& B) {
    Mat<T> X;

    if(!solve(X, B)) X.reset();

    return X;
}

template <typename T>
bool SymmPackedMat<T>::solve(Mat<T>& X, const Mat<T>& B) {
    X = B;

    int N = n_rows;
    auto NRHS = static_cast<int>(B.n_cols);
    const auto IPIV = new int[N];
    auto LDB = N;
    auto INFO = 0;

    if(std::is_same<T, float>::value) {
        using E = float;
        arma_fortran(arma_sspsv)(&UPLO, &N, &NRHS, reinterpret_cast<E*>(this->memptr()), IPIV, (E*)X.memptr(), &LDB, &INFO);
    } else if(std::is_same<T, double>::value) {
        using E = double;
        arma_fortran(arma_dspsv)(&UPLO, &N, &NRHS, reinterpret_cast<E*>(this->memptr()), IPIV, (E*)X.memptr(), &LDB, &INFO);
    }

    delete[] IPIV;

    return INFO == 0;
}
#endif
