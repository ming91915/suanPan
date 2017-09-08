#include <suanPan>

void example_symm_mat();

int main(int argc, char** argv) {
    wall_clock T;
    T.tic();

    // argument_parser(argc, argv);

    example_symm_mat();

    cout << T.toc() << "\n";
    return 0;
}

using symm_mat = SymmMat<double>;

void example_symm_mat() {
    const auto N = 10;
    mat A(N, N, fill::randn);
    A = A + A.t();
    const symm_mat B(A);

    shared_ptr<MetaMat<double>> C = make_shared<SymmPackMat<double>>(N);
    memcpy(C->memptr(), B.memptr(), B.n_elem * sizeof(double));

    // SymmPackMat<double> C(N);
    // memcpy(C.memptr(), B.memptr(), B.n_elem * sizeof(double));

    FullMat<double> E(10);
    memcpy(E.memptr(), A.memptr(), A.n_elem * sizeof(double));

    auto F = E;
    const mat D(N, 5, fill::randn);

    auto& K = *C;
    C->solve(D).print();
    cout << endl;
    cout << K(2, 2) << endl;
    C->solve_trs(D).print();
    cout << endl;
    E.solve(D).print();
    cout << endl;
    E.solve_trs(D).print();

    cout << endl;
}
