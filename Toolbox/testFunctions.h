#ifndef TESTFUNCTIONS_H
#define TESTFUNCTIONS_H

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

void example_ODE()
{
    DP45 E(make_shared<ODE_INSTANCE>(), make_shared<RelError>());
    E.initialize();
    auto& D = getCurrentDisplacement(E.getWorkroom());
    D(0) = 1;
    E.analyze(1);
    E.getWorkroom()->getTrialDisplacement().print();
}

void example_Newmark()
{
    auto B = make_shared<Workroom>(3, SUANPAN_DYNAMICS);
    B->initialize();
    mat M = eye(3, 3);
    M(1, 1) = 3;
    B->updateMass(M);
    mat K = { { 2, -1, 0 }, { -1, 4, -2 }, { 0, -2, 2 } };
    B->updateStiffness(K);
    B->updateCurrentAcceleration({ 0, 0, 6 });
    B->updateTrialLoad({ 0, 0, 6 });
    auto W = make_shared<Domain>();
    W->setWorkroom(B);
    Newmark C(W);
    for(auto i = 0; i < 363; ++i) {
        B->updateIncreTime(0.01);
        C.updateStatus();
        B->commitStatus();
    }
    cout << B->getCurrentTime() << endl;
    B->getCurrentDisplacement().print();
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

    Newton S(D, make_shared<AbsResidual>(1E-4, true));
    S.initialize();

    auto INFO = S.analyze(1);
    if(INFO != 0) cout << INFO << endl;

    // D->disable_bc(3);
    // S.analyze(.5);

    cout << "\n";
    D->getNode(2)->getCurrentDisplacement().print();
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
    Newton S(D, make_shared<AbsIncreDisp>(nullptr, 1E-4));
    S.initialize();
    S.analyze(1);
    cout << "\n";
    D->getNode(2)->getCurrentDisplacement().print();
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

    Newton S(D, make_shared<AbsIncreDisp>(nullptr, 1E-4));
    S.initialize();
    S.analyze(1);
    cout << "\n";
    D->getNode(5)->getCurrentDisplacement().print();
}

