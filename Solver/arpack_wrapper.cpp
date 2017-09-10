#include "arpack_wrapper.h"

int eig_solve(cx_vec& eigval, cx_mat& eigvec, mat& K, const unsigned& num, char* form) {
    auto IDO = 0;
    auto BMAT = 'I'; // standard eigenvalue problem A*x=lambda*x
    auto N = static_cast<int>(K.n_rows);
    const auto WHICH = form;
    auto NEV = std::min(static_cast<int>(num), N - 2);
    auto TOL = 0.;
    auto NCV = std::min(std::min(NEV + 8, 2 * NEV + 1), N);
    auto LDV = N;
    auto LWORKL = 3 * NCV * (NCV + 2);
    auto INFO = 0;

    podarray<int> IPARAM(11), IPNTR(14);
    podarray<double> RESID(N), V(N * NCV), WORKD(3 * N), WORKL(LWORKL);

    IPARAM(0) = 1;   // exact shift
    IPARAM(2) = 500; // maximum iteration
    IPARAM(6) = 1;   // mode 1: A*x=lambda*x

    while(IDO != 99) {
        arma_fortran(arma_dnaupd)(&IDO, &BMAT, &N, WHICH, &NEV, &TOL, RESID.memptr(), &NCV, V.memptr(), &LDV, IPARAM.memptr(), IPNTR.memptr(), WORKD.memptr(), WORKL.memptr(), &LWORKL, &INFO);
        if(IDO == 1 || IDO == -1) {
            const vec X(WORKD.memptr() + IPNTR[0] - 1, N, false);
            vec Y(WORKD.memptr() + IPNTR[1] - 1, N, false);
            Y = K * X;
        }
    }

    if(INFO != 0) return INFO;

    auto RVEC = 1;
    auto HOWMNY = 'A';
    auto LDZ = N;
    auto SIGMAR = 0.;
    auto SIGMAI = 0.;

    podarray<int> SELECT(NCV);
    podarray<double> DR(NEV + 1), DI(NEV + 1), Z(N * (NEV + 1)), WORKEV(3 * NCV);

    arma_fortran(arma_dneupd)(&RVEC, &HOWMNY, SELECT.memptr(), DR.memptr(), DI.memptr(), Z.memptr(), &LDZ, &SIGMAR, &SIGMAI, WORKEV.memptr(), &BMAT, &N, WHICH, &NEV, &TOL, RESID.memptr(), &NCV, V.memptr(), &LDV, IPARAM.memptr(), IPNTR.memptr(), WORKD.memptr(), WORKL.memptr(), &LWORKL, &INFO);

    eigval.set_size(NEV);
    eigvec.set_size(N, NEV);
    using T = double;
    for(auto I = 0; I < NEV; ++I) eigval(I) = std::complex<T>(DR(I), DI(I));
    for(auto I = 0; I < NEV; ++I) {
        if(I < NEV - 1 && eigval[I] == std::conj(eigval[I + 1])) {
            for(auto J = 0; J < N; ++J) {
                eigvec.at(J, I) = std::complex<T>(Z[N * I + J], Z[N * (I + 1) + J]);
                eigvec.at(J, I + 1) = std::complex<T>(Z[N * I + J], -Z[N * (I + 1) + J]);
            }
            ++I;
        } else if(I == NEV - 1 && std::complex<T>(eigval[I]).imag() != 0.)
            for(auto J = 0; J < N; ++J) eigvec.at(J, I) = std::complex<T>(Z[N * I + J], Z[N * (I + 1) + J]);
        else
            for(auto J = 0; J < N; ++J) eigvec.at(J, I) = std::complex<T>(Z[N * I + J], 0.);
    }

    return INFO;
}

