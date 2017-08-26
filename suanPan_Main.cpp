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

    // argumentParser(argc, argv);

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
    for(auto I = 0; I < idx2; ++I) C[I] = idx--;
    A += 10;
    for(auto I = 0; I < N; I++) {
        for(auto J = 0; J < N; J++) {
            cout << A(I, J) << "\t";
            B(I, J) = A(I, J);
        }
        cout << endl;
    }

    vec D(10, fill::randn);

    vec E = B * D;

    E.print();

    cout << endl;

    vec HH = A * D;

    HH.print();
}