#ifndef TESTFUNCTIONS_H
#define TESTFUNCTIONS_H

#include <suanPan>

void example_ext_module()
{
    ExternalModule A("ElementExample");
    if(A.locate_module()) {
        unique_ptr<Element> new_elemnt = nullptr;
        istringstream command("1 1 2 3 1 1");
        A.new_object(new_elemnt, command);
        new_elemnt->print();
    }
}

void example_uni_material()
{
    // RambergOsgood A;
    Gap01 A(0, 2E5, 400, 0.001);
    vector<double> B, C;
    for(auto I = 0; I < 30; ++I) {
        A.update_incre_status({ 0.0001 });
        A.commit_status();
        B.push_back(as_scalar(A.get_strain()));
        C.push_back(as_scalar(A.get_stress()));
    }
    for(auto I = 0; I < 50; ++I) {
        A.update_incre_status({ -0.0001 });
        A.commit_status();
        B.push_back(as_scalar(A.get_strain()));
        C.push_back(as_scalar(A.get_stress()));
    }
    for(auto I = 0; I < 50; ++I) {
        A.update_incre_status({ 0.0001 });
        A.commit_status();
        B.push_back(as_scalar(A.get_strain()));
        C.push_back(as_scalar(A.get_stress()));
    }

    mat D(B.size(), 2);
    D.col(0) = vec(B);
    D.col(1) = vec(C);

    D.save("D", raw_ascii);
}

void example_ODE()
{
    DP45 E(make_shared<ODE_INSTANCE>(), make_shared<RelError>());
    E.initialize();
    auto& D = get_current_displacement(E.get_workroom());
    D(0) = 1;
    E.analyze(1);
    E.get_workroom()->get_trial_displacement().print();
}

void example_Newmark()
{
    auto B = make_shared<Workroom>(3, AnalysisType::DYNAMICS);
    B->initialize();
    mat M = eye(3, 3);
    M(1, 1) = 3;
    B->update_mass(M);
    mat K = { { 2, -1, 0 }, { -1, 4, -2 }, { 0, -2, 2 } };
    B->update_stiffness(K);
    B->update_current_acceleration({ 0, 0, 6 });
    B->update_trial_load({ 0, 0, 6 });
    auto W = make_shared<Domain>();
    W->set_workroom(B);
    Newmark C(W);
    for(auto i = 0; i < 10; ++i) {
        B->update_incre_time(0.363);
        C.update_resistance();
        C.update_stiffness();
        C.commit_status();
    }
    cout << B->get_current_time() << endl;
    B->get_current_displacement().print();
}

void example_Truss2D()
{
    auto D = make_shared<Domain>();

    D->insert(make_shared<Bilinear1D>(static_cast<unsigned>(1), 2E5, 1000, .4));

    D->insert(make_shared<Node>(1, vec({ 0, 0 })));
    D->insert(make_shared<Node>(2, vec({ 4, 0 })));
    D->insert(make_shared<Node>(3, vec({ 0, -3 })));

    D->insert(make_shared<Truss2D>(1, uvec({ 1, 2 }), 1, 10, false));
    D->insert(make_shared<Truss2D>(2, uvec({ 3, 2 }), 1, 10, false));
    D->insert(make_shared<Truss2D>(3, uvec({ 3, 1 }), 1, 10, false));

    D->insert(make_shared<BC>(1, 0, uvec({ 1 }), "p"));
    D->insert(make_shared<BC>(2, 0, uvec({ 3 }), 1));
    D->insert(make_shared<BC>(3, 0, uvec({ 3 }), 2));
    D->insert(make_shared<CLoad>(1, 0, 20000, uvec({ 2 }), 2));

    D->get_node(2)->get_current_displacement().print();
}

void example_CP3()
{
    auto D = make_shared<Domain>();
    D->insert(make_shared<Elastic2D>(static_cast<unsigned>(1), 2E5, 0.2, 1234));
    D->insert(make_shared<Node>(1, vec({ 0, 0 })));
    D->insert(make_shared<Node>(2, vec({ 4, 0 })));
    D->insert(make_shared<Node>(3, vec({ 0, -3 })));
    D->insert(make_shared<ElementTemplate>(1, uvec({ 1, 3, 2 }), 1, 1));
    D->insert(make_shared<BC>(1, 0, uvec({ 1, 3 }), "p"));
    D->insert(make_shared<CLoad>(1, 0, 20000, uvec({ 2 }), 2));

    D->get_node(2)->get_current_displacement().print();
}

void example_GQ12()
{
    auto D = make_shared<Domain>();
    D->insert(make_shared<Elastic2D>(static_cast<unsigned>(1), 3E4, .25));
    D->insert(make_shared<Node>(1, vec({ 0, 0 })));
    D->insert(make_shared<Node>(2, vec({ 12, 0 })));
    D->insert(make_shared<Node>(3, vec({ 24, 0 })));
    D->insert(make_shared<Node>(4, vec({ 36, 0 })));
    D->insert(make_shared<Node>(5, vec({ 48, 0 })));
    D->insert(make_shared<Node>(6, vec({ 48, 12 })));
    D->insert(make_shared<Node>(7, vec({ 36, 12 })));
    D->insert(make_shared<Node>(8, vec({ 24, 12 })));
    D->insert(make_shared<Node>(9, vec({ 12, 12 })));
    D->insert(make_shared<Node>(10, vec({ 0, 12 })));

    D->insert(make_shared<GQ12>(1, uvec({ 1, 2, 9, 10 }), 1, 1));
    D->insert(make_shared<GQ12>(2, uvec({ 2, 3, 8, 9 }), 1, 1));
    D->insert(make_shared<GQ12>(3, uvec({ 3, 4, 7, 8 }), 1, 1));
    D->insert(make_shared<GQ12>(4, uvec({ 4, 5, 6, 7 }), 1, 1));

    D->insert(make_shared<BC>(1, 0, uvec({ 1 }), "p"));
    D->insert(make_shared<BC>(2, 0, uvec({ 10 }), 1));
    D->insert(make_shared<BC>(3, 0, uvec({ 10 }), 3));
    D->insert(make_shared<CLoad>(1, 0, 20, uvec({ 5, 6 }), 2));

    D->get_node(5)->get_current_displacement().print();
}

#endif
