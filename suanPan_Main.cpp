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

    // argument_parser(argc, argv);
    example_symm_mat();

#ifdef SUANPAN_DEBUG
    cout << T.toc() << "\n";
#endif

    return 0;
}

void example_symm_mat()
{
    const auto N = 10;
    mat A(N, N, fill::randn);
    A = A + A.t();
    SymmMat<double> B(A);

    A.i().print();

    cout << endl;

    SymmMat<double> E = inv(B);
    E.print();
}