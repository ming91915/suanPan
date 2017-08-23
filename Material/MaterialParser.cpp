#include "MaterialParser.h"
#include <Material/Material>

using std::vector;

void new_elastic1d_(unique_ptr<Material>& return_obj, istringstream& command)
{
    unsigned tag;
    command >> tag;

    double elastic_modulus;
    command >> elastic_modulus;

    auto density = 0.;
    if(command.good())
        command >> density;
    else
        suanpan_debug("new_elastic1d_() assumes zero density.\n");

    return_obj = make_unique<Elastic1D>(tag, elastic_modulus, density);
}

void new_elastic2d_(unique_ptr<Material>& return_obj, istringstream& command)
{
    unsigned tag;
    command >> tag;

    double elastic_modulus;
    command >> elastic_modulus;

    double poissons_ratio;
    command >> poissons_ratio;

    auto density = 0.;
    auto material_type = 0;
    if(command.good()) {
        command >> density;
        if(command.good())
            command >> material_type;
        else
            suanpan_debug("new_elastic2d_() assumes plane stress.\n");
    } else
        suanpan_debug("new_elastic2d_() assumes zero density.\n");

    return_obj = make_unique<Elastic2D>(
        tag, elastic_modulus, poissons_ratio, density, material_type);
}

void new_elastic3d_(unique_ptr<Material>& return_obj, istringstream& command)
{
    unsigned tag;
    command >> tag;

    double elastic_modulus;
    command >> elastic_modulus;

    double poissons_ratio;
    command >> poissons_ratio;

    auto density = 0.;
    if(command.good())
        command >> density;
    else
        suanpan_debug("new_elastic3d_() assumes zero density.\n");

    return_obj = make_unique<Elastic3D>(tag, elastic_modulus, poissons_ratio, density);
}

void new_bilinear1d_(unique_ptr<Material>& return_obj, istringstream& command)
{
    unsigned tag;
    command >> tag;

    double elastic_modulus;
    command >> elastic_modulus;

    double yield_stress;
    command >> yield_stress;

    double hardening_ratio;
    command >> hardening_ratio;

    double beta;
    command >> beta;

    auto density = 0.;
    if(command.good())
        command >> density;
    else
        suanpan_debug("new_bilinear1d_() assumes zero density.\n");

    return_obj = make_unique<Bilinear1D>(
        tag, elastic_modulus, yield_stress, hardening_ratio, beta, density);
}
