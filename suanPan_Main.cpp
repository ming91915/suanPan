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
    factory A(3, AnalysisType::DISP);
    A.initialize();

    A.print();

#ifdef SUANPAN_DEBUG
    cout << T.toc() << "\n";
#endif

    return 0;
}

void example_symm_mat()
{
    const auto N = 10;
    fmat A(N, N, fill::randn);
    A = A + A.t();
    const symm_fmat B(A);

    A.i().print();
    cout << endl;

    symm_fmat C = inv(B);
    C.print();
    cout << endl;

    const fvec D(10, fill::randn);

    (A * D).print();
    cout << endl;
    (B * D).print();
    cout << endl;

    const auto E = solve_sm(B, D);

    fvec F;
    glue_solve_symm::apply(F, E);

    F.print();
    cout << endl;
    solve(A, D).print();
}