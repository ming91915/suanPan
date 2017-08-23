#include "commandParser.h"
#include "argumentParser.h"
#include "elementParser.h"
#include "materialParser.h"
#include <Constraint/BC/BC.h>
#include <Convergence/Convergence>
#include <Domain/Domain.h>
#include <Domain/ExternalModule.h>
#include <Domain/Node.h>
#include <Element/Element.h>
#include <Load/CLoad.h>
#include <Solver/Newton.h>
#include <Step/Bead.h>
#include <Step/Dynamic.h>
#include <Step/Static.h>

using std::string;
using std::ifstream;
using std::vector;

int process_command(const shared_ptr<Bead>& model, istringstream& command)
{
    string command_id;
    if((command >> command_id).fail()) return 0;

    if(command_id == "exit" || command_id == "quit") return SUANPAN_EXIT;

    if(command_id == "file" && process_file(model, command) == SUANPAN_EXIT)
        return SUANPAN_EXIT;

    auto& domain = getCurrentDomain(model);

    if(command_id == "domain") // create new domain
        create_new_domain(model, command);
    else if(command_id == "converger") // create new convergence
        create_new_converger(model, command);
    else if(command_id == "step") // create new step
        create_new_step(model, command);
    else if(command_id == "recorder") // create new recorder
        create_new_recorder(model, command);
    else if(command_id == "disable" || command_id == "mute") // disable any object
        disable_object(model, command);
    else if(command_id == "erase" || command_id == "delete" ||
        command_id == "remove") // delete any obejct
        erase_object(model, command);
    else if(command_id == "node") // create new node
        create_new_node(domain, command);
    else if(command_id == "material") // create new material model
        create_new_material(domain, command);
    else if(command_id == "element") // create new element
        create_new_element(domain, command);
    else if(command_id == "fix") // assign homogeneous boundary condition
        create_new_bc(domain, command);
    else if(command_id == "cload") // point load
        create_new_cload(domain, command);
    else if(command_id == "version") // print version information
        print_version();
    else if(command_id == "help") // print helper
        print_helper();
    else if(command_id == "print") // print object status
        print_info(domain, command);
    else if(command_id == "analyze") // run analysis
        model->analyze();
    else if(command_id == "clear") // clear current domain status
        domain->clear_status();
    else if(command_id == "summary") { // print current domain summary
        if(domain != nullptr) domain->summary();
    }

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
    while(input_file.good()) {
        getline(input_file, command_line);
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

void create_new_domain(const shared_ptr<Bead>& model, istringstream& command)
{
    unsigned domain_id;
    if((command >> domain_id).fail()) {
        suanpan_info("create_new_domain() requires a tag.\n");
        return;
    }

    model->setCurrentDomain(domain_id);

    auto& tmp_domain = getDomain(model, domain_id);

    if(tmp_domain == nullptr) {
        tmp_domain = make_shared<Domain>(domain_id);
        if(tmp_domain != nullptr)
            suanpan_info(
                "create_new_domain() successfully creates Domain %u.\n", domain_id);
    } else
        suanpan_info("create_new_domain() switches to Domain %u.\n", domain_id);
}

void create_new_converger(const shared_ptr<Bead>& model, istringstream& command)
{
    const auto& tmp_domain = getCurrentDomain(model);
    if(tmp_domain == nullptr) {
        suanpan_info("create_new_converger() needs a valid domain.\n");
        return;
    }

    string converger_id;
    if((command >> converger_id).fail()) {
        suanpan_info("create_new_converger() requires convergence type.\n");
        return;
    }

    unsigned tag;
    if((command >> tag).fail()) {
        suanpan_info("create_new_converger() requires a tag.\n");
        return;
    }

    auto tolerance = 1E-8;
    if(!command.eof() && (command >> tolerance).fail()) {
        suanpan_info("create_new_converger() reads wrong tolerance.\n");
        return;
    }

    auto print_flag = 0;
    if(!command.eof() && (command >> print_flag).fail()) {
        suanpan_info("create_new_converger() reads wrong print flag.\n");
        return;
    }

    if(_strcmpi(converger_id.c_str(), "AbsResidual") == 0) {
        if(!model->insert(
               make_shared<AbsResidual>(tag, tmp_domain, tolerance, !!print_flag)))
            suanpan_info("create_new_converger() fails to create the new converger.\n");
    } else if(_strcmpi(converger_id.c_str(), "AbsIncreDisp") == 0) {
        if(!model->insert(
               make_shared<AbsIncreDisp>(tag, tmp_domain, tolerance, !!print_flag)))
            suanpan_info("create_new_converger() fails to create the new converger.\n");
    } else
        suanpan_info("create_new_converger() cannot identify the converger type.\n");
}

void create_new_step(const shared_ptr<Bead>& model, istringstream& command)
{
    const auto& tmp_domain = getCurrentDomain(model);
    if(tmp_domain == nullptr) {
        suanpan_info("create_new_step() needs a valid domain.\n");
        return;
    }

    string step_type;
    if((command >> step_type).fail()) {
        suanpan_info("create_new_step() requires step type.\n");
        return;
    }

    string solver_type;
    if((command >> solver_type).fail()) {
        suanpan_info("create_new_step() requires solver type.\n");
        return;
    }

    unsigned tag;
    if((command >> tag).fail()) {
        suanpan_info("create_new_step() requires a tag.\n");
        return;
    }

    unsigned converger_tag;
    if((command >> converger_tag).fail()) {
        suanpan_info("create_new_step() requires a converger tag.\n");
        return;
    }

    const auto& tmp_converger = getConvergence(model, converger_tag);
    if(tmp_converger == nullptr) {
        suanpan_error(
            "create_new_step() cannot find Converger %u, please define it first.\n",
            converger_tag);
        return;
    }

    auto time = 1.;
    if(!command.eof() && (command >> time).fail()) {
        suanpan_info("create_new_step() reads a wrong time period.\n");
        return;
    }

    if(step_type == "static") {
        if(_strcmpi(solver_type.c_str(), "Newton") == 0 &&
            !model->insert(make_shared<Static>(
                tag, make_shared<Newton>(tag, tmp_domain, tmp_converger), time)))
            suanpan_error("create_new_step() cannot create the new step.\n");
    } else if(step_type == "dynamic")
        model->insert(make_shared<Dynamic>(tag));
    else
        suanpan_info("create_new_step() cannot identify step type.\n");
}

void create_new_recorder(const shared_ptr<Bead>& model, istringstream& command) {}

void create_new_node(const shared_ptr<Domain>& domain, istringstream& command)
{
    unsigned node_id;
    if((command >> node_id).fail()) {
        suanpan_info("create_new_node() needs a tag.\n");
        return;
    }

    vector<double> coor;
    double X;
    while(!(command >> X).fail()) coor.push_back(X);

    if(!domain->insert(make_shared<Node>(node_id, vec(coor))))
        suanpan_debug("create_new_node() fails to insert Node %u.\n", node_id);
}

void create_new_material(const shared_ptr<Domain>& domain, istringstream& command)
{
    string material_id;
    if((command >> material_id).fail()) {
        suanpan_info("create_new_material() needs a tag.\n");
        return;
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
}

void create_new_element(const shared_ptr<Domain>& domain, istringstream& command)
{
    string element_id;
    if((command >> element_id).fail()) {
        suanpan_info("create_new_element() needs element type.\n");
        return;
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
}

void create_new_bc(const shared_ptr<Domain>& domain, istringstream& command)
{
    unsigned bc_id;
    if((command >> bc_id).fail()) {
        suanpan_info("create_new_bc() needs BC type.\n");
        return;
    }

    string dof_id;
    if((command >> dof_id).fail()) {
        suanpan_info("create_new_bc() needs valid DoFs.\n");
        return;
    }

    unsigned X;
    vector<uword> node_tag;
    while(!(command >> X).fail()) node_tag.push_back(X);

    auto bc_type = tolower(dof_id[0]);
    if(bc_type == 'p')
        domain->insert(make_shared<BC>(bc_id, 0, uvec(node_tag), "PINNED"));
    else if(bc_type == 'e')
        domain->insert(make_shared<BC>(bc_id, 0, uvec(node_tag), "ENCASTRE"));
    else if(bc_type == 'x')
        domain->insert(make_shared<BC>(bc_id, 0, uvec(node_tag), "XSYMM"));
    else if(bc_type == 'y')
        domain->insert(make_shared<BC>(bc_id, 0, uvec(node_tag), "YSYMM"));
    else if(bc_type == 'z')
        domain->insert(make_shared<BC>(bc_id, 0, uvec(node_tag), "ZSYMM"));
    else if(bc_type == '1')
        domain->insert(make_shared<BC>(bc_id, 0, uvec(node_tag), 1));
    else if(bc_type == '2')
        domain->insert(make_shared<BC>(bc_id, 0, uvec(node_tag), 2));
    else if(bc_type == '3')
        domain->insert(make_shared<BC>(bc_id, 0, uvec(node_tag), 3));
    else if(bc_type == '4')
        domain->insert(make_shared<BC>(bc_id, 0, uvec(node_tag), 4));
    else if(bc_type == '5')
        domain->insert(make_shared<BC>(bc_id, 0, uvec(node_tag), 5));
    else if(bc_type == '6')
        domain->insert(make_shared<BC>(bc_id, 0, uvec(node_tag), 6));
}

void create_new_cload(const shared_ptr<Domain>& domain, istringstream& command)
{
    unsigned load_id;
    if((command >> load_id).fail()) {
        suanpan_info("create_new_cload() needs a tag.\n");
        return;
    }

    double magnitude;
    if((command >> magnitude).fail()) {
        suanpan_info("create_new_cload() needs load magnitude.\n");
        return;
    }

    unsigned dof_id;
    if((command >> dof_id).fail()) {
        suanpan_info("create_new_cload() needs a valid DoF.\n");
        return;
    }

    unsigned X;
    vector<uword> node_tag;
    while(!(command >> X).fail()) node_tag.push_back(X);

    if(!domain->insert(make_shared<CLoad>(load_id, 0, magnitude, uvec(node_tag), dof_id)))
        suanpan_error("create_new_cload() fails to create new load.\n");
}

void print_info(const shared_ptr<Domain>& domain, istringstream& command)
{
    string object_type;
    if((command >> object_type).fail()) {
        suanpan_info("print_info() needs object type.\n");
        return;
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
}

void disable_object(const shared_ptr<Bead>& model, istringstream& command)
{
    const auto& domain = getCurrentDomain(model);
    if(domain == nullptr) {
        suanpan_info("disable_object() needs a valid domain.\n");
        return;
    }

    string object_type;
    if((command >> object_type).fail()) {
        suanpan_info("disable_object() needs object type.\n");
        return;
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
}

void erase_object(const shared_ptr<Bead>& model, istringstream& command)
{
    const auto& domain = getCurrentDomain(model);
    if(domain == nullptr) {
        suanpan_info("erase_object() needs a valid domain.\n");
        return;
    }

    string object_type;
    if((command >> object_type).fail()) {
        suanpan_info("erase_object() needs object type.\n");
        return;
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
}
