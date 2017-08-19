#include "Bilinear2D.h"

Bilinear2D::Bilinear2D(const unsigned& T,
    const double& E,
    const double& V,
    const double& Y,
    const double& H,
    const double& B,
    const unsigned& M,
    const double& D)
    : Material(T, MT_BILINEAR2D)
    , material_type(M)
    , base(0, E, V, Y, H, B, D)
{
    density = D;

    auto EE = material_type == 0 ? E : E / (1 - V * V);

    auto VV = material_type == 0 ? V : V / (1 - V);

    initial_stiffness.zeros(3, 3);
    initial_stiffness(0, 0) = 1;
    initial_stiffness(1, 1) = 1;
    initial_stiffness(2, 2) = (1. - VV) / 2.;
    initial_stiffness(0, 1) = VV;
    initial_stiffness(1, 0) = VV;
    initial_stiffness *= EE / (1. - VV * VV);

    Bilinear2D::initialize();
}

void Bilinear2D::initialize()
{
    trial_full_strain.zeros(6);

    current_strain.zeros(3);
    current_stress.zeros(3);
    trial_strain.zeros(3);
    trial_stress.zeros(3);

    current_stiffness = initial_stiffness;
    trial_stiffness = initial_stiffness;
}

double Bilinear2D::getParameter(const unsigned& T) const { return base.getParameter(T); }

unique_ptr<Material> Bilinear2D::getCopy() { return make_unique<Bilinear2D>(*this); }

int Bilinear2D::updateIncreStatus(const vec& i_strain)
{
    return updateTrialStatus(current_strain + i_strain);
}

int Bilinear2D::updateTrialStatus(const vec& t_strain)
{
    trial_full_strain(0) = t_strain(0);
    trial_full_strain(1) = t_strain(1);
    trial_full_strain(3) = t_strain(2);

    base.updateTrialStatus(trial_full_strain);

    // PLANE STRESS
    if(material_type == 0)
        while(fabs(base.getStress().at(2)) > 1E-10) {
            trial_full_strain(2) -= base.getStress().at(2) / base.getStiffness().at(2, 2);
            base.updateTrialStatus(trial_full_strain);
        }

    auto& tmp_strain = base.getStrain();
    trial_strain(0) = tmp_strain.at(0);
    trial_strain(1) = tmp_strain.at(1);
    trial_strain(2) = tmp_strain.at(3);

    auto& tmp_stress = base.getStress();
    trial_stress(0) = tmp_stress.at(0);
    trial_stress(1) = tmp_stress.at(1);
    trial_stress(2) = tmp_stress.at(3);

    auto& tmp_stiffness = base.getStiffness();
    trial_stiffness(0, 0) = tmp_stiffness.at(0, 0);
    trial_stiffness(0, 1) = tmp_stiffness.at(0, 1);
    trial_stiffness(0, 2) = tmp_stiffness.at(0, 3);
    trial_stiffness(1, 0) = tmp_stiffness.at(1, 0);
    trial_stiffness(1, 1) = tmp_stiffness.at(1, 1);
    trial_stiffness(1, 2) = tmp_stiffness.at(1, 3);
    trial_stiffness(2, 0) = tmp_stiffness.at(3, 0);
    trial_stiffness(2, 1) = tmp_stiffness.at(3, 1);
    trial_stiffness(2, 2) = tmp_stiffness.at(3, 3);

    if(material_type == 0) {
        if(tmp_stiffness(2, 2) != 0.) {
            auto tmp_a = tmp_stiffness(2, 0) / tmp_stiffness(2, 2);
            auto tmp_b = tmp_stiffness(2, 1) / tmp_stiffness(2, 2);
            auto tmp_c = tmp_stiffness(2, 3) / tmp_stiffness(2, 2);
            trial_stiffness(0, 0) -= tmp_stiffness(0, 2) * tmp_a;
            trial_stiffness(0, 1) -= tmp_stiffness(0, 2) * tmp_b;
            trial_stiffness(0, 2) -= tmp_stiffness(0, 2) * tmp_c;
            trial_stiffness(1, 0) -= tmp_stiffness(1, 2) * tmp_a;
            trial_stiffness(1, 1) -= tmp_stiffness(1, 2) * tmp_b;
            trial_stiffness(1, 2) -= tmp_stiffness(1, 2) * tmp_c;
            trial_stiffness(2, 0) -= tmp_stiffness(3, 2) * tmp_a;
            trial_stiffness(2, 1) -= tmp_stiffness(3, 2) * tmp_b;
            trial_stiffness(2, 2) -= tmp_stiffness(3, 2) * tmp_c;
        } else
            suanpan_error("K(2,2)=0.\n");
    }

    return 0;
}

int Bilinear2D::clearStatus()
{
    initialize();
    return base.clearStatus();
}

int Bilinear2D::commitStatus()
{
    current_strain = trial_strain;
    current_stress = trial_stress;
    current_stiffness = trial_stiffness;

    return base.commitStatus();
}

int Bilinear2D::resetStatus()
{
    trial_strain = current_strain;
    trial_stress = current_stress;
    trial_stiffness = current_stiffness;

    return base.resetStatus();
}
