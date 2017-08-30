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
    Factory<uword, Mat<uword>> A(3, AnalysisType::DISP);
    A.initialize();

    A.get_trial_displacement().print();

    A.print();

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
    const symm_mat B(A);

    A.i().print();

    cout << endl;

    symm_mat C = inv(B);
    C.print();

    const vec D(10, fill::randn);

    (A * D).print();

    (B * D).print();
}