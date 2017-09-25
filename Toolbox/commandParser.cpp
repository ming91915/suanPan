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

#include "commandParser.h"
#include "argumentParser.h"
#include <Constraint/BC/BC.h>
#include <Converger/Converger>
#include <Domain/Domain.h>
#include <Domain/ExternalModule.h>
#include <Domain/Node.h>
#include <Element/Element.h>
#include <Element/ElementParser.h>
#include <Load/CLoad.h>
#include <Load/DisplacementLoad.h>
#include <Material/MaterialParser.h>
#include <Recorder/NodeRecorder.h>
#include <Solver/Solver>
#include <Step/ArcLength.h>
#include <Step/Bead.h>
#include <Step/Dynamic.h>
#include <Step/Static.h>
#include <Toolbox/utility.h>

using std::ifstream;
using std::string;
using std::vector;

int process_command(const shared_ptr<Bead>& model, istringstream& command) {
    if(model == nullptr) return 0;

    const auto command_id = get_input<string>(command);
    if(command.fail()) return 0;

    if(command_id == "exit") return SUANPAN_EXIT;
    if(command_id == "quit") return SUANPAN_EXIT;

    if(command_id == "file") return process_file(model, command);

    if(command_id == "domain") return create_new_domain(model, command);
    if(command_id == "converger") return create_new_converger(model, command);
    if(command_id == "step") return create_new_step(model, command);
    if(command_id == "solver") return create_new_solver(model, command);
    if(command_id == "fix") return create_new_bc(model, command);
    if(command_id == "cload") return create_new_cload(model, command);
    if(command_id == "disp") return create_new_dispload(model, command);
    if(command_id == "dispload") return create_new_dispload(model, command);
    if(command_id == "enable") return enable_object(model, command);
    if(command_id == "disable") return disable_object(model, command);
    if(command_id == "mute") return disable_object(model, command);
    if(command_id == "erase") return erase_object(model, command);
    if(command_id == "delete") return erase_object(model, command);
    if(command_id == "remove") return erase_object(model, command);
    if(command_id == "set") return set_property(model, command);

    const auto& domain = get_current_domain(model);
    if(domain == nullptr) return 0;

    if(command_id == "node") return create_new_node(domain, command);
    if(command_id == "material") return create_new_material(domain, command);
    if(command_id == "element") return create_new_element(domain, command);
    if(command_id == "recorder") return create_new_recorder(domain, command);

    if(command_id == "analyze") return model->analyze();

    if(command_id == "clear") {
        domain->clear_status();
        return 0;
    }

    if(command_id == "summary") {
        domain->summary();
        return 0;
    }

    if(command_id == "peek") return print_info(domain, command);

    if(command_id == "version")
        print_version();
    else if(command_id == "help")
        print_command_usage(command);

    return 0;
}

int process_file(const shared_ptr<Bead>& model, const char* file_name) {
    ifstream input_file(file_name);

    if(!input_file.is_open()) {
        suanpan_error("process_file() cannot open the input file.\n");
        return 0;
    }

    string command_line;
    while(!getline(input_file, command_line).fail()) {
        if(!command_line.empty() && command_line[0] != '#') {
            istringstream tmp_str(command_line);
            if(process_command(model, tmp_str) == SUANPAN_EXIT) return SUANPAN_EXIT;
        }
    }
    return 0;
}

int process_file(const shared_ptr<Bead>& model, istringstream& command) {
    string file_name;
    if(!get_input(command, file_name)) {
        suanpan_info("process_file() needs a file name.\n");
        return 0;
    }

    return process_file(model, file_name.c_str());
}

int create_new_domain(const shared_ptr<Bead>& model, istringstream& command) {
    unsigned domain_id;
    if((command >> domain_id).fail()) {
        suanpan_info("create_new_domain() requires a tag.\n");
        return 0;
    }

    model->set_current_domain(domain_id);

    auto& tmp_domain = get_domain(model, domain_id);

    if(tmp_domain == nullptr) {
        tmp_domain = make_shared<Domain>(domain_id);
        if(tmp_domain != nullptr) suanpan_info("create_new_domain() successfully creates Domain %u.\n", domain_id);
    } else
        suanpan_info("create_new_domain() switches to Domain %u.\n", domain_id);

    return 0;
}

