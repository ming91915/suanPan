////////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2017 Theodore Chang
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
////////////////////////////////////////////////////////////////////////////////

#include <Domain/DomainBase.h>
#include <Domain/ExternalModule.h>
#include <Material/Material>
#include <Toolbox/utility.h>

int create_new_material(const shared_ptr<DomainBase>& domain, istringstream& command) {
    string material_id;
    if(!get_input(command, material_id)) {
        suanpan_info("create_new_material() needs a tag.\n");
        return 0;
    }

    unique_ptr<Material> new_material = nullptr;

    if(is_equal(material_id, "Elastic1D"))
        new_elastic1d(new_material, command);
    else if(is_equal(material_id, "Elastic2D"))
        new_elastic2d(new_material, command);
    else if(is_equal(material_id, "Elastic3D"))
        new_elastic3d(new_material, command);
    else if(is_equal(material_id, "Bilinear1D"))
        new_bilinear1d(new_material, command);
    else if(is_equal(material_id, "Bilinear2D"))
        new_bilinear2d(new_material, command);
    else if(is_equal(material_id, "Bilinear3D"))
        new_bilinear3d(new_material, command);
    else if(is_equal(material_id, "MPF"))
        new_mpf(new_material, command);
    else if(is_equal(material_id, "RambergOsgood"))
        new_rambergosgood(new_material, command);
    else if(is_equal(material_id, "Gap01"))
        new_gap01(new_material, command);
    else if(is_equal(material_id, "Concrete01"))
        new_concrete01(new_material, command);
    else {
        // check if the library is already loaded
        auto code = 0;
        for(const auto& I : domain->get_external_module_pool())
            if(I->library_name == material_id) {
                code = 1;
                break;
            }

        // not loaded then try load it
        if(code == 0 && domain->insert(make_shared<ExternalModule>(material_id))) code = 1;

        // if loaded find corresponding function
        if(code == 1)
            for(const auto& I : domain->get_external_module_pool()) {
                if(I->locate_module(material_id)) I->new_object(new_material, command);
                if(new_material != nullptr) break;
            }
    }

    if(new_material == nullptr || !domain->insert(move(new_material))) suanpan_debug("create_new_material() fails to insert new material.\n");

    return 0;
}

void new_elastic1d(unique_ptr<Material>& return_obj, istringstream& command) {
    unsigned tag;
    if(!get_input(command, tag)) {
        suanpan_error("new_elastic1d() requires a valid tag.\n");
        return;
    }

    double elastic_modulus;
    if(!get_input(command, elastic_modulus)) {
        suanpan_error("new_elastic1d() requires a valid elastic modulus.\n");
        return;
    }

    auto density = 0.;
    if(!command.eof()) {
        if(!get_input(command, density)) {
            suanpan_error("new_elastic1d() requires a valid density.\n");
            return;
        }
    } else
        suanpan_debug("new_elastic1d() assumes zero density.\n");

    return_obj = make_unique<Elastic1D>(tag, elastic_modulus, density);
}

void new_elastic2d(unique_ptr<Material>& return_obj, istringstream& command) {
    unsigned tag;
    if(!get_input(command, tag)) {
        suanpan_error("new_elastic2d() requires a valid tag.\n");
        return;
    }

    double elastic_modulus;
    if(!get_input(command, elastic_modulus)) {
        suanpan_error("new_elastic2d() requires a valid elastic modulus.\n");
        return;
    }

    double poissons_ratio;
    if(!get_input(command, poissons_ratio)) {
        suanpan_error("new_elastic2d() requires a valid poissons ratio.\n");
        return;
    }

    auto density = 0.;
    if(!command.eof()) {
        if(!get_input(command, density)) {
            suanpan_error("new_elastic2d() requires a valid density.\n");
            return;
        }
    } else
        suanpan_debug("new_elastic2d() assumes zero density.\n");

    auto material_type = 0;
    if(!command.eof()) {
        if(!get_input(command, material_type)) {
            suanpan_error("new_elastic2d() requires a valid material type.\n");
            return;
        }
    } else
        suanpan_debug("new_elastic2d() assumes plane stress.\n");

    return_obj = make_unique<Elastic2D>(tag, elastic_modulus, poissons_ratio, density, material_type == 0 ? PlaneType::S : PlaneType::E);
}

