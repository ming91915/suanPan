#include <suanPan>

void example_symm_mat();

int main(int argc, char** argv) {
    wall_clock T;
    T.tic();

    // argument_parser(argc, argv);

    // example_symm_mat();

    const auto N = 10;

    mat A;
    A.load("A", raw_ascii);
    mat B = eye(N, N);
    B(3, 3) = 4;

    // mat A = { { 2, -1, 0, 0 }, { -1, 4, -2, 0 }, { 0, -2, 2, 0 }, { 0, 0, 0, 2 } };
    // mat B = { { 1, 0, 0, 0 }, { 0, 3, 0, 0 }, { 0, 0, 1, 0 }, { 0, 0, 0, 2 } };

    vec E;
    mat F;

    mat R = chol(B);
    mat AA = inv(R) * A * inv(R);
    cout << eig_solve(E, F, AA, 4, "LM") << endl;

    cx_vec C;
    cx_mat D;
    eig_pair(C, D, A, B);
    C.print("\n");

    E.print("\n");

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

    C->solve(D).print();
    cout << endl;
    C->solve_trs(D).print();
    cout << endl;
    E.solve(D).print();
    cout << endl;
    E.solve_trs(D).print();

    cout << endl;
}