int create_new_step(const shared_ptr<Bead>& model, istringstream& command) {
    string step_type;
    if((command >> step_type).fail()) {
        suanpan_info("create_new_step() requires step type.\n");
        return 0;
    }

    unsigned tag;
    if((command >> tag).fail()) {
        suanpan_info("create_new_step() requires a tag.\n");
        return 0;
    }

    if(if_equal(step_type, "Static")) {
        auto time = 1.;
        if(!command.eof() && (command >> time).fail()) {
            suanpan_info("create_new_step() reads a wrong time period.\n");
            return 0;
        }
        if(model->insert(make_shared<Static>(tag, time))) {
            model->set_current_step(tag);
            model->get_current_step()->set_domain(model->get_current_domain());
        } else
            suanpan_error("create_new_step() cannot create the new step.\n");
    } else if(if_equal(step_type, "Dynamic")) {
        auto time = 1.;
        if(!command.eof() && (command >> time).fail()) {
            suanpan_info("create_new_step() reads a wrong time period.\n");
            return 0;
        }
        if(model->insert(make_shared<Dynamic>(tag, time))) {
            model->set_current_step(tag);
            model->get_current_step()->set_domain(model->get_current_domain());
        } else
            suanpan_error("create_new_step() cannot create the new step.\n");
    } else if(if_equal(step_type, "ArcLength")) {
        unsigned node;
        if((command >> node).fail()) {
            suanpan_info("create_new_step() requires a node.\n");
            return 0;
        }

        unsigned dof;
        if((command >> dof).fail()) {
            suanpan_info("create_new_step() requires a dof.\n");
            return 0;
        }

        double magnitude;
        if((command >> magnitude).fail()) {
            suanpan_info("create_new_step() requires a magnitude.\n");
            return 0;
        }

        if(model->insert(make_shared<ArcLength>(tag, node, dof, magnitude))) {
            model->set_current_step(tag);
            model->get_current_step()->set_domain(model->get_current_domain());
        } else
            suanpan_error("create_new_step() cannot create the new step.\n");
    } else
        suanpan_info("create_new_step() cannot identify step type.\n");

    return 0;
}

int create_new_converger(const shared_ptr<Bead>& model, istringstream& command) {
    const auto& t_step = get_current_step(model);
    if(t_step == nullptr) {
        suanpan_info("create_new_converger() needs a valid step.\n");
        return 0;
    }

    string converger_id;
    if((command >> converger_id).fail()) {
        suanpan_info("create_new_converger() requires converger type.\n");
        return 0;
    }

    unsigned tag;
    if((command >> tag).fail()) {
        suanpan_info("create_new_converger() requires a tag.\n");
        return 0;
    }

    auto tolerance = 1E-8;
    if(!command.eof() && (command >> tolerance).fail()) {
        suanpan_info("create_new_converger() reads wrong tolerance.\n");
        return 0;
    }

    auto max_iteration = 10;
    if(!command.eof() && (command >> max_iteration).fail()) {
        suanpan_info("create_new_converger() reads wrong max iteration.\n");
        return 0;
    }

    auto print_flag = 0;
    if(!command.eof() && (command >> print_flag).fail()) {
        suanpan_info("create_new_converger() reads wrong print flag.\n");
        return 0;
    }

    if(if_equal(converger_id, "AbsResidual")) {
        if(model->insert(make_shared<AbsResidual>(tag, nullptr, tolerance, max_iteration, !!print_flag)))
            t_step->set_converger(model->get_converger(tag));
        else
            suanpan_info("create_new_converger() fails to create the new converger.\n");
    } else if(if_equal(converger_id, "RelResidual")) {
        if(model->insert(make_shared<RelResidual>(tag, nullptr, tolerance, max_iteration, !!print_flag)))
            t_step->set_converger(model->get_converger(tag));
        else
            suanpan_info("create_new_converger() fails to create the new converger.\n");
    } else if(if_equal(converger_id, "AbsIncreDisp")) {
        if(model->insert(make_shared<AbsIncreDisp>(tag, nullptr, tolerance, max_iteration, !!print_flag)))
            t_step->set_converger(model->get_converger(tag));
        else
            suanpan_info("create_new_converger() fails to create the new converger.\n");
    } else if(if_equal(converger_id, "RelIncreDisp")) {
        if(model->insert(make_shared<RelIncreDisp>(tag, nullptr, tolerance, max_iteration, !!print_flag)))
            t_step->set_converger(model->get_converger(tag));
        else
            suanpan_info("create_new_converger() fails to create the new converger.\n");
    } else if(if_equal(converger_id, "AbsIncreEnergy")) {
        if(model->insert(make_shared<AbsIncreEnergy>(tag, nullptr, tolerance, max_iteration, !!print_flag)))
            t_step->set_converger(model->get_converger(tag));
        else
            suanpan_info("create_new_converger() fails to create the new converger.\n");
    } else if(if_equal(converger_id, "RelIncreEnergy")) {
        if(model->insert(make_shared<RelIncreEnergy>(tag, nullptr, tolerance, max_iteration, !!print_flag)))
            t_step->set_converger(model->get_converger(tag));
        else
            suanpan_info("create_new_converger() fails to create the new converger.\n");
    } else
        suanpan_info("create_new_converger() cannot identify the converger type.\n");

    return 0;
}

