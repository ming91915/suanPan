#pragma once

template <typename T1, typename T2>
MetaMat<T1, T2>& operator*(const T1& value, MetaMat<T1, T2>& M) {
    arrayops::inplace_mul(M.memptr(), value, M.n_elem);
    return M;
}

template <typename T1, typename T2>
std::enable_if_t<is_Full<T2>::value, Mat<T1>> operator*(const Mat<T1>& A, const MetaMat<T1, T2>& B) {
    Mat<T1> C(A.n_rows, A.n_cols);

    const auto TRAN = 'N';

    auto M = static_cast<int>(A.n_rows);
    auto N = static_cast<int>(B.n_cols);
    auto K = static_cast<int>(A.n_cols);
    T1 ALPHA = 1.0;
    auto LDA = M;
    auto LDB = K;
    T1 BETA = 0.;
    auto LDC = M;

    if(std::is_same<T1, float>::value) {
        using E = float;
        arma_fortran(arma_sgemm)(&TRAN, &TRAN, &M, &N, &K, reinterpret_cast<E*>(&ALPHA), (E*)(A.memptr()), &LDA, (E*)(B.memptr()), &LDB, reinterpret_cast<E*>(&BETA), reinterpret_cast<E*>(C.memptr()), &LDC);
    } else if(std::is_same<T1, double>::value) {
        using E = double;
        arma_fortran(arma_dgemm)(&TRAN, &TRAN, &M, &N, &K, reinterpret_cast<E*>(&ALPHA), (E*)(A.memptr()), &LDA, (E*)(B.memptr()), &LDB, reinterpret_cast<E*>(&BETA), reinterpret_cast<E*>(C.memptr()), &LDC);
    }

    return C;
}

template <const char S, const char T, typename T1, typename T2>
std::enable_if_t<is_SymmPack<T2>::value, Mat<T1>> spmm(const MetaMat<T1, T2>& A, const Mat<T1>& B) {
    Mat<T1> C;

    auto SIDE = S;
    auto TRAN = T;
    auto UPLO = 'U';

    auto M = static_cast<int>(A.n_rows);

    auto PT = 0;
    if(SIDE == 'L') PT += 1;
    if(TRAN == 'T') PT += 10;

    int N, LDC;

    switch(PT) {
    case 0: // A*B
        N = static_cast<int>(B.n_cols);
        C.set_size(M, N);
        LDC = M;
        break;
    case 1: // B*A
        N = static_cast<int>(B.n_rows);
        C.set_size(N, M);
        LDC = N;
        break;
    case 10: // A*B**T
        N = static_cast<int>(B.n_rows);
        C.set_size(M, N);
        LDC = M;
        break;
    case 11: // B**T*A
        N = static_cast<int>(B.n_cols);
        C.set_size(N, M);
        LDC = N;
        break;
    default:
        break;
    }

    T1 ALPHA = 1.0;
    auto LDB = static_cast<int>(B.n_rows);
    T1 BETA = 0.;

    if(std::is_same<T1, float>::value) {
        using E = float;
        arma_fortran(arma_sspmm)(&SIDE, &UPLO, &TRAN, &M, &N, (E*)(A.memptr()), reinterpret_cast<E*>(&ALPHA), (E*)(B.memptr()), &LDB, reinterpret_cast<E*>(&BETA), reinterpret_cast<E*>(C.memptr()), &LDC);
    } else if(std::is_same<T1, double>::value) {
        using E = double;
        arma_fortran(arma_dspmm)(&SIDE, &UPLO, &TRAN, &M, &N, (E*)(A.memptr()), reinterpret_cast<E*>(&ALPHA), (E*)(B.memptr()), &LDB, reinterpret_cast<E*>(&BETA), reinterpret_cast<E*>(C.memptr()), &LDC);
    }

    return C;
}

template <typename T1, typename T2>
std::enable_if_t<is_SymmPack<T2>::value, Mat<T1>> operator*(const Mat<T1>& A, const MetaMat<T1, T2>& B) {
    return spmm<'L', 'N'>(B, A);
}