void new_elastic3d(unique_ptr<Material>& return_obj, istringstream& command) {
    unsigned tag;
    if(!get_input(command, tag)) {
        suanpan_error("new_elastic3d() requires a valid tag.\n");
        return;
    }

    double elastic_modulus;
    if(!get_input(command, elastic_modulus)) {
        suanpan_error("new_elastic3d() requires a valid elastic modulus.\n");
        return;
    }

    double poissons_ratio;
    if(!get_input(command, poissons_ratio)) {
        suanpan_error("new_elastic3d() requires a valid poissons ratio.\n");
        return;
    }

    auto density = 0.;
    if(!command.eof()) {
        if(!get_input(command, density)) {
            suanpan_error("new_elastic3d() requires a valid density.\n");
            return;
        }
    } else
        suanpan_debug("new_elastic3d() assumes zero density.\n");

    return_obj = make_unique<Elastic3D>(tag, elastic_modulus, poissons_ratio, density);
}

void new_bilinear1d(unique_ptr<Material>& return_obj, istringstream& command) {
    unsigned tag;
    if(!get_input(command, tag)) {
        suanpan_error("new_bilinear1d() requires a valid tag.\n");
        return;
    }

    double elastic_modulus;
    if(!get_input(command, elastic_modulus)) {
        suanpan_error("new_bilinear1d() requires a valid elastic modulus.\n");
        return;
    }

    double yield_stress;
    if(!get_input(command, yield_stress)) {
        suanpan_error("new_bilinear1d() requires a valid yield stress.\n");
        return;
    }

    auto hardening_ratio = 0.;
    if(!command.eof()) {
        if(!get_input(command, hardening_ratio)) {
            suanpan_error("new_bilinear1d() requires a valid hardening ratio.\n");
            return;
        }
    } else
        suanpan_debug("new_bilinear1d() assumes zero hardening ratio.\n");

    auto beta = 0.;
    if(!command.eof()) {
        if(!get_input(command, beta)) {
            suanpan_error("new_bilinear1d() requires a valid beta.\n");
            return;
        }
    } else
        suanpan_debug("new_bilinear1d() assumes isotropic hardening.\n");

    auto density = 0.;
    if(!command.eof()) {
        if(!get_input(command, density)) {
            suanpan_error("new_bilinear1d() requires a valid density.\n");
            return;
        }
    } else
        suanpan_debug("new_bilinear1d() assumes zero density.\n");

    return_obj = make_unique<Bilinear1D>(tag, elastic_modulus, yield_stress, hardening_ratio, beta, density);
}

void new_bilinear2d(unique_ptr<Material>& return_obj, istringstream& command) {
    unsigned tag;
    if(!get_input(command, tag)) {
        suanpan_error("new_bilinear2d() requires a valid tag.\n");
        return;
    }

    double elastic_modulus;
    if(!get_input(command, elastic_modulus)) {
        suanpan_error("new_bilinear2d() requires a valid elastic modulus.\n");
        return;
    }

    double poissons_ratio;
    if(!get_input(command, poissons_ratio)) {
        suanpan_error("new_bilinear2d() requires a valid poissons ratio.\n");
        return;
    }

    double yield_stress;
    if(!get_input(command, yield_stress)) {
        suanpan_error("new_bilinear2d() requires a valid yield stress.\n");
        return;
    }

    auto hardening_ratio = 0.;
    if(!command.eof()) {
        if(!get_input(command, hardening_ratio)) {
            suanpan_error("new_bilinear2d() requires a valid hardening ratio.\n");
            return;
        }
    } else
        suanpan_debug("new_bilinear2d() assumes zero hardening ratio.\n");

    auto beta = 0.;
    if(!command.eof()) {
        if(!get_input(command, beta)) {
            suanpan_error("new_bilinear2d() requires a valid beta.\n");
            return;
        }
    } else
        suanpan_debug("new_bilinear2d() assumes isotropic hardening.\n");

    unsigned material_type = 0;
    if(!get_input(command, material_type)) {
        suanpan_error("new_bilinear2d() requires a valid material type.\n");
        return;
    }

    auto density = 0.;
    if(!command.eof()) {
        if(!get_input(command, density)) {
            suanpan_error("new_bilinear2d() requires a valid density.\n");
            return;
        }
    } else
        suanpan_debug("new_bilinear2d() assumes zero density.\n");

    return_obj = make_unique<Bilinear2D>(tag, elastic_modulus, poissons_ratio, yield_stress, hardening_ratio, beta, material_type == 0 ? PlaneType::S : PlaneType::E, density);
}

