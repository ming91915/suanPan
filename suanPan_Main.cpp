#include <suanPan>

void example_symm_mat();

int main(int argc, char** argv) {
    wall_clock T;
    T.tic();

    argument_parser(argc, argv);

    cout << T.toc() << "\n";

    return 0;
}

void example_symm_mat() {
    const auto N = 10;
    mat A(N, N, fill::randn);
    A = A + A.t();
    const symm_mat B(A);

    const mat D(10, 5, fill::randn);

    (A * D).print();
    cout << endl;
}
