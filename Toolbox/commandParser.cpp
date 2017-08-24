#include "commandParser.h"
#include "Recorder/NodeRecorder.h"
#include "argumentParser.h"
#include <Constraint/BC/BC.h>
#include <Convergence/Convergence>
#include <Domain/Domain.h>
#include <Domain/ExternalModule.h>
#include <Domain/Node.h>
#include <Element/Element.h>
#include <Element/ElementParser.h>
#include <Load/CLoad.h>
#include <Material/MaterialParser.h>
#include <Solver/Newton.h>
#include <Step/Bead.h>
#include <Step/Dynamic.h>
#include <Step/Static.h>

using std::string;
using std::ifstream;
using std::vector;

int process_command(const shared_ptr<Bead>& model, istringstream& command)
{
    if(model == nullptr) return 0;

    string command_id;
    if((command >> command_id).fail()) return 0;

    if(command_id == "exit") return SUANPAN_EXIT;

    if(command_id == "quit") return SUANPAN_EXIT;

    if(command_id == "file") return process_file(model, command);

    if(command_id == "domain") return create_new_domain(model, command);

    if(command_id == "converger") return create_new_converger(model, command);

    if(command_id == "step") return create_new_step(model, command);

    if(command_id == "solver") return create_new_solver(model, command);

    if(command_id == "fix") return create_new_bc(model, command);

    if(command_id == "cload") return create_new_cload(model, command);

    if(command_id == "disable") return disable_object(model, command);

    if(command_id == "mute") return disable_object(model, command);

    if(command_id == "erase") return erase_object(model, command);

    if(command_id == "delete") return erase_object(model, command);

    if(command_id == "remove") return erase_object(model, command);

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

int process_file(const shared_ptr<Bead>& model, const char* file_name)
{
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

int process_file(const shared_ptr<Bead>& model, istringstream& command)
{
    string file_name;
    if((command >> file_name).fail()) {
        suanpan_info("process_file() needs a file name.\n");
        return 0;
    }

    return process_file(model, file_name.c_str());
}

int create_new_domain(const shared_ptr<Bead>& model, istringstream& command)
{
    unsigned domain_id;
    if((command >> domain_id).fail()) {
        suanpan_info("create_new_domain() requires a tag.\n");
        return 0;
    }

    model->set_current_domain(domain_id);

    auto& tmp_domain = get_domain(model, domain_id);

    if(tmp_domain == nullptr) {
        tmp_domain = make_shared<Domain>(domain_id);
        if(tmp_domain != nullptr)
            suanpan_info(
                "create_new_domain() successfully creates Domain %u.\n", domain_id);
    } else
        suanpan_info("create_new_domain() switches to Domain %u.\n", domain_id);

    return 0;
}

int create_new_step(const shared_ptr<Bead>& model, istringstream& command)
{
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

    auto time = 1.;
    if(!command.eof() && (command >> time).fail()) {
        suanpan_info("create_new_step() reads a wrong time period.\n");
        return 0;
    }

    if(step_type == "static") {
        if(!model->insert(make_shared<Static>(tag, nullptr, nullptr, nullptr, time)))
            suanpan_error("create_new_step() cannot create the new step.\n");
    } else if(step_type == "dynamic") {
        if(!model->insert(make_shared<Dynamic>(tag)))
            suanpan_error("create_new_step() cannot create the new step.\n");
    } else
        suanpan_info("create_new_step() cannot identify step type.\n");

    model->set_current_step(tag);

    model->get_current_step()->set_domain(model->get_current_domain());

    return 0;
}

int create_new_converger(const shared_ptr<Bead>& model, istringstream& command)
{
    const auto& tmp_step = get_current_step(model);
    if(tmp_step == nullptr) {
        suanpan_info("create_new_converger() needs a valid step.\n");
        return 0;
    }

    string converger_id;
    if((command >> converger_id).fail()) {
        suanpan_info("create_new_converger() requires convergence type.\n");
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

    auto max_iteration = 7;
    if(!command.eof() && (command >> max_iteration).fail()) {
        suanpan_info("create_new_converger() reads wrong max iteration.\n");
        return 0;
    }

    auto print_flag = 0;
    if(!command.eof() && (command >> print_flag).fail()) {
        suanpan_info("create_new_converger() reads wrong print flag.\n");
        return 0;
    }

    if(_strcmpi(converger_id.c_str(), "AbsResidual") == 0) {
        if(!model->insert(make_shared<AbsResidual>(
               tag, nullptr, tolerance, max_iteration, !!print_flag)))
            suanpan_info("create_new_converger() fails to create the new converger.\n");
    } else if(_strcmpi(converger_id.c_str(), "AbsIncreDisp") == 0) {
        if(!model->insert(make_shared<AbsIncreDisp>(
               tag, nullptr, tolerance, max_iteration, !!print_flag)))
            suanpan_info("create_new_converger() fails to create the new converger.\n");
    } else
        suanpan_info("create_new_converger() cannot identify the converger type.\n");

    tmp_step->set_convergence(model->get_convergence(tag));

    return 0;
}

int create_new_solver(const shared_ptr<Bead>& model, istringstream& command)
{
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

    if(_strcmpi(solver_type.c_str(), "Newton") == 0) {
        if(!model->insert(make_shared<Newton>(tag)))
            suanpan_error("create_new_solver() cannot create the new solver.\n");
    } else if(_strcmpi(solver_type.c_str(), "BFGS") == 0) {
        if(!model->insert(make_shared<Newton>(tag)))
            suanpan_error("create_new_solver() cannot create the new solver.\n");
    } else
        suanpan_error("create_new_solver() cannot identify solver type.\n");

    tmp_step->set_solver(model->get_solver(tag));

    return 0;
}

int create_new_bc(const shared_ptr<Bead>& model, istringstream& command)
{
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
    const auto& step_tag = model->get_current_step()->get_tag();

    auto bc_type = tolower(dof_id[0]);
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

int create_new_cload(const shared_ptr<Bead>& model, istringstream& command)
{
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
    while(!(command >> node).fail()) node_tag.push_back(node);

    const auto& domain = model->get_current_domain();
    const auto& step_tag = model->get_current_step()->get_tag();

    if(!domain->insert(
           make_shared<CLoad>(load_id, step_tag, magnitude, uvec(node_tag), dof_id)))
        suanpan_error("create_new_cload() fails to create new load.\n");

    return 0;
}

int create_new_node(const shared_ptr<Domain>& domain, istringstream& command)
{
    unsigned node_id;
    if((command >> node_id).fail()) {
        suanpan_info("create_new_node() needs a tag.\n");
        return 0;
    }

    vector<double> coor;
    double X;
    while(!(command >> X).fail()) coor.push_back(X);

    if(!domain->insert(make_shared<Node>(node_id, vec(coor))))
        suanpan_debug("create_new_node() fails to insert Node %u.\n", node_id);

    return 0;
}

int create_new_material(const shared_ptr<Domain>& domain, istringstream& command)
{
    string material_id;
    if((command >> material_id).fail()) {
        suanpan_info("create_new_material() needs a tag.\n");
        return 0;
    }

    unique_ptr<Material> new_material = nullptr;

    if(_strcmpi(material_id.c_str(), "Elastic2D") == 0)
        new_elastic2d_(new_material, command);
    else {
        ExternalModule ext_library(material_id);
        if(ext_library.locate_module()) ext_library.new_object(new_material, command);
    }

    if(new_material == nullptr || !domain->insert(move(new_material)))
        suanpan_debug("create_new_material() fails to insert new material.\n");

    return 0;
}

int create_new_element(const shared_ptr<Domain>& domain, istringstream& command)
{
    string element_id;
    if((command >> element_id).fail()) {
        suanpan_info("create_new_element() needs element type.\n");
        return 0;
    }

    unique_ptr<Element> new_element = nullptr;

    if(_strcmpi(element_id.c_str(), "CP4") == 0)
        new_cp4_(new_element, command);
    else if(_strcmpi(element_id.c_str(), "PS") == 0)
        new_ps_(new_element, command);
    else if(_strcmpi(element_id.c_str(), "QE2") == 0)
        new_qe2_(new_element, command);
    else if(_strcmpi(element_id.c_str(), "GQ12") == 0)
        new_gq12_(new_element, command);
    else {
        ExternalModule ext_library(element_id);
        if(ext_library.locate_module()) ext_library.new_object(new_element, command);
    }

    if(new_element == nullptr || !domain->insert(move(new_element)))
        suanpan_error("create_new_element() fails to create new element.\n");

    return 0;
}

int create_new_recorder(const shared_ptr<Domain>& domain, istringstream& command)
{
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

    if(_strcmpi(object_type.c_str(), "Node") == 0 &&
        !domain->insert(make_shared<NodeRecorder>(
            tag, object_tag, to_list(variable_type.c_str()), true)))
        suanpan_info("create_new_recorder() fails to create a new node recorder.\n");

    return 0;
}

int print_info(const shared_ptr<Domain>& domain, istringstream& command)
{
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

int disable_object(const shared_ptr<Bead>& model, istringstream& command)
{
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
        while(!(command >> tag).fail()) model->disable_convergence(tag);
    else if(object_type == "bc" || object_type == "constraint")
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

int erase_object(const shared_ptr<Bead>& model, istringstream& command)
{
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
        while(!(command >> tag).fail()) model->erase_convergence(tag);
    else if(object_type == "bc" || object_type == "constraint")
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

void print_command_usage(istringstream& command)
{
    string command_id;
    command >> command_id;

    if(_strcmpi(command_id.c_str(), "converger") == 0) {
        suanpan_info("\nconverger $type $tag $tolerance [$max_iteration] [$if_print]\n");
        suanpan_info("\t$type --- converger type\n");
        suanpan_info("\t$tag --- converger tag\n");
        suanpan_info("\t$tolerance --- tolerance -> 1E-8\n");
        suanpan_info("\t$max_iteration --- maximum iteration number -> 7\n");
        suanpan_info("\t$if_print --- print error in each iteration -> false\n\n");
    } else if(_strcmpi(command_id.c_str(), "step") == 0) {
        suanpan_info("\nstep $type $tag [$time_period]\n");
        suanpan_info("\t$type --- step type\n");
        suanpan_info("\t$tag --- step tag\n");
        suanpan_info("\t$time_period --- step time period -> 1.0\n\n");
    }
}