int create_new_solver(const shared_ptr<Bead>& model, istringstream& command) {
    const auto& tmp_step = get_current_step(model);
    if(tmp_step == nullptr) {
        suanpan_info("create_new_solver() needs a valid step.\n");
        return 0;
    }

    string solver_type;
    if((command >> solver_type).fail()) {
        suanpan_info("create_new_solver() requires solver type.\n");
        return 0;
    }

    unsigned tag;
    if((command >> tag).fail()) {
        suanpan_info("create_new_solver() requires a tag.\n");
        return 0;
    }

    if(if_equal(solver_type, "Newton")) {
        if(model->insert(make_shared<Newton>(tag)))
            tmp_step->set_solver(model->get_solver(tag));
        else
            suanpan_error("create_new_solver() cannot create the new solver.\n");
    } else if(if_equal(solver_type, "BFGS")) {
        if(model->insert(make_shared<BFGS>(tag)))
            tmp_step->set_solver(model->get_solver(tag));
        else
            suanpan_error("create_new_solver() cannot create the new solver.\n");
    } else
        suanpan_error("create_new_solver() cannot identify solver type.\n");

    return 0;
}

int create_new_bc(const shared_ptr<Bead>& model, istringstream& command) {
    unsigned bc_id;
    if((command >> bc_id).fail()) {
        suanpan_info("create_new_bc() needs BC type.\n");
        return 0;
    }

    string dof_id;
    if((command >> dof_id).fail()) {
        suanpan_info("create_new_bc() needs valid DoFs.\n");
        return 0;
    }

    unsigned node;
    vector<uword> node_tag;
    while(!(command >> node).fail()) node_tag.push_back(node);

    const auto& domain = model->get_current_domain();
    const auto& step_tag = model->get_current_step_tag();

    const auto bc_type = tolower(dof_id[0]);
    if(bc_type == 'p')
        domain->insert(make_shared<BC>(bc_id, step_tag, uvec(node_tag), "PINNED"));
    else if(bc_type == 'e')
        domain->insert(make_shared<BC>(bc_id, step_tag, uvec(node_tag), "ENCASTRE"));
    else if(bc_type == 'x')
        domain->insert(make_shared<BC>(bc_id, step_tag, uvec(node_tag), "XSYMM"));
    else if(bc_type == 'y')
        domain->insert(make_shared<BC>(bc_id, step_tag, uvec(node_tag), "YSYMM"));
    else if(bc_type == 'z')
        domain->insert(make_shared<BC>(bc_id, step_tag, uvec(node_tag), "ZSYMM"));
    else if(bc_type == '1')
        domain->insert(make_shared<BC>(bc_id, step_tag, uvec(node_tag), 1));
    else if(bc_type == '2')
        domain->insert(make_shared<BC>(bc_id, step_tag, uvec(node_tag), 2));
    else if(bc_type == '3')
        domain->insert(make_shared<BC>(bc_id, step_tag, uvec(node_tag), 3));
    else if(bc_type == '4')
        domain->insert(make_shared<BC>(bc_id, step_tag, uvec(node_tag), 4));
    else if(bc_type == '5')
        domain->insert(make_shared<BC>(bc_id, step_tag, uvec(node_tag), 5));
    else if(bc_type == '6')
        domain->insert(make_shared<BC>(bc_id, step_tag, uvec(node_tag), 6));

    return 0;
}