int eig_solve(cx_vec& eigval, cx_mat& eigvec, mat& K, mat& M, const unsigned& num, char* form) {
    auto IDO = 0;
    auto BMAT = 'G'; // generalized eigenvalue problem A*x=lambda*B*x
    auto N = static_cast<int>(K.n_rows);
    const auto WHICH = form;
    auto NEV = std::min(static_cast<int>(num), N - 2);
    auto TOL = 0.;
    auto NCV = std::min(std::min(NEV + 8, 2 * NEV + 1), N);
    auto LDV = N;
    auto LWORKL = 3 * NCV * (NCV + 2);
    auto INFO = 0;

    podarray<int> IPARAM(11), IPNTR(14);
    podarray<double> RESID(N), V(N * NCV), WORKD(3 * N), WORKL(LWORKL);

    IPARAM(0) = 1;   // exact shift
    IPARAM(2) = 500; // maximum iteration
    IPARAM(6) = 2;   // mode 2: A*x=lambda*M*x

    while(IDO != 99) {
        arma_fortran(arma_dnaupd)(&IDO, &BMAT, &N, WHICH, &NEV, &TOL, RESID.memptr(), &NCV, V.memptr(), &LDV, IPARAM.memptr(), IPNTR.memptr(), WORKD.memptr(), WORKL.memptr(), &LWORKL, &INFO);
        vec X(WORKD.memptr() + IPNTR[0] - 1, N, false);
        vec Y(WORKD.memptr() + IPNTR[1] - 1, N, false);
        if(IDO == 1 || IDO == -1) {
            X = K * X;
            Y = solve(M, X);
        } else if(IDO == 2)
            Y = M * X;
    }

    if(INFO != 0) return INFO;

    auto RVEC = 1;
    auto HOWMNY = 'A';
    auto LDZ = N;
    auto SIGMAR = 0.;
    auto SIGMAI = 0.;

    podarray<int> SELECT(NCV);
    podarray<double> DR(NEV + 1), DI(NEV + 1), Z(N * (NEV + 1)), WORKEV(3 * NCV);

    arma_fortran(arma_dneupd)(&RVEC, &HOWMNY, SELECT.memptr(), DR.memptr(), DI.memptr(), Z.memptr(), &LDZ, &SIGMAR, &SIGMAI, WORKEV.memptr(), &BMAT, &N, WHICH, &NEV, &TOL, RESID.memptr(), &NCV, V.memptr(), &LDV, IPARAM.memptr(), IPNTR.memptr(), WORKD.memptr(), WORKL.memptr(), &LWORKL, &INFO);

    eigval.set_size(NEV);
    eigvec.set_size(N, NEV);
    using T = double;
    for(auto I = 0; I < NEV; ++I) eigval(I) = std::complex<T>(DR(I), DI(I));
    for(auto I = 0; I < NEV; ++I) {
        if(I < NEV - 1 && eigval[I] == std::conj(eigval[I + 1])) {
            for(auto J = 0; J < N; ++J) {
                eigvec.at(J, I) = std::complex<T>(Z[N * I + J], Z[N * (I + 1) + J]);
                eigvec.at(J, I + 1) = std::complex<T>(Z[N * I + J], -Z[N * (I + 1) + J]);
            }
            ++I;
        } else if(I == NEV - 1 && std::complex<T>(eigval[I]).imag() != 0.)
            for(auto J = 0; J < N; ++J) eigvec.at(J, I) = std::complex<T>(Z[N * I + J], Z[N * (I + 1) + J]);
        else
            for(auto J = 0; J < N; ++J) eigvec.at(J, I) = std::complex<T>(Z[N * I + J], 0.);
    }

    return INFO;
}

