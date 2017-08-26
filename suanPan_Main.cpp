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
    auto N = 10;
    symm_mat A(N, fill::zeros);
    mat B(10, 10);
    auto C = A.memptr();
    auto idx = N * (N + 1) / 2;
    auto idx2 = idx;
    for(auto i = 0; i < idx2; ++i) C[i] = idx--;
    A += 10;
    for(auto i = 0; i < N; i++) {
        for(auto j = 0; j < N; j++) {
            cout << A(i, j) << "\t";
            B(i, j) = A(i, j);
        }
        cout << endl;
    }

    vec D(10, fill::randn);

    vec E = B * D;

    E.print();

    cout << endl;

    vec HH = A * D;

    HH.print();

    vec K;
    K.resize(3);
    K.print();
}