#include "H5Cpp.h"
#include <suanPan>

using namespace H5;

void example_symm_mat();

int main(int argc, char** argv)
{
#ifndef SUANPAN_DEBUG
    wall_clock T;
    T.tic();
#endif

    argument_parser(argc, argv);

// example_symm_mat();
// factory A(3, AnalysisType::DISP);
// A.initialize();

// A.print();

#ifndef SUANPAN_DEBUG
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
    cout << endl;

    const vec D(10, fill::randn);

    (A * D).print();
    cout << endl;
    (B * D).print();
    cout << endl;

    const auto E = solve(B, D);

    vec F;
    glue_solve_symm::apply(F, E);

    F.print();
    cout << endl;
    solve(A, D).print();
}