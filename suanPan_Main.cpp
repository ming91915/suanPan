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
    example_Newmark();

// argument_parser(argc, argv);

// example_symm_mat();

#ifdef SUANPAN_DEBUG
    cout << T.toc() << "\n";
#endif

    return 0;
}

void example_symm_mat()
{
    const auto N = 10;
    symm_mat A(N, fill::zeros);
    mat B(10, 10);
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

    (B * D).print();

    cout << endl;

    (A * D).print();
}