void new_bilinear3d(unique_ptr<Material>& return_obj, istringstream& command) {
    unsigned tag;
    if(!get_input(command, tag)) {
        suanpan_error("new_bilinear3d() requires a valid tag.\n");
        return;
    }

    double elastic_modulus;
    if(!get_input(command, elastic_modulus)) {
        suanpan_error("new_bilinear3d() requires a valid elastic modulus.\n");
        return;
    }

    double poissons_ratio;
    if(!get_input(command, poissons_ratio)) {
        suanpan_error("new_bilinear3d() requires a valid poissons ratio.\n");
        return;
    }

    double yield_stress;
    if(!get_input(command, yield_stress)) {
        suanpan_error("new_bilinear3d() requires a valid yield stress.\n");
        return;
    }

    auto hardening_ratio = 0.;
    if(!command.eof()) {
        if(!get_input(command, hardening_ratio)) {
            suanpan_error("new_bilinear3d() requires a valid hardening ratio.\n");
            return;
        }
    } else
        suanpan_debug("new_bilinear3d() assumes zero hardening ratio.\n");

    auto beta = 0.;
    if(!command.eof()) {
        if(!get_input(command, beta)) {
            suanpan_error("new_bilinear3d() requires a valid beta.\n");
            return;
        }
    } else
        suanpan_debug("new_bilinear3d() assumes isotropic hardening.\n");

    auto density = 0.;
    if(!command.eof()) {
        if(!get_input(command, density)) {
            suanpan_error("new_bilinear3d() requires a valid density.\n");
            return;
        }
    } else
        suanpan_debug("new_bilinear3d() assumes zero density.\n");

    return_obj = make_unique<Bilinear3D>(tag, elastic_modulus, poissons_ratio, yield_stress, hardening_ratio, beta, density);
}

void new_concrete01(unique_ptr<Material>& return_obj, istringstream& command) {
    unsigned tag;
    if(!get_input(command, tag)) {
        suanpan_error("new_concrete01() requires a valid tag.\n");
        return;
    }

    double peak_c_strain;
    if(!get_input(command, peak_c_strain)) {
        suanpan_error("new_concrete01() requires a valid compression strain.\n");
        return;
    }

    double peak_c_stress;
    if(!get_input(command, peak_c_stress)) {
        suanpan_error("new_concrete01() requires a valid compression stress.\n");
        return;
    }

    string backbone_type;
    if(!get_input(command, backbone_type)) {
        suanpan_error("new_concrete01() requires a valid backbone type.\n");
        return;
    }

    string center_oriented = "false";
    if(!command.eof() && !get_input(command, center_oriented)) {
        suanpan_error("new_concrete01() requires a valid center oriented switch.\n");
        return;
    }

    auto density = 0.;
    if(!command.eof()) {
        if(!get_input(command, density)) {
            suanpan_error("new_concrete01() requires a valid density.\n");
            return;
        }
    } else
        suanpan_extra_debug("new_concrete01() assumes zero density.\n");

    BackboneType type;
    if(is_equal(backbone_type, "THORENFELDT"))
        type = BackboneType::THORENFELDT;
    else if(is_equal(backbone_type, "POPOVICS"))
        type = BackboneType::POPOVICS;
    else if(is_equal(backbone_type, "TSAI"))
        type = BackboneType::TSAI;
    else if(is_equal(backbone_type, "KPSC"))
        type = BackboneType::KPSC;
    else if(is_equal(backbone_type, "KPSU"))
        type = BackboneType::KPSU;

    return_obj = make_unique<Concrete01>(tag, peak_c_strain, peak_c_stress, type, is_true(center_oriented), density);
}

void new_gap01(unique_ptr<Material>& return_obj, istringstream& command) {
    unsigned tag;
    if(!get_input(command, tag)) {
        suanpan_error("new_gap01() requires a valid tag.\n");
        return;
    }

    double elastic_modulus;
    if(!get_input(command, elastic_modulus)) {
        suanpan_error("new_gap01() requires a valid elastic modulus.\n");
        return;
    }

    double yield_stress;
    if(!get_input(command, yield_stress)) {
        suanpan_error("new_gap01() requires a valid yield stress.\n");
        return;
    }

    auto gap_strain = 0.;
    if(!command.eof() && !get_input(command, gap_strain)) {
        suanpan_error("new_gap01() requires a valid hardening ratio.\n");
        return;
    }

    auto density = 0.;
    if(!command.eof() && !get_input(command, density)) {
        suanpan_error("new_gap01() requires a valid density.\n");
        return;
    }

    return_obj = make_unique<Gap01>(tag, elastic_modulus, yield_stress, gap_strain, density);
}

