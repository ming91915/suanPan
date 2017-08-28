#include "H5Cpp.h"
#include <suanPan>

using namespace H5;

void example_symm_mat();

int main(int argc, char** argv)
{
#ifdef SUANPAN_DEBUG
    wall_clock T;
    T.tic();
#endif

    argument_parser(argc, argv);
	// example_symm_mat();

#ifdef SUANPAN_DEBUG
    cout << T.toc() << "\n";
#endif

    return 0;
}

void example_symm_mat()
{
    const auto N = 10;
    SymmMat<double> A(N, fill::zeros);
    Mat<double> B(10, 10);
    const auto C = A.memptr();
    auto idx = N * (N + 1) / 2;
    const auto idx2 = idx;
    for(auto i = 0; i < idx2; ++i) C[i] = idx--;
    A += 10;
    for(auto i = 0; i < N; i++) {
        for(auto j = 0; j < N; j++) {
            cout << A(i, j) << "\t";
            B(i, j) = A(i, j);
        }
        cout << endl;
    }

    const vec D(10, fill::randn);

    cout << endl;

    SymmMat<double> E = inv_sp(A);
}