void example_CP4()
{
    auto D = make_shared<Domain>();

    D->insert(make_shared<Elastic2D>(static_cast<unsigned>(1), 2E5));

    D->insert(make_shared<Node>(1, vec({ 2.72914219, 4.38675451 })));
    D->insert(make_shared<Node>(2, vec({ 3.45700812, 3.77801371 })));
    D->insert(make_shared<Node>(3, vec({ 4.31792021, 3.3682332 })));
    D->insert(make_shared<Node>(4, vec({ 6.15400314, 2.85372901 })));
    D->insert(make_shared<Node>(5, vec({ 7.09516191, 2.69512796 })));
    D->insert(make_shared<Node>(6, vec({ 8.99575901, 2.52251244 })));
    D->insert(make_shared<Node>(7, vec({ 9.94998169, 2.50005555 })));
    D->insert(make_shared<Node>(8, vec({ 11.8836107, 2.58012843 })));
    D->insert(make_shared<Node>(9, vec({ 12.8455591, 2.68692613 })));
    D->insert(make_shared<Node>(10, vec({ 13.8004313, 2.84472942 })));
    D->insert(make_shared<Node>(11, vec({ 15.663744, 3.36116552 })));
    D->insert(make_shared<Node>(12, vec({ 16.5376148, 3.77482033 })));
    D->insert(make_shared<Node>(13, vec({ 17.2751637, 4.39245558 })));
    D->insert(make_shared<Node>(14, vec({ 17.5, 5 })));
    D->insert(make_shared<Node>(15, vec({ 17.2751637, 5.60754442 })));
    D->insert(make_shared<Node>(16, vec({ 16.5376148, 6.22517967 })));
    D->insert(make_shared<Node>(17, vec({ 15.663744, 6.63883448 })));
    D->insert(make_shared<Node>(18, vec({ 13.8004313, 7.15527058 })));
    D->insert(make_shared<Node>(19, vec({ 12.8455591, 7.31307364 })));
    D->insert(make_shared<Node>(20, vec({ 11.8836107, 7.41987181 })));
    D->insert(make_shared<Node>(21, vec({ 9.94998169, 7.49994421 })));
    D->insert(make_shared<Node>(22, vec({ 8.99575901, 7.47748756 })));
    D->insert(make_shared<Node>(23, vec({ 7.09516191, 7.30487204 })));
    D->insert(make_shared<Node>(24, vec({ 6.15400314, 7.14627075 })));
    D->insert(make_shared<Node>(25, vec({ 4.31792021, 6.6317668 })));
    D->insert(make_shared<Node>(26, vec({ 3.45700812, 6.22198629 })));
    D->insert(make_shared<Node>(27, vec({ 2.72914219, 5.61324549 })));
    D->insert(make_shared<Node>(28, vec({ 2.5, 5 })));
    D->insert(make_shared<Node>(29, vec({ 20, 0 })));
    D->insert(make_shared<Node>(30, vec({ 0, 0 })));
    D->insert(make_shared<Node>(31, vec({ 20, 10 })));
    D->insert(make_shared<Node>(32, vec({ 0, 10 })));
    D->insert(make_shared<Node>(33, vec({ 5.22498178, 3.07215714 })));
    D->insert(make_shared<Node>(34, vec({ 8.04343128, 2.58656907 })));
    D->insert(make_shared<Node>(35, vec({ 10.9176826, 2.51878476 })));
    D->insert(make_shared<Node>(36, vec({ 14.7431421, 3.06343365 })));
    D->insert(make_shared<Node>(37, vec({ 14.7431421, 6.93656635 })));
    D->insert(make_shared<Node>(38, vec({ 10.9176826, 7.481215 })));
    D->insert(make_shared<Node>(39, vec({ 8.04343128, 7.41343069 })));
    D->insert(make_shared<Node>(40, vec({ 5.22498178, 6.92784262 })));
    D->insert(make_shared<Node>(41, vec({ 19, 0 })));
    D->insert(make_shared<Node>(42, vec({ 18, 0 })));
    D->insert(make_shared<Node>(43, vec({ 17, 0 })));
    D->insert(make_shared<Node>(44, vec({ 16, 0 })));
    D->insert(make_shared<Node>(45, vec({ 15, 0 })));
    D->insert(make_shared<Node>(46, vec({ 14, 0 })));
    D->insert(make_shared<Node>(47, vec({ 13, 0 })));
    D->insert(make_shared<Node>(48, vec({ 12, 0 })));
    D->insert(make_shared<Node>(49, vec({ 11, 0 })));
    D->insert(make_shared<Node>(50, vec({ 10, 0 })));
    D->insert(make_shared<Node>(51, vec({ 9, 0 })));
    D->insert(make_shared<Node>(52, vec({ 8, 0 })));
    D->insert(make_shared<Node>(53, vec({ 7, 0 })));
    D->insert(make_shared<Node>(54, vec({ 6, 0 })));
    D->insert(make_shared<Node>(55, vec({ 5, 0 })));
    D->insert(make_shared<Node>(56, vec({ 4, 0 })));
    D->insert(make_shared<Node>(57, vec({ 3, 0 })));
    D->insert(make_shared<Node>(58, vec({ 2, 0 })));
    D->insert(make_shared<Node>(59, vec({ 1, 0 })));
    D->insert(make_shared<Node>(60, vec({ 20, 9 })));
    D->insert(make_shared<Node>(61, vec({ 20, 8 })));
    D->insert(make_shared<Node>(62, vec({ 20, 7 })));
    D->insert(make_shared<Node>(63, vec({ 20, 6 })));
    D->insert(make_shared<Node>(64, vec({ 20, 5 })));
    D->insert(make_shared<Node>(65, vec({ 20, 4 })));
    D->insert(make_shared<Node>(66, vec({ 20, 3 })));
    D->insert(make_shared<Node>(67, vec({ 20, 2 })));
    D->insert(make_shared<Node>(68, vec({ 20, 1 })));
    D->insert(make_shared<Node>(69, vec({ 1, 10 })));
    D->insert(make_shared<Node>(70, vec({ 2, 10 })));
    D->insert(make_shared<Node>(71, vec({ 3, 10 })));
    D->insert(make_shared<Node>(72, vec({ 4, 10 })));
    D->insert(make_shared<Node>(73, vec({ 5, 10 })));
    D->insert(make_shared<Node>(74, vec({ 6, 10 })));
    D->insert(make_shared<Node>(75, vec({ 7, 10 })));
    D->insert(make_shared<Node>(76, vec({ 8, 10 })));
    D->insert(make_shared<Node>(77, vec({ 9, 10 })));
    D->insert(make_shared<Node>(78, vec({ 10, 10 })));
    D->insert(make_shared<Node>(79, vec({ 11, 10 })));
    D->insert(make_shared<Node>(80, vec({ 12, 10 })));
    D->insert(make_shared<Node>(81, vec({ 13, 10 })));
    D->insert(make_shared<Node>(82, vec({ 14, 10 })));
    D->insert(make_shared<Node>(83, vec({ 15, 10 })));
    D->insert(make_shared<Node>(84, vec({ 16, 10 })));
    D->insert(make_shared<Node>(85, vec({ 17, 10 })));
    D->insert(make_shared<Node>(86, vec({ 18, 10 })));
    D->insert(make_shared<Node>(87, vec({ 19, 10 })));
    D->insert(make_shared<Node>(88, vec({ 0, 1 })));
    D->insert(make_shared<Node>(89, vec({ 0, 2 })));
    D->insert(make_shared<Node>(90, vec({ 0, 3 })));
    D->insert(make_shared<Node>(91, vec({ 0, 4 })));
    D->insert(make_shared<Node>(92, vec({ 0, 5 })));
    D->insert(make_shared<Node>(93, vec({ 0, 6 })));
    D->insert(make_shared<Node>(94, vec({ 0, 7 })));
    D->insert(make_shared<Node>(95, vec({ 0, 8 })));
    D->insert(make_shared<Node>(96, vec({ 0, 9 })));
    D->insert(make_shared<Node>(97, vec({ 0.905789375, 6.95894146 })));
    D->insert(make_shared<Node>(98, vec({ 3.46965075, 7.29488039 })));
    D->insert(make_shared<Node>(99, vec({ 4.49119663, 7.49757385 })));
    D->insert(make_shared<Node>(100, vec({ 6.05716705, 8.00672913 })));
    D->insert(make_shared<Node>(101, vec({ 7.02774763, 8.15115929 })));
    D->insert(make_shared<Node>(102, vec({ 8.98692513, 8.29409981 })));
    D->insert(make_shared<Node>(103, vec({ 9.9701519, 8.31453609 })));
    D->insert(make_shared<Node>(104, vec({ 10.9672155, 8.30825615 })));
    D->insert(make_shared<Node>(105, vec({ 13.1626215, 8.23344135 })));
    D->insert(make_shared<Node>(106, vec({ 14.1279993, 7.81007099 })));
    D->insert(make_shared<Node>(107, vec({ 15.0116558, 7.52844667 })));
    D->insert(make_shared<Node>(108, vec({ 17.3222466, 6.86478138 })));
    D->insert(make_shared<Node>(109, vec({ 19.2091503, 5.77586746 })));
    D->insert(make_shared<Node>(110, vec({ 18.3604908, 4.82103205 })));
    D->insert(make_shared<Node>(111, vec({ 18.9746513, 3.00121284 })));
    D->insert(make_shared<Node>(112, vec({ 16.3487091, 2.66877604 })));
    D->insert(make_shared<Node>(113, vec({ 15.7442207, 2.74105406 })));
    D->insert(make_shared<Node>(114, vec({ 13.964386, 1.92259634 })));
    D->insert(make_shared<Node>(115, vec({ 12.943243, 1.8163197 })));
    D->insert(make_shared<Node>(116, vec({ 11.9476366, 1.74521804 })));
    D->insert(make_shared<Node>(117, vec({ 8.98355675, 1.70602751 })));
    D->insert(make_shared<Node>(118, vec({ 7.99123192, 1.74261677 })));
    D->insert(make_shared<Node>(119, vec({ 5.78785276, 1.88504279 })));
    D->insert(make_shared<Node>(120, vec({ 4.79941797, 2.37401557 })));
    D->insert(make_shared<Node>(121, vec({ 3.14516735, 3.34468198 })));
    D->insert(make_shared<Node>(122, vec({ 2.00935268, 1.02064848 })));
    D->insert(make_shared<Node>(123, vec({ 0.796394646, 4.07696199 })));
    D->insert(make_shared<Node>(124, vec({ 1.88147092, 5.21837997 })));
    D->insert(make_shared<Node>(125, vec({ 9.96777534, 1.69049704 })));
    D->insert(make_shared<Node>(126, vec({ 3.90780711, 2.78453946 })));
    D->insert(make_shared<Node>(127, vec({ 6.96569729, 1.80076873 })));
    D->insert(make_shared<Node>(128, vec({ 10.9563141, 1.70306897 })));
    D->insert(make_shared<Node>(129, vec({ 15.0968275, 2.0825994 })));
    D->insert(make_shared<Node>(130, vec({ 15.8577843, 7.2250824 })));
    D->insert(make_shared<Node>(131, vec({ 11.9960947, 8.28171444 })));
    D->insert(make_shared<Node>(132, vec({ 8.00659657, 8.24302769 })));
    D->insert(make_shared<Node>(133, vec({ 5.16086864, 7.78778744 })));
    D->insert(make_shared<Node>(134, vec({ 18.9980774, 1.01758873 })));
    D->insert(make_shared<Node>(135, vec({ 18.0038948, 0.98239702 })));
    D->insert(make_shared<Node>(136, vec({ 17.0212708, 0.924255311 })));
    D->insert(make_shared<Node>(137, vec({ 16.0247078, 0.934111238 })));
    D->insert(make_shared<Node>(138, vec({ 15.0279636, 0.994493902 })));
    D->insert(make_shared<Node>(139, vec({ 14.0077772, 0.969480634 })));
    D->insert(make_shared<Node>(140, vec({ 12.9911575, 0.92061156 })));
    D->insert(make_shared<Node>(141, vec({ 11.9849424, 0.885057449 })));
    D->insert(make_shared<Node>(142, vec({ 10.9834728, 0.86283505 })));
    D->insert(make_shared<Node>(143, vec({ 9.98371506, 0.855803609 })));
    D->insert(make_shared<Node>(144, vec({ 8.98540306, 0.863417983 })));
    D->insert(make_shared<Node>(145, vec({ 7.98308372, 0.882420361 })));
    D->insert(make_shared<Node>(146, vec({ 6.97070217, 0.903956175 })));
    D->insert(make_shared<Node>(147, vec({ 5.96605206, 0.8801682 })));
    D->insert(make_shared<Node>(148, vec({ 5.02722645, 0.780165315 })));
    D->insert(make_shared<Node>(149, vec({ 4.06440306, 0.865388095 })));
    D->insert(make_shared<Node>(150, vec({ 3.04873967, 0.996961594 })));
    D->insert(make_shared<Node>(151, vec({ 18.9912052, 7.92200422 })));
    D->insert(make_shared<Node>(152, vec({ 18.9186077, 6.75034285 })));
    D->insert(make_shared<Node>(153, vec({ 17.8792305, 6.08912706 })));
    D->insert(make_shared<Node>(154, vec({ 19.1846809, 4.85861349 })));
    D->insert(make_shared<Node>(155, vec({ 19.0867691, 3.94382739 })));
    D->insert(make_shared<Node>(156, vec({ 17.7438412, 3.06235099 })));
    D->insert(make_shared<Node>(157, vec({ 2.01231289, 9.19629669 })));
    D->insert(make_shared<Node>(158, vec({ 2.90785336, 9.29030704 })));
    D->insert(make_shared<Node>(159, vec({ 3.81359935, 9.14692974 })));
    D->insert(make_shared<Node>(160, vec({ 4.80953312, 8.79588509 })));
    D->insert(make_shared<Node>(161, vec({ 5.95392847, 8.95729446 })));
    D->insert(make_shared<Node>(162, vec({ 6.9824872, 9.04945946 })));
    D->insert(make_shared<Node>(163, vec({ 7.98720169, 9.10609245 })));
    D->insert(make_shared<Node>(164, vec({ 8.986022, 9.13694191 })));
    D->insert(make_shared<Node>(165, vec({ 9.98459625, 9.14822292 })));
    D->insert(make_shared<Node>(166, vec({ 10.9887295, 9.14535904 })));
    D->insert(make_shared<Node>(167, vec({ 12.0020962, 9.14016819 })));
    D->insert(make_shared<Node>(168, vec({ 13.0038252, 9.17920589 })));
    D->insert(make_shared<Node>(169, vec({ 13.9234333, 9.2929039 })));
    D->insert(make_shared<Node>(170, vec({ 14.8521681, 9.23360348 })));
    D->insert(make_shared<Node>(171, vec({ 15.7958097, 9.07630539 })));
    D->insert(make_shared<Node>(172, vec({ 16.8592625, 8.78177643 })));
    D->insert(make_shared<Node>(173, vec({ 17.9861469, 8.92427254 })));
    D->insert(make_shared<Node>(174, vec({ 0.929384053, 3.04808116 })));
    D->insert(make_shared<Node>(175, vec({ 2.39908123, 3.95593715 })));
    D->insert(make_shared<Node>(176, vec({ 0.855608225, 5.11023951 })));
    D->insert(make_shared<Node>(177, vec({ 0.744426489, 6.06128693 })));
    D->insert(make_shared<Node>(178, vec({ 2.27765369, 6.16860247 })));
    D->insert(make_shared<Node>(179, vec({ 1.61493957, 5.65488243 })));
    D->insert(make_shared<Node>(180, vec({ 18.9774742, 2.01195407 })));
    D->insert(make_shared<Node>(181, vec({ 17.9630547, 1.94630134 })));
    D->insert(make_shared<Node>(182, vec({ 17.041851, 1.78374124 })));
    D->insert(make_shared<Node>(183, vec({ 4.27830029, 1.68435526 })));
    D->insert(make_shared<Node>(184, vec({ 3.17428946, 2.05556226 })));
    D->insert(make_shared<Node>(185, vec({ 18.988348, 8.96646976 })));
    D->insert(make_shared<Node>(186, vec({ 18.040329, 6.9163537 })));
    D->insert(make_shared<Node>(187, vec({ 18.2189426, 5.84491825 })));
    D->insert(make_shared<Node>(188, vec({ 1.24509966, 7.95531988 })));
    D->insert(make_shared<Node>(189, vec({ 2.8498354, 8.72278214 })));
    D->insert(make_shared<Node>(190, vec({ 3.38308358, 8.39914703 })));
    D->insert(make_shared<Node>(191, vec({ 1.87784696, 3.20200205 })));
    D->insert(make_shared<Node>(192, vec({ 18.0740376, 7.90526295 })));
    D->insert(make_shared<Node>(193, vec({ 18.1284237, 4.03021479 })));
    D->insert(make_shared<Node>(194, vec({ 1.98389804, 2.03907585 })));
    D->insert(make_shared<Node>(195, vec({ 0.988115489, 2.01605821 })));
    D->insert(make_shared<Node>(196, vec({ 2.0276885, 4.65614223 })));
    D->insert(make_shared<Node>(197, vec({ 16.1204815, 1.81770265 })));
    D->insert(make_shared<Node>(198, vec({ 2.76133513, 2.89158201 })));
    D->insert(make_shared<Node>(199, vec({ 16.6994743, 7.56638002 })));
    D->insert(make_shared<Node>(200, vec({ 4.0096693, 7.97319031 })));
    D->insert(make_shared<Node>(201, vec({ 17.2687492, 7.79265833 })));
    D->insert(make_shared<Node>(202, vec({ 2.78393555, 7.83020544 })));
    D->insert(make_shared<Node>(203, vec({ 2.15439582, 7.28728819 })));
    D->insert(make_shared<Node>(204, vec({ 15.3946495, 8.24264622 })));
    D->insert(make_shared<Node>(205, vec({ 5.02577114, 1.41451979 })));
    D->insert(make_shared<Node>(206, vec({ 1.00050795, 1.00847018 })));
    D->insert(make_shared<Node>(207, vec({ 1.04875529, 9.0366497 })));
    D->insert(make_shared<Node>(208, vec({ 16.640049, 6.94418478 })));
    D->insert(make_shared<Node>(209, vec({ 2.87511015, 6.74684477 })));
    D->insert(make_shared<Node>(210, vec({ 18.574482, 5.51834917 })));
    D->insert(make_shared<Node>(211, vec({ 1.67823446, 6.65729523 })));
    D->insert(make_shared<Node>(212, vec({ 1.34159851, 6.10260534 })));
    D->insert(make_shared<Node>(213, vec({ 1.36583495, 4.85384989 })));
    D->insert(make_shared<Node>(214, vec({ 1.46808827, 4.20651293 })));
    D->insert(make_shared<Node>(215, vec({ 17.0200481, 2.49889779 })));
    D->insert(make_shared<Node>(216, vec({ 2.21254873, 8.43283176 })));
    D->insert(make_shared<Node>(217, vec({ 16.1810532, 7.94949865 })));
    D->insert(make_shared<Node>(218, vec({ 14.5619202, 8.47592258 })));
    D->insert(make_shared<Node>(219, vec({ 13.9022322, 8.70239544 })));

    D->insert(make_shared<CP4>(1, uvec({ 94, 93, 177, 97 }), 1, .01));
    D->insert(make_shared<CP4>(2, uvec({ 190, 202, 98, 200 }), 1, .01));
    D->insert(make_shared<CP4>(3, uvec({ 70, 157, 158, 71 }), 1, .01));
    D->insert(make_shared<CP4>(4, uvec({ 98, 25, 99, 200 }), 1, .01));
    D->insert(make_shared<CP4>(5, uvec({ 73, 160, 161, 74 }), 1, .01));
    D->insert(make_shared<CP4>(6, uvec({ 23, 101, 100, 24 }), 1, .01));
    D->insert(make_shared<CP4>(7, uvec({ 132, 102, 164, 163 }), 1, .01));
    D->insert(make_shared<CP4>(8, uvec({ 21, 103, 102, 22 }), 1, .01));
    D->insert(make_shared<CP4>(9, uvec({ 38, 104, 103, 21 }), 1, .01));
    D->insert(make_shared<CP4>(10, uvec({ 81, 168, 169, 82 }), 1, .01));
    D->insert(make_shared<CP4>(11, uvec({ 18, 106, 105, 19 }), 1, .01));
    D->insert(make_shared<CP4>(12, uvec({ 37, 107, 106, 18 }), 1, .01));
    D->insert(make_shared<CP4>(13, uvec({ 85, 172, 173, 86 }), 1, .01));
    D->insert(make_shared<CP4>(14, uvec({ 108, 15, 153, 186 }), 1, .01));
    D->insert(make_shared<CP4>(15, uvec({ 14, 187, 153, 15 }), 1, .01));
    D->insert(make_shared<CP4>(16, uvec({ 66, 65, 155, 111 }), 1, .01));
    D->insert(make_shared<CP4>(17, uvec({ 134, 135, 42, 41 }), 1, .01));
    D->insert(make_shared<CP4>(18, uvec({ 113, 11, 36, 129 }), 1, .01));
    D->insert(make_shared<CP4>(19, uvec({ 137, 138, 45, 44 }), 1, .01));
    D->insert(make_shared<CP4>(20, uvec({ 115, 114, 10, 9 }), 1, .01));
    D->insert(make_shared<CP4>(21, uvec({ 116, 115, 9, 8 }), 1, .01));
    D->insert(make_shared<CP4>(22, uvec({ 117, 118, 145, 144 }), 1, .01));
    D->insert(make_shared<CP4>(23, uvec({ 34, 118, 117, 6 }), 1, .01));
    D->insert(make_shared<CP4>(24, uvec({ 146, 147, 54, 53 }), 1, .01));
    D->insert(make_shared<CP4>(25, uvec({ 33, 120, 119, 4 }), 1, .01));
    D->insert(make_shared<CP4>(26, uvec({ 175, 121, 2, 1 }), 1, .01));
    D->insert(make_shared<CP4>(27, uvec({ 59, 58, 122, 206 }), 1, .01));
    D->insert(make_shared<CP4>(28, uvec({ 91, 90, 174, 123 }), 1, .01));
    D->insert(make_shared<CP4>(29, uvec({ 178, 209, 203, 211 }), 1, .01));
    D->insert(make_shared<CP4>(30, uvec({ 6, 117, 125, 7 }), 1, .01));
    D->insert(make_shared<CP4>(31, uvec({ 2, 121, 126, 3 }), 1, .01));
    D->insert(make_shared<CP4>(32, uvec({ 4, 119, 127, 5 }), 1, .01));
    D->insert(make_shared<CP4>(33, uvec({ 7, 125, 128, 35 }), 1, .01));
    D->insert(make_shared<CP4>(34, uvec({ 36, 10, 114, 129 }), 1, .01));
    D->insert(make_shared<CP4>(35, uvec({ 208, 130, 17, 16 }), 1, .01));
    D->insert(make_shared<CP4>(36, uvec({ 19, 105, 131, 20 }), 1, .01));
    D->insert(make_shared<CP4>(37, uvec({ 22, 102, 132, 39 }), 1, .01));
    D->insert(make_shared<CP4>(38, uvec({ 133, 40, 24, 100 }), 1, .01));
    D->insert(make_shared<CP4>(39, uvec({ 41, 29, 68, 134 }), 1, .01));
    D->insert(make_shared<CP4>(40, uvec({ 68, 67, 180, 134 }), 1, .01));
    D->insert(make_shared<CP4>(41, uvec({ 180, 181, 135, 134 }), 1, .01));
    D->insert(make_shared<CP4>(42, uvec({ 135, 136, 43, 42 }), 1, .01));
    D->insert(make_shared<CP4>(43, uvec({ 135, 181, 182, 136 }), 1, .01));
    D->insert(make_shared<CP4>(44, uvec({ 114, 139, 138, 129 }), 1, .01));
    D->insert(make_shared<CP4>(45, uvec({ 138, 139, 46, 45 }), 1, .01));
    D->insert(make_shared<CP4>(46, uvec({ 115, 140, 139, 114 }), 1, .01));
    D->insert(make_shared<CP4>(47, uvec({ 140, 115, 116, 141 }), 1, .01));
    D->insert(make_shared<CP4>(48, uvec({ 142, 141, 116, 128 }), 1, .01));
    D->insert(make_shared<CP4>(49, uvec({ 144, 143, 125, 117 }), 1, .01));
    D->insert(make_shared<CP4>(50, uvec({ 146, 145, 118, 127 }), 1, .01));
    D->insert(make_shared<CP4>(51, uvec({ 147, 146, 127, 119 }), 1, .01));
    D->insert(make_shared<CP4>(52, uvec({ 147, 119, 205, 148 }), 1, .01));
    D->insert(make_shared<CP4>(53, uvec({ 205, 183, 149, 148 }), 1, .01));
    D->insert(make_shared<CP4>(54, uvec({ 191, 174, 195, 194 }), 1, .01));
    D->insert(make_shared<CP4>(55, uvec({ 149, 183, 184, 150 }), 1, .01));
    D->insert(make_shared<CP4>(56, uvec({ 87, 185, 60, 31 }), 1, .01));
    D->insert(make_shared<CP4>(57, uvec({ 16, 15, 108, 208 }), 1, .01));
    D->insert(make_shared<CP4>(58, uvec({ 187, 152, 186, 153 }), 1, .01));
    D->insert(make_shared<CP4>(59, uvec({ 109, 154, 64, 63 }), 1, .01));
    D->insert(make_shared<CP4>(60, uvec({ 110, 193, 155, 154 }), 1, .01));
    D->insert(make_shared<CP4>(61, uvec({ 13, 12, 156, 193 }), 1, .01));
    D->insert(make_shared<CP4>(62, uvec({ 32, 96, 207, 69 }), 1, .01));
    D->insert(make_shared<CP4>(63, uvec({ 95, 188, 207, 96 }), 1, .01));
    D->insert(make_shared<CP4>(64, uvec({ 188, 216, 157, 207 }), 1, .01));
    D->insert(make_shared<CP4>(65, uvec({ 71, 158, 159, 72 }), 1, .01));
    D->insert(make_shared<CP4>(66, uvec({ 158, 189, 190, 159 }), 1, .01));
    D->insert(make_shared<CP4>(67, uvec({ 162, 161, 100, 101 }), 1, .01));
    D->insert(make_shared<CP4>(68, uvec({ 74, 161, 162, 75 }), 1, .01));
    D->insert(make_shared<CP4>(69, uvec({ 163, 162, 101, 132 }), 1, .01));
    D->insert(make_shared<CP4>(70, uvec({ 165, 164, 102, 103 }), 1, .01));
    D->insert(make_shared<CP4>(71, uvec({ 166, 165, 103, 104 }), 1, .01));
    D->insert(make_shared<CP4>(72, uvec({ 167, 166, 104, 131 }), 1, .01));
    D->insert(make_shared<CP4>(73, uvec({ 168, 167, 131, 105 }), 1, .01));
    D->insert(make_shared<CP4>(74, uvec({ 169, 219, 218, 170 }), 1, .01));
    D->insert(make_shared<CP4>(75, uvec({ 170, 218, 204, 171 }), 1, .01));
    D->insert(make_shared<CP4>(76, uvec({ 130, 208, 199, 217 }), 1, .01));
    D->insert(make_shared<CP4>(77, uvec({ 60, 185, 151, 61 }), 1, .01));
    D->insert(make_shared<CP4>(78, uvec({ 30, 59, 206, 88 }), 1, .01));
    D->insert(make_shared<CP4>(79, uvec({ 195, 206, 122, 194 }), 1, .01));
    D->insert(make_shared<CP4>(80, uvec({ 195, 174, 90, 89 }), 1, .01));
    D->insert(make_shared<CP4>(81, uvec({ 123, 176, 92, 91 }), 1, .01));
    D->insert(make_shared<CP4>(82, uvec({ 214, 213, 176, 123 }), 1, .01));
    D->insert(make_shared<CP4>(83, uvec({ 26, 209, 178, 27 }), 1, .01));
    D->insert(make_shared<CP4>(84, uvec({ 97, 211, 203, 188 }), 1, .01));
    D->insert(make_shared<CP4>(85, uvec({ 95, 94, 97, 188 }), 1, .01));
    D->insert(make_shared<CP4>(86, uvec({ 25, 98, 209, 26 }), 1, .01));
    D->insert(make_shared<CP4>(87, uvec({ 99, 25, 40, 133 }), 1, .01));
    D->insert(make_shared<CP4>(88, uvec({ 72, 159, 160, 73 }), 1, .01));
    D->insert(make_shared<CP4>(89, uvec({ 101, 23, 39, 132 }), 1, .01));
    D->insert(make_shared<CP4>(90, uvec({ 104, 38, 20, 131 }), 1, .01));
    D->insert(make_shared<CP4>(91, uvec({ 107, 37, 17, 130 }), 1, .01));
    D->insert(make_shared<CP4>(92, uvec({ 82, 169, 170, 83 }), 1, .01));
    D->insert(make_shared<CP4>(93, uvec({ 83, 170, 171, 84 }), 1, .01));
    D->insert(make_shared<CP4>(94, uvec({ 151, 152, 62, 61 }), 1, .01));
    D->insert(make_shared<CP4>(95, uvec({ 192, 186, 152, 151 }), 1, .01));
    D->insert(make_shared<CP4>(96, uvec({ 110, 14, 13, 193 }), 1, .01));
    D->insert(make_shared<CP4>(97, uvec({ 156, 12, 112, 215 }), 1, .01));
    D->insert(make_shared<CP4>(98, uvec({ 67, 66, 111, 180 }), 1, .01));
    D->insert(make_shared<CP4>(99, uvec({ 112, 12, 11, 113 }), 1, .01));
    D->insert(make_shared<CP4>(100, uvec({ 136, 137, 44, 43 }), 1, .01));
    D->insert(make_shared<CP4>(101, uvec({ 116, 8, 35, 128 }), 1, .01));
    D->insert(make_shared<CP4>(102, uvec({ 118, 34, 5, 127 }), 1, .01));
    D->insert(make_shared<CP4>(103, uvec({ 120, 33, 3, 126 }), 1, .01));
    D->insert(make_shared<CP4>(104, uvec({ 147, 148, 55, 54 }), 1, .01));
    D->insert(make_shared<CP4>(105, uvec({ 150, 184, 194, 122 }), 1, .01));
    D->insert(make_shared<CP4>(106, uvec({ 206, 195, 89, 88 }), 1, .01));
    D->insert(make_shared<CP4>(107, uvec({ 149, 150, 57, 56 }), 1, .01));
    D->insert(make_shared<CP4>(108, uvec({ 28, 196, 175, 1 }), 1, .01));
    D->insert(make_shared<CP4>(109, uvec({ 28, 27, 124, 196 }), 1, .01));
    D->insert(make_shared<CP4>(110, uvec({ 143, 142, 128, 125 }), 1, .01));
    D->insert(make_shared<CP4>(111, uvec({ 148, 149, 56, 55 }), 1, .01));
    D->insert(make_shared<CP4>(112, uvec({ 145, 146, 53, 52 }), 1, .01));
    D->insert(make_shared<CP4>(113, uvec({ 171, 172, 85, 84 }), 1, .01));
    D->insert(make_shared<CP4>(114, uvec({ 219, 105, 106, 218 }), 1, .01));
    D->insert(make_shared<CP4>(115, uvec({ 197, 137, 136, 182 }), 1, .01));
    D->insert(make_shared<CP4>(116, uvec({ 138, 137, 197, 129 }), 1, .01));
    D->insert(make_shared<CP4>(117, uvec({ 47, 46, 139, 140 }), 1, .01));
    D->insert(make_shared<CP4>(118, uvec({ 48, 47, 140, 141 }), 1, .01));
    D->insert(make_shared<CP4>(119, uvec({ 49, 48, 141, 142 }), 1, .01));
    D->insert(make_shared<CP4>(120, uvec({ 50, 49, 142, 143 }), 1, .01));
    D->insert(make_shared<CP4>(121, uvec({ 51, 50, 143, 144 }), 1, .01));
    D->insert(make_shared<CP4>(122, uvec({ 52, 51, 144, 145 }), 1, .01));
    D->insert(make_shared<CP4>(123, uvec({ 120, 183, 205, 119 }), 1, .01));
    D->insert(make_shared<CP4>(124, uvec({ 191, 198, 121, 175 }), 1, .01));
    D->insert(make_shared<CP4>(125, uvec({ 150, 122, 58, 57 }), 1, .01));
    D->insert(make_shared<CP4>(126, uvec({ 199, 201, 172, 217 }), 1, .01));
    D->insert(make_shared<CP4>(127, uvec({ 152, 109, 63, 62 }), 1, .01));
    D->insert(make_shared<CP4>(128, uvec({ 187, 14, 110, 210 }), 1, .01));
    D->insert(make_shared<CP4>(129, uvec({ 65, 64, 154, 155 }), 1, .01));
    D->insert(make_shared<CP4>(130, uvec({ 157, 70, 69, 207 }), 1, .01));
    D->insert(make_shared<CP4>(131, uvec({ 200, 160, 159, 190 }), 1, .01));
    D->insert(make_shared<CP4>(132, uvec({ 160, 200, 99, 133 }), 1, .01));
    D->insert(make_shared<CP4>(133, uvec({ 163, 76, 75, 162 }), 1, .01));
    D->insert(make_shared<CP4>(134, uvec({ 164, 77, 76, 163 }), 1, .01));
    D->insert(make_shared<CP4>(135, uvec({ 165, 78, 77, 164 }), 1, .01));
    D->insert(make_shared<CP4>(136, uvec({ 166, 79, 78, 165 }), 1, .01));
    D->insert(make_shared<CP4>(137, uvec({ 167, 80, 79, 166 }), 1, .01));
    D->insert(make_shared<CP4>(138, uvec({ 168, 81, 80, 167 }), 1, .01));
    D->insert(make_shared<CP4>(139, uvec({ 218, 106, 107, 204 }), 1, .01));
    D->insert(make_shared<CP4>(140, uvec({ 172, 201, 192, 173 }), 1, .01));
    D->insert(make_shared<CP4>(141, uvec({ 185, 173, 192, 151 }), 1, .01));
    D->insert(make_shared<CP4>(142, uvec({ 93, 92, 176, 177 }), 1, .01));
    D->insert(make_shared<CP4>(143, uvec({ 196, 124, 213, 214 }), 1, .01));
    D->insert(make_shared<CP4>(144, uvec({ 124, 27, 178, 179 }), 1, .01));
    D->insert(make_shared<CP4>(145, uvec({ 190, 189, 216, 202 }), 1, .01));
    D->insert(make_shared<CP4>(146, uvec({ 210, 110, 154, 109 }), 1, .01));
    D->insert(make_shared<CP4>(147, uvec({ 177, 176, 179, 212 }), 1, .01));
    D->insert(make_shared<CP4>(148, uvec({ 111, 156, 181, 180 }), 1, .01));
    D->insert(make_shared<CP4>(149, uvec({ 129, 197, 112, 113 }), 1, .01));
    D->insert(make_shared<CP4>(150, uvec({ 183, 120, 126, 184 }), 1, .01));
    D->insert(make_shared<CP4>(151, uvec({ 121, 198, 184, 126 }), 1, .01));
    D->insert(make_shared<CP4>(152, uvec({ 185, 87, 86, 173 }), 1, .01));
    D->insert(make_shared<CP4>(153, uvec({ 109, 152, 187, 210 }), 1, .01));
    D->insert(make_shared<CP4>(154, uvec({ 189, 158, 157, 216 }), 1, .01));
    D->insert(make_shared<CP4>(155, uvec({ 107, 130, 217, 204 }), 1, .01));
    D->insert(make_shared<CP4>(156, uvec({ 175, 196, 214, 191 }), 1, .01));
    D->insert(make_shared<CP4>(157, uvec({ 194, 184, 198, 191 }), 1, .01));
    D->insert(make_shared<CP4>(158, uvec({ 179, 176, 213, 124 }), 1, .01));
    D->insert(make_shared<CP4>(159, uvec({ 112, 197, 182, 215 }), 1, .01));
    D->insert(make_shared<CP4>(160, uvec({ 201, 199, 208, 108 }), 1, .01));
    D->insert(make_shared<CP4>(161, uvec({ 201, 108, 186, 192 }), 1, .01));
    D->insert(make_shared<CP4>(162, uvec({ 203, 202, 216, 188 }), 1, .01));
    D->insert(make_shared<CP4>(163, uvec({ 202, 203, 209, 98 }), 1, .01));
    D->insert(make_shared<CP4>(164, uvec({ 111, 155, 193, 156 }), 1, .01));
    D->insert(make_shared<CP4>(165, uvec({ 133, 100, 161, 160 }), 1, .01));
    D->insert(make_shared<CP4>(166, uvec({ 212, 211, 97, 177 }), 1, .01));
    D->insert(make_shared<CP4>(167, uvec({ 179, 178, 211, 212 }), 1, .01));
    D->insert(make_shared<CP4>(168, uvec({ 214, 123, 174, 191 }), 1, .01));
    D->insert(make_shared<CP4>(169, uvec({ 181, 156, 215, 182 }), 1, .01));
    D->insert(make_shared<CP4>(170, uvec({ 217, 172, 171, 204 }), 1, .01));
    D->insert(make_shared<CP4>(171, uvec({ 219, 169, 168, 105 }), 1, .01));

    D->insert(make_shared<BC>(1, 0, uvec({ 31, 32, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78,
                                        79, 80, 81, 82, 83, 84, 85, 86, 87 }),
        "p"));
    D->insert(
        make_shared<CLoad>(1, 0, -1, uvec({ 29, 30, 41, 42, 43, 44, 45, 46, 47, 48, 49,
                                         50, 51, 52, 53, 54, 55, 56, 57, 58, 59 }),
            2));

    Newton S(D, make_shared<AbsResidual>(1E-4));
    S.setStepSize(.01, 0, 1);
    S.enable_symm();
    S.enable_band();
    S.initialize();
    S.analyze(1);

    cout << "\n";
    D->getNode(7)->getCurrentDisplacement().print();
}

#endif
