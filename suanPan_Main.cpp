#include <Toolbox/commandParser.h>
#include <suanPan>

void example_symm_mat();
void example_uni_material();

int main(int argc, char** argv)
{
    argumentParser(argc, argv);

    wall_clock T;
    T.tic();

    // auto D = make_shared<Domain>();
    // D->insert(make_shared<Elastic2D>(static_cast<unsigned>(1), 1, 1. / 3.));
    // process_file(D, "W.tcl");

    // cout << "The model has " << D->getNumberNode() << " nodes.\n";
    // cout << "The model has " << D->getNumberElement() << " elements.\n";

    // Newton S(D, make_shared<AbsResidual>(1E-4));
    // S.enable_band();
    // S.enable_symm();
    // S.setStepSize(.01, 0, 1);
    // S.initialize();

    // auto INFO = S.analyze(1);
    // if(INFO != 0) cout << INFO << endl;

    // cout << "\n";
    // D->getNode(1326)->getCurrentDisplacement().print();

    example_uni_material();

    ExternalModule A("ElementExample");
    if(A.locate_module()) {
        unique_ptr<Element> new_elemnt = nullptr;
        istringstream command("1 1 2 3 1 1");
        A.new_object(new_elemnt, command);
        new_elemnt->print();
    }

    cout << endl << T.toc() << endl;

    return 0;
}

void example_uni_material()
{
    // RambergOsgood A;
    Gap01 A(0, 2E5, 400, 0.001);
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