int eig_solve(vec& eigval, mat& eigvec, mat& K, const unsigned& num, char* form) {
    auto IDO = 0;
    auto BMAT = 'I'; // standard eigenvalue problem A*x=lambda*x
    auto N = static_cast<int>(K.n_rows);
    const auto WHICH = form;
    auto NEV = std::min(static_cast<int>(num), N - 1);
    auto TOL = 0.;
    auto NCV = std::min(std::min(NEV + 8, 2 * NEV + 1), N);
    auto LDV = N;
    auto LWORKL = NCV * (NCV + 8);
    auto INFO = 0;

    podarray<int> IPARAM(11), IPNTR(14);
    podarray<double> RESID(N), V(N * NCV), WORKD(3 * N), WORKL(LWORKL);

    IPARAM(0) = 1;   // exact shift
    IPARAM(2) = 500; // maximum iteration
    IPARAM(6) = 1;   // mode 1: A*x=lambda*x

    while(IDO != 99) {
        arma_fortran(arma_dsaupd)(&IDO, &BMAT, &N, WHICH, &NEV, &TOL, RESID.memptr(), &NCV, V.memptr(), &LDV, IPARAM.memptr(), IPNTR.memptr(), WORKD.memptr(), WORKL.memptr(), &LWORKL, &INFO);
        if(IDO == 1 || IDO == -1) {
            const vec X(WORKD.memptr() + IPNTR[0] - 1, N, false);
            vec Y(WORKD.memptr() + IPNTR[1] - 1, N, false);
            Y = K * X;
        }
    }

    if(INFO != 0) return INFO;

    auto RVEC = 1;
    auto HOWMNY = 'A';
    auto LDZ = N;
    auto SIGMA = 0.;

    podarray<int> SELECT(NCV);

    eigval.set_size(NEV);
    eigvec.set_size(N, NEV);

    arma_fortran(arma_dseupd)(&RVEC, &HOWMNY, SELECT.memptr(), eigval.memptr(), eigvec.memptr(), &LDZ, &SIGMA, &BMAT, &N, WHICH, &NEV, &TOL, RESID.memptr(), &NCV, V.memptr(), &LDV, IPARAM.memptr(), IPNTR.memptr(), WORKD.memptr(), WORKL.memptr(), &LWORKL, &INFO);

    return INFO;
}

int eig_solve(vec& eigval, mat& eigvec, mat& K, mat& M, const unsigned& num, char* form) {
    auto IDO = 0;
    auto BMAT = 'G'; // generalized eigenvalue problem A*x=lambda*M*x
    auto N = static_cast<int>(K.n_rows);
    const auto WHICH = form;
    auto NEV = std::min(static_cast<int>(num), N - 1);
    auto TOL = 0.;
    auto NCV = std::min(std::min(NEV + 8, 2 * NEV + 1), N);
    auto LDV = N;
    auto LWORKL = NCV * (NCV + 8);
    auto INFO = 0;

    podarray<int> IPARAM(11), IPNTR(14);
    podarray<double> RESID(N), V(N * NCV), WORKD(3 * N), WORKL(LWORKL);

    IPARAM(0) = 1;   // exact shift
    IPARAM(2) = 500; // maximum iteration
    IPARAM(6) = 2;   // mode 2: A*x=lambda*M*x

    while(IDO != 99) {
        arma_fortran(arma_dsaupd)(&IDO, &BMAT, &N, WHICH, &NEV, &TOL, RESID.memptr(), &NCV, V.memptr(), &LDV, IPARAM.memptr(), IPNTR.memptr(), WORKD.memptr(), WORKL.memptr(), &LWORKL, &INFO);
        vec X(WORKD.memptr() + IPNTR[0] - 1, N, false);
        vec Y(WORKD.memptr() + IPNTR[1] - 1, N, false);
        if(IDO == 1 || IDO == -1) {
            X = K * X;
            Y = solve(M, X);
        } else if(IDO == 2)
            Y = M * X;
    }

    if(INFO != 0) return INFO;

    auto RVEC = 1;
    auto HOWMNY = 'A';
    auto LDZ = N;
    auto SIGMA = 0.;

    podarray<int> SELECT(NCV);

    eigval.set_size(NEV);
    eigvec.set_size(N, NEV);

    arma_fortran(arma_dseupd)(&RVEC, &HOWMNY, SELECT.memptr(), eigval.memptr(), eigvec.memptr(), &LDZ, &SIGMA, &BMAT, &N, WHICH, &NEV, &TOL, RESID.memptr(), &NCV, V.memptr(), &LDV, IPARAM.memptr(), IPNTR.memptr(), WORKD.memptr(), WORKL.memptr(), &LWORKL, &INFO);

    return INFO;
}