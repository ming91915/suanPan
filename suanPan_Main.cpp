#include "H5Cpp.h"
#include <Toolbox/commandParser.h>
#include <suanPan>

using namespace H5;

void example_symm_mat();
void example_file();

int main(int argc, char** argv)
{
    argumentParser(argc, argv);

    wall_clock T;
    T.tic();

    example_file();

    return 0;
}

void example_file()
{
    auto D = make_shared<Domain>();
    process_file(D, "A.tcl");

    D->summary();

    Static S(1, make_shared<Newton>(D, make_shared<AbsResidual>(1E-4)));

    auto INFO = S.analyze();
    if(INFO != 0) cout << INFO << endl;

    cout << endl;
    D->getNode(6)->getCurrentDisplacement().print();
}

void example_symm_mat()
{
    using symm_mat = SymmMat<double>;

    auto N = 10;
    symm_mat A(N, N, fill::zeros);
    mat B(10, 10);
    auto C = A.memptr();
    auto idx = N * (N + 1) / 2;
    auto idx2 = idx;
    for(auto I = 0; I < idx2; ++I) C[I] = idx--;
    A += 100;
    for(auto I = 0; I < N; I++) {
        for(auto J = 0; J < N; J++) cout << A(I, J) << "\t";
        cout << endl;
    }
}