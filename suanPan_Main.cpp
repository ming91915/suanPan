#include <suanPan>

auto invalid_code = static_cast<unsigned>(-1);

using symm_mat = SymmMat<double>;

int main(int argc, char** argv)
{
    argumentParser(argc, argv);

    wall_clock T;
    T.tic();

    // auto D = make_shared<Domain>();
    // D->insert(make_shared<Elastic2D>(static_cast<unsigned>(1), 2E4, 0.2));
    // D->insert(make_shared<Node>(1, vec({ 0, 0 })));
    // D->insert(make_shared<Node>(2, vec({ 1, 0 })));
    // D->insert(make_shared<Node>(3, vec({ 1, 1 })));
    // D->insert(make_shared<Node>(4, vec({ 0, 1 })));
    // D->insert(make_shared<GQ12>(1, uvec({ 1, 2, 3, 4 }), 1, 1));
    // D->insert(make_shared<BC>(1, 0, uvec({ 1, 4 }), "p"));
    // D->insert(make_shared<CLoad>(1, 0, 20000, uvec({ 2, 3 }), 1));
    // Newton S(D, make_shared<AbsResidual>(1E-4));
    // S.initialize();
    // S.analyze(1);
    // cout << "\n";
    // D->getNode(2)->getCurrentDisplacement().print();

    example_CP4();

    // auto N = 10;
    // symm_mat A(N, N, fill::zeros);
    // mat B(10, 10);
    // auto C = A.memptr();
    // auto idx = N * (N + 1) / 2;
    // auto idx2 = idx;
    // for(auto I = 0; I < idx2; ++I) C[I] = idx--;
    // A += 100;
    // for(auto I = 0; I < N; I++) {
    //    for(auto J = 0; J < N; J++) cout << A(I, J) << "\t";
    //    cout << endl;
    //}

    cout << endl << T.toc() << endl;

    return 0;
}