int create_new_cload(const shared_ptr<Bead>& model, istringstream& command) {
    unsigned load_id;
    if((command >> load_id).fail()) {
        suanpan_info("create_new_cload() needs a tag.\n");
        return 0;
    }

    double magnitude;
    if((command >> magnitude).fail()) {
        suanpan_info("create_new_cload() needs load magnitude.\n");
        return 0;
    }

    unsigned dof_id;
    if((command >> dof_id).fail()) {
        suanpan_info("create_new_cload() needs a valid DoF.\n");
        return 0;
    }

    unsigned node;
    vector<uword> node_tag;
    while(get_input(command, node)) node_tag.push_back(node);

    const auto& domain = model->get_current_domain();
    const auto& step_tag = model->get_current_step_tag();

    if(!domain->insert(make_shared<CLoad>(load_id, step_tag, magnitude, uvec(node_tag), dof_id))) suanpan_error("create_new_cload() fails to create new load.\n");

    return 0;
}

int create_new_dispload(const shared_ptr<Bead>& model, istringstream& command) {
    unsigned load_id;
    if((command >> load_id).fail()) {
        suanpan_info("create_new_dispload() needs a tag.\n");
        return 0;
    }

    double magnitude;
    if((command >> magnitude).fail()) {
        suanpan_info("create_new_dispload() needs load magnitude.\n");
        return 0;
    }

    unsigned dof_id;
    if((command >> dof_id).fail()) {
        suanpan_info("create_new_dispload() needs a valid DoF.\n");
        return 0;
    }

    unsigned node;
    vector<uword> node_tag;
    while(get_input(command, node)) node_tag.push_back(node);

    const auto& domain = model->get_current_domain();
    const auto& step_tag = model->get_current_step_tag();

    if(!domain->insert(make_shared<DisplacementLoad>(load_id, step_tag, magnitude, uvec(node_tag), dof_id))) suanpan_error("create_new_dispload() fails to create new load.\n");

    return 0;
}

int set_property(const shared_ptr<Bead>& model, istringstream& command) {
    const auto& tmp_step = get_current_step(model);
    if(tmp_step == nullptr) return 0;

    string property_id;
    if(!get_input(command, property_id)) {
        suanpan_info("set_property() need a property type.\n");
        return 0;
    }

    string value;
    if(if_equal(property_id, "fixed_step_size"))
        if(get_input(command, value))
            tmp_step->set_fixed_step_size(if_true(value));
        else
            suanpan_info("set_property() need a valid value.\n");
    else if(if_equal(property_id, "symm_mat"))
        if(get_input(command, value))
            tmp_step->set_symm(if_true(value));
        else
            suanpan_info("set_property() need a valid value.\n");
    else if(if_equal(property_id, "band_mat"))
        if(get_input(command, value))
            tmp_step->set_band(if_true(value));
        else
            suanpan_info("set_property() need a valid value.\n");
    else if(if_equal(property_id, "ini_step_size")) {
        double step_time;
        if(get_input(command, step_time))
            tmp_step->set_ini_step_size(step_time);
        else
            suanpan_info("set_property() need a valid value.\n");
    } else if(if_equal(property_id, "min_step_size")) {
        double step_time;
        if(get_input(command, step_time))
            tmp_step->set_min_step_size(step_time);
        else
            suanpan_info("set_property() need a valid value.\n");
    } else if(if_equal(property_id, "max_step_size")) {
        double step_time;
        if(get_input(command, step_time))
            tmp_step->set_max_step_size(step_time);
        else
            suanpan_info("set_property() need a valid value.\n");
    } else if(if_equal(property_id, "max_iteration")) {
        unsigned max_number;
        if(get_input(command, max_number))
            tmp_step->set_max_substep(max_number);
        else
            suanpan_info("set_property() need a valid value.\n");
    }

    return 0;
}

