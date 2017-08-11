#include <suanPan>

void example_symm_mat();
void example_RO();

int main(int argc, char** argv)
{
    argumentParser(argc, argv);

    wall_clock T;
    T.tic();

	example_symm_mat();

    cout << endl << T.toc() << endl;

    return 0;
}

void example_RO()
{
    RambergOsgood A;
    vector<double> B, C;
    for(auto I = 0; I < 30; ++I) {
        A.updateIncreStatus({ 0.0001 });
        A.commitStatus();
        B.push_back(as_scalar(A.getStrain()));
        C.push_back(as_scalar(A.getStress()));
    }
    for(auto I = 0; I < 50; ++I) {
        A.updateIncreStatus({ -0.0001 });
        A.commitStatus();
        B.push_back(as_scalar(A.getStrain()));
        C.push_back(as_scalar(A.getStress()));
    }
    for(auto I = 0; I < 50; ++I) {
        A.updateIncreStatus({ 0.0001 });
        A.commitStatus();
        B.push_back(as_scalar(A.getStrain()));
        C.push_back(as_scalar(A.getStress()));
    }

    mat D(B.size(), 2);
    D.col(0) = vec(B);
    D.col(1) = vec(C);

    D.save("D", raw_ascii);
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