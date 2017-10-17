#include "B21H.h"
#include <Domain/DomainBase.h>
#include <Domain/Node.h>
#include <Section/Section.h>
#include <Toolbox/IntegrationPlan.h>
#include <Toolbox/shapeFunction.hpp>
#include <Toolbox/tensorToolbox.h>

const unsigned B21H::b_node = 2;
const unsigned B21H::b_dof = 3;

B21H::IntegrationPoint::IntegrationPoint(const double C, const double W, unique_ptr<Section>&& M)
    : coor(C)
    , weight(W)
    , b_section(move(M))
    , strain_mat(2, 3, fill::zeros) {}

B21H::B21H(const unsigned T, const uvec& N, const unsigned S, const double L, const bool F)
    : Element(T, ET_B21H, b_node, b_dof, N, uvec{ S }, F)
    , hinge_length(L) {}

void B21H::initialize(const shared_ptr<DomainBase>& D) {
    auto& coord_i = node_ptr.at(0).lock()->get_coordinate();
    auto& coord_j = node_ptr.at(1).lock()->get_coordinate();

    // chord vector
    const vec pos_diff = coord_j - coord_i;
    length = norm(pos_diff);
    direction_cosine = pos_diff / length;
    trans_mat = transform::beam::global_to_local(direction_cosine, length);
    inclination = transform::atan2(direction_cosine);

    auto& section_proto = D->get_section(unsigned(material_tag(0)));

    elastic_section_stiffness = section_proto->get_initial_stiffness();

    const IntegrationPlan plan(1, 2, IntegrationType::GAUSS);
    // add two inner points of Radau quadrature
    const auto int_pt_num = plan.n_rows + 2;
    const auto elastic_length = 1. - 8. * hinge_length;
    // elastic part will be reused in computation
    elastic_local_stiffness.zeros(3, 3);
    // build up the elastic interior
    elastic_int_pt.clear(), elastic_int_pt.reserve(int_pt_num);
    for(unsigned I = 0; I < int_pt_num; ++I) {
        double coor, weight;
        if(I == 0) {
            // left inner Radau point
            coor = 16. / 3. * hinge_length - 1.;
            weight = 3. * hinge_length;
        } else if(I == int_pt_num - 1) {
            // right inner Radau point
            coor = 1. - 16. / 3. * hinge_length;
            weight = 3. * hinge_length;
        } else {
            // Gauss points
            coor = plan(I - 1, 0) * elastic_length;
            weight = plan(I - 1, 1) * elastic_length / 2.;
        }
        elastic_int_pt.emplace_back(coor, weight, section_proto->get_copy());
        elastic_int_pt[I].strain_mat(0, 0) = 1. / length;
        elastic_int_pt[I].strain_mat(1, 1) = (3. * coor - 1.) / length;
        elastic_int_pt[I].strain_mat(1, 2) = (3. * coor + 1.) / length;
        elastic_local_stiffness += elastic_int_pt[I].strain_mat.t() * elastic_section_stiffness * elastic_int_pt[I].strain_mat * weight * length;
    }

    int_pt.clear(), int_pt.reserve(2);
    int_pt.emplace_back(-1., hinge_length, section_proto->get_copy());
    int_pt.emplace_back(1., hinge_length, section_proto->get_copy());
    for(auto& I : int_pt) {
        I.strain_mat(0, 0) = 1. / length;
        I.strain_mat(1, 1) = (3. * I.coor - 1.) / length;
        I.strain_mat(1, 2) = (3. * I.coor + 1.) / length;
    }
}

int B21H::update_status() {
    const auto& node_i = node_ptr.at(0).lock();
    const auto& node_j = node_ptr.at(1).lock();

    auto& disp_i = node_i->get_trial_displacement();
    auto& disp_j = node_j->get_trial_displacement();

    const auto new_length = length;

    // transform global deformation to local one (remove rigid body motion)
    vec t_disp(6);
    for(auto I = 0; I < 3; ++I) t_disp(I) = disp_i(I), t_disp(I + 3) = disp_j(I);

    const vec local_deformation = trans_mat * t_disp;

    auto local_stiffness = elastic_local_stiffness;
    vec local_resistance = elastic_local_stiffness * local_deformation;
    for(const auto& I : int_pt) {
        I.b_section->update_trial_status(I.strain_mat * local_deformation);
        const mat tmp_a = I.strain_mat.t() * I.weight * new_length;
        local_stiffness += tmp_a * I.b_section->get_stiffness() * I.strain_mat;
        local_resistance += tmp_a * I.b_section->get_resistance();
    }

    for(const auto& I : elastic_int_pt) I.b_section->update_trial_status(I.strain_mat * local_deformation);

    stiffness = trans_mat.t() * local_stiffness * trans_mat;
    resistance = trans_mat.t() * local_resistance;

    return 0;
}

int B21H::commit_status() {
    auto code = 0;
    for(const auto& I : int_pt) code += I.b_section->commit_status();
    return code;
}

int B21H::clear_status() {
    auto code = 0;
    for(const auto& I : int_pt) code += I.b_section->clear_status();
    return code;
}

int B21H::reset_status() {
    auto code = 0;
    for(const auto& I : int_pt) code += I.b_section->reset_status();
    return code;
}

void B21H::print() {}
