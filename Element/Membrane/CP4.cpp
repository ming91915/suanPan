#include "CP4.h"
#include <Toolbox/integrationPlan.h>
#include <Toolbox/shapeFunctionQuad.h>

const unsigned CP4::m_node = 4;
const unsigned CP4::m_dof = 2;

CP4::CP4(const unsigned& T, const uvec& N, const unsigned& M, const double& TH, const bool& R, const bool& F)
    : Element(T, ET_CP4, m_node, m_dof, N, uvec{ M }, F)
    , thickness(TH)
    , reduced_scheme(R) {}

void CP4::initialize(const shared_ptr<Domain>& D) {
    const auto& material_proto = D->get_material(static_cast<unsigned>(material_tag(0)));

    const unsigned order = reduced_scheme ? 1 : 2;
    const integrationPlan plan(2, order, 1);

    int_pt.clear();
    for(unsigned I = 0; I < plan.n_rows(); ++I) {
        int_pt.push_back(make_unique<IntegrationPoint>());
        int_pt[I]->coor.zeros(2);
        for(auto J = 0; J < 2; ++J) int_pt[I]->coor(J) = plan(I, J);
        int_pt[I]->weight = plan(I, 2);
        int_pt[I]->m_material = material_proto->get_copy();
    }

    mat ele_coor(m_node, m_dof);
    for(unsigned I = 0; I < m_node; ++I) {
        auto& tmp_coor = node_ptr[I].lock()->get_coordinate();
        for(unsigned J = 0; J < m_dof; ++J) ele_coor(I, J) = tmp_coor(J);
    }

    mass.zeros();
    for(const auto& I : int_pt) {
        const auto pn = shapeFunctionQuad(I->coor, 1);
        const mat jacob = pn * ele_coor;
        I->jacob_det = det(jacob);
        I->pn_pxy = solve(jacob, pn);

        const auto n_int = shapeFunctionQuad(I->coor, 0);

        auto tmp_density = I->m_material->get_parameter();
        if(tmp_density != 0.) {
            tmp_density *= I->jacob_det * I->weight * thickness;
            for(auto J = 0; J < 4; ++J)
                for(auto K = J; K < 4; ++K) mass(2 * J, 2 * K) += tmp_density * n_int(J) * n_int(K);
        }
    }

    if(mass(0, 0) != 0.)
        for(auto I = 0; I < 8; I += 2) {
            mass(I + 1, I + 1) = mass(I, I);
            for(auto J = I + 2; J < 8; J += 2) {
                mass(J, I) = mass(I, J);
                mass(I + 1, J + 1) = mass(I, J);
                mass(J + 1, I + 1) = mass(I, J);
            }
        }
}