int create_new_node(const shared_ptr<Domain>& domain, istringstream& command) {
    unsigned node_id;
    if((command >> node_id).fail()) {
        suanpan_info("create_new_node() needs a tag.\n");
        return 0;
    }

    vector<double> coor;
    double X;
    while(get_input(command, X)) coor.push_back(X);

    if(!domain->insert(make_shared<Node>(node_id, vec(coor)))) suanpan_debug("create_new_node() fails to insert Node %u.\n", node_id);

    return 0;
}

int create_new_material(const shared_ptr<Domain>& domain, istringstream& command) {
    string material_id;
    if((command >> material_id).fail()) {
        suanpan_info("create_new_material() needs a tag.\n");
        return 0;
    }

    unique_ptr<Material> new_material = nullptr;

    if(if_equal(material_id, "Elastic1D"))
        new_elastic1d(new_material, command);
    else if(if_equal(material_id, "Elastic2D"))
        new_elastic2d(new_material, command);
    else if(if_equal(material_id, "Elastic3D"))
        new_elastic3d(new_material, command);
    else if(if_equal(material_id, "Bilinear1D"))
        new_bilinear1d(new_material, command);
    else if(if_equal(material_id, "Bilinear2D"))
        new_bilinear2d(new_material, command);
    else if(if_equal(material_id, "Bilinear3D"))
        new_bilinear3d(new_material, command);
    else if(if_equal(material_id, "MPF"))
        new_mpf(new_material, command);
    else {
        ExternalModule ext_library(material_id);
        if(ext_library.locate_module()) ext_library.new_object(new_material, command);
    }

    if(new_material == nullptr || !domain->insert(move(new_material))) suanpan_debug("create_new_material() fails to insert new material.\n");

    return 0;
}

int create_new_element(const shared_ptr<Domain>& domain, istringstream& command) {
    string element_id;
    if((command >> element_id).fail()) {
        suanpan_info("create_new_element() needs element type.\n");
        return 0;
    }

    unique_ptr<Element> new_element = nullptr;

    if(if_equal(element_id, "CP3"))
        new_cp3(new_element, command);
    else if(if_equal(element_id, "CP4"))
        new_cp4(new_element, command);
    else if(if_equal(element_id, "CP8"))
        new_cp8(new_element, command);
    else if(if_equal(element_id, "C3D8"))
        new_c3d8(new_element, command);
    else if(if_equal(element_id, "C3D20"))
        new_c3d20(new_element, command);
    else if(if_equal(element_id, "PS"))
        new_ps(new_element, command);
    else if(if_equal(element_id, "QE2"))
        new_qe2(new_element, command);
    else if(if_equal(element_id, "GQ12"))
        new_gq12(new_element, command);
    else if(if_equal(element_id, "Truss2D"))
        new_truss2d(new_element, command);
    else if(if_equal(element_id, "ElasticB21"))
        new_elasticb21(new_element, command);
    else if(if_equal(element_id, "Proto01"))
        new_proto01(new_element, command);
    else {
        ExternalModule ext_library(element_id);
        if(ext_library.locate_module()) ext_library.new_object(new_element, command);
    }

    if(new_element == nullptr || !domain->insert(move(new_element))) suanpan_error("create_new_element() fails to create new element.\n");

    return 0;
}

int create_new_recorder(const shared_ptr<Domain>& domain, istringstream& command) {
    unsigned tag;
    if((command >> tag).fail()) {
        suanpan_info("create_new_recorder() needs a valid tag.\n");
        return 0;
    }

    string object_type;
    if((command >> object_type).fail()) {
        suanpan_info("create_new_recorder() needs a valid object type.\n");
        return 0;
    }

    string variable_type;
    if((command >> variable_type).fail()) {
        suanpan_info("create_new_recorder() needs a valid recorder type.\n");
        return 0;
    }

    unsigned object_tag;
    if((command >> object_tag).fail()) {
        suanpan_info("create_new_recorder() needs a valid object tag.\n");
        return 0;
    }

    if(if_equal(object_type, "Node") && !domain->insert(make_shared<NodeRecorder>(tag, object_tag, to_list(variable_type.c_str()), true))) suanpan_info("create_new_recorder() fails to create a new node recorder.\n");

    return 0;
}

