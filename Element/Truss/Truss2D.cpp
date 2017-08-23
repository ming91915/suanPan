#include "Truss2D.h"

Truss2D::Truss2D(const unsigned& T)
    : Element(T, ET_TRUSS2D)
{
}

Truss2D::Truss2D(const unsigned& T,
    const uvec& N,
    const unsigned& M,
    const double& A,
    const bool& F,
    const bool& UA,
    const bool& LS)
    : Element(T, ET_TRUSS2D, 2, 2, N, uvec({ M }), F)
    , area(A)
    , direction_cosine(2)
    , update_area(UA)
    , log_strain(LS)
{
}

void Truss2D::initialize(const shared_ptr<Domain>& D)
{
    vec pos_diff =
        node_ptr.at(0).lock()->get_coordinate() - node_ptr.at(1).lock()->get_coordinate();

    length = norm(pos_diff);

    try {
        t_material = D->get_material(static_cast<unsigned>(material_tag(0)))->get_copy();
    } catch(const out_of_range&) {
        printf("Truss2D %u cannot find a valid Material %u\n", get_tag(),
            static_cast<unsigned>(material_tag(0)));
        throw;
    }

    direction_cosine = pos_diff / length;

    auto tmp_d = area / length * as_scalar(t_material->get_initial_stiffness());
    auto tmp_a = tmp_d * direction_cosine(0) * direction_cosine(0);
    auto tmp_b = tmp_d * direction_cosine(1) * direction_cosine(1);
    auto tmp_c = tmp_d * direction_cosine(0) * direction_cosine(1);

    initial_stiffness(0, 0) = tmp_a;
    initial_stiffness(0, 1) = tmp_c;
    initial_stiffness(0, 2) = -tmp_a;
    initial_stiffness(0, 3) = -tmp_c;

    initial_stiffness(1, 0) = tmp_c;
    initial_stiffness(1, 1) = tmp_b;
    initial_stiffness(1, 2) = -tmp_c;
    initial_stiffness(1, 3) = -tmp_b;

    initial_stiffness(2, 0) = -tmp_a;
    initial_stiffness(2, 1) = -tmp_c;
    initial_stiffness(2, 2) = tmp_a;
    initial_stiffness(2, 3) = tmp_c;

    initial_stiffness(3, 0) = -tmp_c;
    initial_stiffness(3, 1) = -tmp_b;
    initial_stiffness(3, 2) = tmp_c;
    initial_stiffness(3, 3) = tmp_b;
}

int Truss2D::update_status()
{
    auto node_i = node_ptr.at(0).lock();
    auto node_j = node_ptr.at(1).lock();

    auto& t_disp_i = node_i->get_trial_displacement();
    auto& t_disp_j = node_j->get_trial_displacement();

    double trial_strain;

    vec disp_diff = t_disp_j - t_disp_i;

    auto new_area = area;
    auto new_length = length;

    if(nlgeom) {
        auto& coord_i = node_i->get_coordinate();
        auto& coord_j = node_j->get_coordinate();
        vec pos_diff = disp_diff + coord_j - coord_i;

        new_length = norm(pos_diff);

        direction_cosine = pos_diff / new_length;

        if(update_area) new_area = area * length / new_length;

        if(log_strain)
            trial_strain = log(new_length / length);
        else
            trial_strain = new_length / length - 1.;
    } else
        trial_strain = dot(disp_diff, direction_cosine) / length;

    t_material->update_trial_status({ trial_strain });

    auto tmp_d = new_area / new_length * as_scalar(t_material->get_stiffness());
    auto tmp_a = tmp_d * direction_cosine(0) * direction_cosine(0);
    auto tmp_b = tmp_d * direction_cosine(1) * direction_cosine(1);
    auto tmp_c = tmp_d * direction_cosine(0) * direction_cosine(1);

    stiffness(0, 0) = tmp_a;
    stiffness(0, 1) = tmp_c;
    stiffness(0, 2) = -tmp_a;
    stiffness(0, 3) = -tmp_c;
    stiffness(1, 1) = tmp_b;
    stiffness(1, 2) = -tmp_c;
    stiffness(1, 3) = -tmp_b;
    stiffness(2, 2) = tmp_a;
    stiffness(2, 3) = tmp_c;
    stiffness(3, 3) = tmp_b;

    if(nlgeom) {
        auto tmp_e = new_area / new_length * as_scalar(t_material->get_stress());
        stiffness(0, 0) += tmp_e;
        stiffness(1, 1) += tmp_e;
        stiffness(2, 2) += tmp_e;
        stiffness(3, 3) += tmp_e;
        stiffness(0, 2) -= tmp_e;
        stiffness(1, 3) -= tmp_e;
    }

    for(auto I = 0; I < 3; ++I)
        for(auto J = I + 1; J < 4; ++J) stiffness(J, I) = stiffness(I, J);

    auto tmp_f = new_area * as_scalar(t_material->get_stress());
    resistance(2) = tmp_f * direction_cosine(0);
    resistance(3) = tmp_f * direction_cosine(1);
    resistance(0) = -resistance(2);
    resistance(1) = -resistance(3);

    return 0;
}

int Truss2D::commit_status() { return t_material->commit_status(); }

int Truss2D::clear_status() { return t_material->clear_status(); }

int Truss2D::reset_status() { return t_material->reset_status(); }

void Truss2D::print()
{
    printf("A 2-D Truss Element.\t");
    printf("Node I:\t%u\t", static_cast<unsigned>(node_encoding(0)));
    printf("Node J:\t%u\n\n", static_cast<unsigned>(node_encoding(1)));
    printf("Area:\t%.4E\n", area);
    printf("Length:\t%.4E\n\n", length);
    printf("Material Model:\n\n");
    t_material->print();
}