void new_mpf(unique_ptr<Material>& return_obj, istringstream& command) {
    unsigned tag;
    if(!get_input(command, tag)) {
        suanpan_error("new_mpf() requires a valid tag.\n");
        return;
    }

    double elastic_modulus;
    if(!get_input(command, elastic_modulus)) {
        suanpan_error("new_mpf() requires a valid elastic modulus.\n");
        return;
    }

    double yield_stress;
    if(!get_input(command, yield_stress)) {
        suanpan_error("new_mpf() requires a valid yield stress.\n");
        return;
    }

    auto hardening_ratio = 0.05;
    if(!command.eof() && !get_input(command, hardening_ratio)) {
        suanpan_error("new_mpf() requires a valid hardening ratio.\n");
        return;
    }

    auto R0 = 20.;
    if(!command.eof() && !get_input(command, R0)) {
        suanpan_error("new_mpf() requires a valid R0.\n");
        return;
    }

    auto A1 = 18.5;
    if(!command.eof() && !get_input(command, A1)) {
        suanpan_error("new_mpf() requires a valid A1.\n");
        return;
    }

    auto A2 = .15;
    if(!command.eof() && !get_input(command, A2)) {
        suanpan_error("new_mpf() requires a valid A2.\n");
        return;
    }

    auto A3 = .01;
    if(!command.eof() && !get_input(command, A3)) {
        suanpan_error("new_mpf() requires a valid A3.\n");
        return;
    }

    auto A4 = 7.;
    if(!command.eof() && !get_input(command, A4)) {
        suanpan_error("new_mpf() requires a valid A4.\n");
        return;
    }

    string iso = "false";
    if(!command.eof() && !get_input(command, iso)) {
        suanpan_error("new_mpf() requires a valid isotropic hardening switch.\n");
        return;
    }

    string con = "false";
    if(!command.eof() && !get_input(command, con)) {
        suanpan_error("new_mpf() requires a valid constant radius switch.\n");
        return;
    }

    auto density = 0.;
    if(!command.eof() && !get_input(command, density)) {
        suanpan_error("new_mpf() requires a valid density.\n");
        return;
    }

    return_obj = make_unique<MPF>(tag, elastic_modulus, yield_stress, hardening_ratio, R0, A1, A2, A3, A4, is_true(iso), is_true(con), density);
}

void new_rambergosgood(unique_ptr<Material>& return_obj, istringstream& command) {
    unsigned tag;
    if(!get_input(command, tag)) {
        suanpan_error("new_rambergosgood() requires a valid tag.\n");
        return;
    }

    double elastic_modulus;
    if(!get_input(command, elastic_modulus)) {
        suanpan_error("new_rambergosgood() requires a valid elastic modulus.\n");
        return;
    }

    double yield_stress;
    if(!get_input(command, yield_stress)) {
        suanpan_error("new_rambergosgood() requires a valid yield stress.\n");
        return;
    }

    auto offset = 1;
    if(!command.eof() && !get_input(command, offset)) {
        suanpan_error("new_rambergosgood() requires a valid offset.\n");
        return;
    }

    auto n = 10.;
    if(!command.eof() && !get_input(command, n)) {
        suanpan_error("new_rambergosgood() requires a valid n.\n");
        return;
    }

    auto density = 0.;
    if(!command.eof() && !get_input(command, density)) {
        suanpan_error("new_rambergosgood() requires a valid density.\n");
        return;
    }

    return_obj = make_unique<RambergOsgood>(tag, elastic_modulus, yield_stress, offset, n, density);
}

void new_maxwell(unique_ptr<Material>& return_obj, istringstream& command) {
    unsigned tag;
    if(!get_input(command, tag)) {
        suanpan_error("new_maxwell() requires a valid tag.\n");
        return;
    }

    double elastic_modulus;
    if(!get_input(command, elastic_modulus)) {
        suanpan_error("new_maxwell() requires a valid elastic modulus.\n");
        return;
    }

    double alpha;
    if(!get_input(command, alpha)) {
        suanpan_error("new_maxwell() requires a valid alpha.\n");
        return;
    }

    double damping_postive;
    if(!get_input(command, damping_postive)) {
        suanpan_error("new_maxwell() requires a valid damping coefficient.\n");
        return;
    }

    auto damping_negative = damping_postive;
    if(!command.eof() && !get_input(command, damping_negative)) {
        suanpan_error("new_maxwell() requires a valid damping coefficient.\n");
        return;
    }

    return_obj = make_unique<Maxwell>(tag, elastic_modulus, alpha, damping_postive, damping_negative);
}

int test_material(const shared_ptr<DomainBase>& domain, istringstream& command) {
    unsigned material_tag;
    if(!get_input(command, material_tag)) {
        suanpan_error("test_material() needs a valid material tag.\n");
        return 0;
    }

    double step_size;
    if(!get_input(command, step_size)) {
        suanpan_error("test_material() needs a valid step size.\n");
        return 0;
    }

    vector<unsigned> load_step;
    unsigned step;
    while(get_input(command, step)) load_step.push_back(step);

    auto& material_proto = domain->get_material(material_tag);

    if(material_proto == nullptr) return 0;

    auto result = material_tester(material_proto->get_copy(), load_step, step_size);

    result.save("RESULT.h5", hdf5_binary_trans);

    return 0;
}