int print_info(const shared_ptr<Domain>& domain, istringstream& command) {
    string object_type;
    if((command >> object_type).fail()) {
        suanpan_info("print_info() needs object type.\n");
        return 0;
    }

    unsigned tag;
    if(object_type == "node")
        while(!(command >> tag).fail()) {
            const auto& tmp_node = get_node(domain, tag);
            if(tmp_node != nullptr) tmp_node->print();
            suanpan_info("\n");
        }
    else if(object_type == "element")
        while(!(command >> tag).fail()) {
            const auto& tmp_element = get_element(domain, tag);
            if(tmp_element != nullptr) tmp_element->print();
            suanpan_info("\n");
        }
    else if(object_type == "material")
        while(!(command >> tag).fail()) {
            const auto& tmp_material = get_material(domain, tag);
            if(tmp_material != nullptr) tmp_material->print();
            suanpan_info("\n");
        }
    else if(object_type == "constraint")
        while(!(command >> tag).fail()) {
            const auto& tmp_constraint = get_constraint(domain, tag);
            if(tmp_constraint != nullptr) tmp_constraint->print();
            suanpan_info("\n");
        }
    else if(object_type == "recorder")
        while(!(command >> tag).fail()) {
            const auto& tmp_recorder = get_recorder(domain, tag);
            if(tmp_recorder != nullptr) tmp_recorder->print();
            suanpan_info("\n");
        }

    return 0;
}

int enable_object(const shared_ptr<Bead>& model, istringstream& command) {
    const auto& domain = get_current_domain(model);
    if(domain == nullptr) {
        suanpan_info("enable_object() needs a valid domain.\n");
        return 0;
    }

    string object_type;
    if((command >> object_type).fail()) {
        suanpan_info("enable_object() needs object type.\n");
        return 0;
    }

    unsigned tag;
    if(object_type == "domain")
        while(!(command >> tag).fail()) model->enable_domain(tag);
    else if(object_type == "step")
        while(!(command >> tag).fail()) model->enable_step(tag);
    else if(object_type == "converger")
        while(!(command >> tag).fail()) model->enable_converger(tag);
    else if(object_type == "bc")
        while(!(command >> tag).fail()) domain->enable_constraint(tag);
    else if(object_type == "constraint")
        while(!(command >> tag).fail()) domain->enable_constraint(tag);
    else if(object_type == "element")
        while(!(command >> tag).fail()) domain->enable_element(tag);
    else if(object_type == "load")
        while(!(command >> tag).fail()) domain->enable_load(tag);
    else if(object_type == "material")
        while(!(command >> tag).fail()) domain->enable_material(tag);
    else if(object_type == "node")
        while(!(command >> tag).fail()) domain->enable_node(tag);
    else if(object_type == "recorder")
        while(!(command >> tag).fail()) domain->enable_recorder(tag);

    return 0;
}

int disable_object(const shared_ptr<Bead>& model, istringstream& command) {
    const auto& domain = get_current_domain(model);
    if(domain == nullptr) {
        suanpan_info("disable_object() needs a valid domain.\n");
        return 0;
    }

    string object_type;
    if((command >> object_type).fail()) {
        suanpan_info("disable_object() needs object type.\n");
        return 0;
    }

    unsigned tag;
    if(object_type == "domain")
        while(!(command >> tag).fail()) model->disable_domain(tag);
    else if(object_type == "step")
        while(!(command >> tag).fail()) model->disable_step(tag);
    else if(object_type == "converger")
        while(!(command >> tag).fail()) model->disable_converger(tag);
    else if(object_type == "bc")
        while(!(command >> tag).fail()) domain->disable_constraint(tag);
    else if(object_type == "constraint")
        while(!(command >> tag).fail()) domain->disable_constraint(tag);
    else if(object_type == "element")
        while(!(command >> tag).fail()) domain->disable_element(tag);
    else if(object_type == "load")
        while(!(command >> tag).fail()) domain->disable_load(tag);
    else if(object_type == "material")
        while(!(command >> tag).fail()) domain->disable_material(tag);
    else if(object_type == "node")
        while(!(command >> tag).fail()) domain->disable_node(tag);
    else if(object_type == "recorder")
        while(!(command >> tag).fail()) domain->disable_recorder(tag);

    return 0;
}

