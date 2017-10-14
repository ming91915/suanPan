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

#include <Material/Material>
#include <Toolbox/utility.h>

using std::vector;

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

    auto iso = 0;
    if(!command.eof() && !get_input(command, iso)) {
        suanpan_error("new_mpf() requires a valid isotropic hardening switch.\n");
        return;
    }

    auto con = 0;
    if(!command.eof() && !get_input(command, con)) {
        suanpan_error("new_mpf() requires a valid constant R switch.\n");
        return;
    }

    auto density = 0.;
    if(!command.eof() && !get_input(command, density)) {
        suanpan_error("new_mpf() requires a valid density.\n");
        return;
    }

    return_obj = make_unique<MPF>(tag, elastic_modulus, yield_stress, hardening_ratio, R0, A1, A2, A3, A4, !!iso, !!con, density);
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