int CP4::update_status() {
    auto code = 0;

    auto& TD0 = node_ptr.at(0).lock()->get_trial_displacement();
    auto& TD1 = node_ptr.at(1).lock()->get_trial_displacement();
    auto& TD2 = node_ptr.at(2).lock()->get_trial_displacement();
    auto& TD3 = node_ptr.at(3).lock()->get_trial_displacement();

    vec tmp_strain(3);

    stiffness.zeros();
    resistance.zeros();
    for(const auto& I : int_pt) {
        const auto& NX1 = I->pn_pxy(0, 0);
        const auto& NY1 = I->pn_pxy(1, 0);
        const auto& NX2 = I->pn_pxy(0, 1);
        const auto& NY2 = I->pn_pxy(1, 1);
        const auto& NX3 = I->pn_pxy(0, 2);
        const auto& NY3 = I->pn_pxy(1, 2);
        const auto& NX4 = I->pn_pxy(0, 3);
        const auto& NY4 = I->pn_pxy(1, 3);

        tmp_strain(0) = NX1 * TD0(0) + NX2 * TD1(0) + NX3 * TD2(0) + NX4 * TD3(0);
        tmp_strain(1) = NY1 * TD0(1) + NY2 * TD1(1) + NY3 * TD2(1) + NY4 * TD3(1);
        tmp_strain(2) = NX1 * TD0(1) + NX2 * TD1(1) + NX3 * TD2(1) + NX4 * TD3(1) + NY1 * TD0(0) + NY2 * TD1(0) + NY3 * TD2(0) + NY4 * TD3(0);

        code += I->m_material->update_trial_status(tmp_strain);

        // OPTIMIZED STIFFNESS MATRRIX FORMULATION B'*D*B
        // MATLAB SYMBOLIC COMPUTATION CODE:
        // syms NX1 NX2 NX3 NX4 NY1 NY2 NY3 NY4 D00 D01 D02 D10 D11 D12
        // D20 D21 D22 S0 S1
        // S2 real;
        // N=[NX1,0,NX2,0,NX3,0,NX4,0;0,NY1,0,NY2,0,NY3,0,NY4;NY1,NX1,NY2,NX2,NY3,NX3,NY4,NX4];
        // D=[D00,D01,D02;D01,D11,D12;D02,D12,D22];
        // S=[S0,S1,S2]';
        // K=N'*D*N;
        // R=N'*S;
        // FOR EACH INTEGRATION POINT DIRECTLY COMPUTING B'*D*B IS OF
        // COMPLEXITY OF
        // 8*3*3*8*3*8=13824
        // FOLLOWING CODES ONLY HAVE 117 MULTIPLICATIONS AND 95 ADDTIONS
        // AND COPY OF UPPER
        // TRIANGLE TO LOWER PART
        const auto tmp_factor = I->jacob_det * I->weight * thickness;

        auto& tmp_stiff = I->m_material->get_stiffness();
        auto& tmp_stress = I->m_material->get_stress();

        const auto D00 = tmp_factor * tmp_stiff(0, 0);
        const auto D01 = tmp_factor * tmp_stiff(0, 1);
        const auto D02 = tmp_factor * tmp_stiff(0, 2);
        const auto D11 = tmp_factor * tmp_stiff(1, 1);
        const auto D12 = tmp_factor * tmp_stiff(1, 2);
        const auto D22 = tmp_factor * tmp_stiff(2, 2);

        const auto S0 = tmp_factor * tmp_stress(0);
        const auto S1 = tmp_factor * tmp_stress(1);
        const auto S2 = tmp_factor * tmp_stress(2);

        const auto AA = D02 * NX1;
        const auto BB = D12 * NY1;
        const auto CC = D02 * NX2;
        const auto DD = D12 * NY2;
        const auto EE = D02 * NX3;
        const auto FF = D12 * NY3;
        const auto GG = D12 * NY4;
        const auto HH = D00 * NX1 + D02 * NY1;
        const auto II = D12 * NX1 + D11 * NY1;
        const auto JJ = D00 * NX2 + D02 * NY2;
        const auto KK = D12 * NX2 + D11 * NY2;
        const auto LL = D12 * NX3 + D11 * NY3;
        const auto MM = D00 * NX3 + D02 * NY3;
        const auto NN = D02 * NX4 + D22 * NY4;
        const auto OO = D22 * NY1 + AA;
        const auto PP = D01 * NY1 + AA;
        const auto QQ = D01 * NX1 + BB;
        const auto RR = D22 * NX1 + BB;
        const auto SS = D01 * NY2 + CC;
        const auto TT = D22 * NY2 + CC;
        const auto UU = D01 * NX2 + DD;
        const auto VV = D22 * NX2 + DD;
        const auto WW = D22 * NY3 + EE;
        const auto XX = D01 * NX3 + FF;
        const auto YY = D22 * NX3 + FF;

        stiffness(0, 0) += NX1 * HH + NY1 * OO;
        stiffness(0, 1) += NX1 * OO + NY1 * QQ;
        stiffness(0, 2) += NX2 * HH + NY2 * OO;
        stiffness(0, 3) += NX2 * OO + NY2 * QQ;
        stiffness(0, 4) += NX3 * HH + NY3 * OO;
        stiffness(0, 5) += NX3 * OO + NY3 * QQ;
        stiffness(0, 6) += NX4 * HH + NY4 * OO;
        stiffness(0, 7) += NX4 * OO + NY4 * QQ;
        stiffness(1, 1) += NX1 * RR + NY1 * II;
        stiffness(1, 2) += NX2 * PP + NY2 * RR;
        stiffness(1, 3) += NX2 * RR + NY2 * II;
        stiffness(1, 4) += NX3 * PP + NY3 * RR;
        stiffness(1, 5) += NX3 * RR + NY3 * II;
        stiffness(1, 6) += NX4 * PP + NY4 * RR;
        stiffness(1, 7) += NX4 * RR + NY4 * II;
        stiffness(2, 2) += NX2 * JJ + NY2 * TT;
        stiffness(2, 3) += NX2 * TT + NY2 * UU;
        stiffness(2, 4) += NX3 * JJ + NY3 * TT;
        stiffness(2, 5) += NX3 * TT + NY3 * UU;
        stiffness(2, 6) += NX4 * JJ + NY4 * TT;
        stiffness(2, 7) += NX4 * TT + NY4 * UU;
        stiffness(3, 3) += NX2 * VV + NY2 * KK;
        stiffness(3, 4) += NX3 * SS + NY3 * VV;
        stiffness(3, 5) += NX3 * VV + NY3 * KK;
        stiffness(3, 6) += NX4 * SS + NY4 * VV;
        stiffness(3, 7) += NX4 * VV + NY4 * KK;
        stiffness(4, 4) += NX3 * MM + NY3 * WW;
        stiffness(4, 5) += NX3 * WW + NY3 * XX;
        stiffness(4, 6) += NX4 * MM + NY4 * WW;
        stiffness(4, 7) += NX4 * WW + NY4 * XX;
        stiffness(5, 5) += NX3 * YY + NY3 * LL;
        stiffness(5, 6) += NX4 * (EE + D01 * NY3) + NY4 * YY;
        stiffness(5, 7) += NX4 * YY + NY4 * LL;
        stiffness(6, 6) += NX4 * (D00 * NX4 + D02 * NY4) + NY4 * NN;
        stiffness(6, 7) += NX4 * NN + NY4 * (D01 * NX4 + GG);
        stiffness(7, 7) += NX4 * (D22 * NX4 + GG) + NY4 * (D12 * NX4 + D11 * NY4);

        resistance(0) += NX1 * S0 + NY1 * S2;
        resistance(1) += NX1 * S2 + NY1 * S1;
        resistance(2) += NX2 * S0 + NY2 * S2;
        resistance(3) += NX2 * S2 + NY2 * S1;
        resistance(4) += NX3 * S0 + NY3 * S2;
        resistance(5) += NX3 * S2 + NY3 * S1;
        resistance(6) += NX4 * S0 + NY4 * S2;
        resistance(7) += NX4 * S2 + NY4 * S1;
    }

    for(auto I = 0; I < 8; ++I)
        for(auto J = I + 1; J < 8; ++J) stiffness(J, I) = stiffness(I, J);

    return code;
}

int CP4::commit_status() {
    auto code = 0;
    for(const auto& I : int_pt) code += I->m_material->commit_status();
    return code;
}

int CP4::clear_status() {
    auto code = 0;
    for(const auto& I : int_pt) code += I->m_material->clear_status();
    return code;
}

int CP4::reset_status() {
    auto code = 0;
    for(const auto& I : int_pt) code += I->m_material->reset_status();
    return code;
}

void CP4::print() { suanpan_info("This is a CP4 element.\n"); }