int erase_object(const shared_ptr<Bead>& model, istringstream& command) {
    const auto& domain = get_current_domain(model);
    if(domain == nullptr) {
        suanpan_info("erase_object() needs a valid domain.\n");
        return 0;
    }

    string object_type;
    if((command >> object_type).fail()) {
        suanpan_info("erase_object() needs object type.\n");
        return 0;
    }

    unsigned tag;
    if(object_type == "domain")
        while(!(command >> tag).fail()) model->erase_domain(tag);
    else if(object_type == "step")
        while(!(command >> tag).fail()) model->erase_step(tag);
    else if(object_type == "converger")
        while(!(command >> tag).fail()) model->erase_converger(tag);
    else if(object_type == "bc")
        while(!(command >> tag).fail()) domain->erase_constraint(tag);
    else if(object_type == "constraint")
        while(!(command >> tag).fail()) domain->erase_constraint(tag);
    else if(object_type == "element")
        while(!(command >> tag).fail()) domain->erase_element(tag);
    else if(object_type == "load")
        while(!(command >> tag).fail()) domain->erase_load(tag);
    else if(object_type == "material")
        while(!(command >> tag).fail()) domain->erase_material(tag);
    else if(object_type == "node")
        while(!(command >> tag).fail()) domain->erase_node(tag);
    else if(object_type == "recorder")
        while(!(command >> tag).fail()) domain->erase_recorder(tag);

    return 0;
}

void print_command_usage(istringstream& command) {
    string command_id;
    command >> command_id;

    if(if_equal(command_id, "converger")) {
        suanpan_info("\nconverger $type $tag $tolerance [$max_iteration] [$if_print]\n");
        suanpan_info("\t$type --- converger type\n");
        suanpan_info("\t$tag --- converger tag\n");
        suanpan_info("\t$tolerance --- tolerance -> 1E-8\n");
        suanpan_info("\t$max_iteration --- maximum iteration number -> 7\n");
        suanpan_info("\t$if_print --- print error in each iteration -> false\n\n");
    } else if(if_equal(command_id, "step")) {
        suanpan_info("\nstep $type $tag [$time_period]\n");
        suanpan_info("\t$type --- step type\n");
        suanpan_info("\t$tag --- step tag\n");
        suanpan_info("\t$time_period --- step time period -> 1.0\n\n");
    } else if(if_equal(command_id, "Truss2D")) {
        suanpan_info("\nelement Truss2D $tag {$node_tag...} $material_tag $area [$nonlinear_switch] [$constant_area_switch] [$log_strain_switch]\n");
        suanpan_info("\t$tag --- element tag\n");
        suanpan_info("\t$node_tag --- node tag (2)\n");
        suanpan_info("\t$material_tag --- material tag\n");
        suanpan_info("\t$area --- cross section area\n");
        suanpan_info("\t$nonlinear_switch --- if to use corotational formulation -> false\n");
        suanpan_info("\t$constant_area_switch --- if to update area based on constant volume assumption -> false\n");
        suanpan_info("\t$log_strain_switch --- if to use log strain or engineering strain -> false\n\n");
    } else if(if_equal(command_id, "Elastic1D")) {
        suanpan_info("\nmaterial Elastic1D $tag $elastic_modulus [$density]\n");
        suanpan_info("\t$tag --- material tag\n");
        suanpan_info("\t$elastic_modulus --- elastic modulus\n");
        suanpan_info("\t$density --- density -> 0.0\n\n");
    } else if(if_equal(command_id, "Bilinear1D")) {
        suanpan_info("\nmaterial Bilinear1D $tag $elastic_modulus $yield_stress [$hardening_ratio] [$beta] [$density]\n");
        suanpan_info("\t$tag --- material tag\n");
        suanpan_info("\t$elastic_modulus --- elastic modulus\n");
        suanpan_info("\t$yield_stress --- yield stress\n");
        suanpan_info("\t$hardening_ratio --- hardening ratio -> 0.0\n");
        suanpan_info("\t$beta --- mixed hardening 0.0 for isotropic hardening 1.0 for kinematic hardening -> 0.0\n");
        suanpan_info("\t$density --- density -> 0.0\n\n");
    }
}
