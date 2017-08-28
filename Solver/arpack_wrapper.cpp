#include "arpack_wrapper.h"

int eig_solve(vec& eigval, mat& eigvec, mat& K, mat& M, const unsigned& num, char* form)
{
    int IDO = 0;
    char BMAT = 'G';
    int N = int(K.n_rows);
    char WHICH[] = "SM";
    int NEV = num;
    double TOL = -1.;
    double* RESID = new double[N];
    int NCV = std::min(std::min(2 * NEV, NEV + 8), N);
    double* V = new double[N * NCV];
    int LDV = N;
    int IPARAM[11];
    IPARAM[0] = 1;
    IPARAM[2] = 500;
    IPARAM[6] = 2;
    int IPNTR[11];
    double* WORKD = new double[3 * N];
    int LWORKL = NCV * NCV + 8 * NCV;
    double* WORKL = new double[LWORKL];
    int INFO = 0;

    while(IDO != 99) {
        suanpan::dsaupd_(&IDO, &BMAT, &N, WHICH, &NEV, &TOL, RESID, &NCV, V, &LDV, IPARAM,
            IPNTR, WORKD, WORKL, &LWORKL, &INFO);
        switch(IDO) {
        case -1:
        case 1:
            break;
        case 2:
            break;
        default:
            break;
        }
    }

    delete[] WORKL;
    delete[] WORKD;
    delete[] V;
    delete[] RESID;

    return INFO;
}
