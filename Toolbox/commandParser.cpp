#include "commandParser.h"
#include "Solver/Newton.h"
#include "elementParser.h"
#include "materialParser.h"
#include <Constraint/BC/BC.h>
#include <Convergence/Convergence>
#include <Domain/Domain.h>
#include <Domain/ExternalModule.h>
#include <Domain/Node.h>
#include <Element/Element.h>
#include <Load/CLoad.h>
#include <Step/Bead.h>
#include <Step/Dynamic.h>
#include <Step/Static.h>

using std::string;
using std::ifstream;
using std::vector;

int process_command(const shared_ptr<Bead>& model, istringstream& command)
{
    string command_id;
    command >> command_id;

    auto& domain = model->getCurrentDomain();

    if(command_id == "file")
        process_file(model, command);
    else if(command_id == "model")
        create_new_domain(model, command);
    else if(command_id == "converger")
        create_new_converger(model, command);
    else if(command_id == "step")
        create_new_step(model, command);
    else if(command_id == "recorder")
        create_new_recorder(model, command);
    else if(command_id == "disable")
        disable_object(model, command);
    else if(command_id == "erase" || command_id == "delete")
        erase_object(model, command);
    else if(command_id == "node")
        create_new_node(domain, command);
    else if(command_id == "material")
        create_new_material(domain, command);
    else if(command_id == "element")
        create_new_element(domain, command);
    else if(command_id == "fix")
        create_new_bc(domain, command);
    else if(command_id == "cload")
        create_new_cload(domain, command);
    else if(command_id == "print")
        print_info(domain, command);
    else if(command_id == "analyze")
        model->analyze();
    else if(command_id == "clear")
        domain->clearStatus();
    else if(command_id == "summary") {
        if(domain != nullptr) domain->summary();
    } else if(command_id == "exit" || command_id == "quit")
        return 1;

    return 0;
}

void process_file(const shared_ptr<Bead>& model, const char* file_name)
{
    ifstream input_file(file_name);

    if(!input_file.is_open()) return;

    string command_line;
    while(input_file.good()) {
        getline(input_file, command_line);
        if(!command_line.empty()) {
            istringstream tmp_str(command_line);
            if(process_command(model, tmp_str) == 1) return;
        }
    }
}

void process_file(const shared_ptr<Bead>& model, istringstream& command)
{
    string file_name;
    command >> file_name;
    process_file(model, file_name.c_str());
}

void create_new_domain(const shared_ptr<Bead>& model, istringstream& command)
{
    unsigned model_id;
    command >> model_id;

    auto& tmp_domain = model->getDomain(model_id);

    if(tmp_domain == nullptr)
        tmp_domain = make_shared<Domain>(model_id);
    else
        suanpan_info("create_new_model() switches to an existing model.\n");

    model->setCurrentDomain(model_id);
}

void create_new_converger(const shared_ptr<Bead>& model, istringstream& command)
{
    const auto& tmp_domain = model->getCurrentDomain();

    string converger_id;
    command >> converger_id;

    unsigned tag;
    command >> tag;

    auto tolerance = 1E-8;
    if(command.good()) command >> tolerance;

    auto print_flag = 0;
    if(command.good()) command >> print_flag;

    if(_strcmpi(converger_id.c_str(), "AbsResidual") == 0)
        model->insert(make_shared<AbsResidual>(tag, tmp_domain, tolerance, !!print_flag));
    else if(_strcmpi(converger_id.c_str(), "AbsIncreDisp") == 0)
        model->insert(
            make_shared<AbsIncreDisp>(tag, tmp_domain, tolerance, !!print_flag));
}

void create_new_step(const shared_ptr<Bead>& model, istringstream& command)
{
    const auto& tmp_domain = model->getCurrentDomain();

    string step_type;
    command >> step_type;

    string solver_type;
    command >> solver_type;

    unsigned tag;
    command >> tag;

    unsigned converger_tag;
    command >> converger_tag;
    const auto& tmp_converger = model->getConvergence(converger_tag);
    if(tmp_converger == nullptr) {
        suanpan_error(
            "create_new_step() cannot find a the converger, please define it first.\n");
        return;
    }

    auto time = 1.;
    if(command.good()) command >> time;

    if(step_type == "static") {
        if(_strcmpi(solver_type.c_str(), "Newton") == 0) {
            model->insert(make_shared<Static>(
                tag, make_shared<Newton>(tag, tmp_domain, tmp_converger), time));
        }
    } else if(step_type == "dynamic")
        model->insert(make_shared<Dynamic>(tag));
}

void create_new_recorder(const shared_ptr<Bead>& model, istringstream& command) {}

void create_new_node(const shared_ptr<Domain>& domain, istringstream& command)
{
    unsigned node_id;
    command >> node_id;
    vector<double> coor;
    double X;
    while(command.good()) {
        command >> X;
        coor.push_back(X);
    }
    if(!domain->insert(make_shared<Node>(node_id, vec(coor))))
        suanpan_debug("create_new_node() fails to insert new node.\n");
}

void create_new_material(const shared_ptr<Domain>& domain, istringstream& command)
{
    string material_id;
    command >> material_id;
    unique_ptr<Material> new_material = nullptr;

    if(_strcmpi(material_id.c_str(), "Elastic2D") == 0)
        new_elastic2d_(new_material, command);
    else {
        ExternalModule ext_library(material_id);
        if(ext_library.locate_module()) ext_library.new_object(new_material, command);
    }

    if(new_material == nullptr)
        suanpan_debug("create_new_material() fails to insert new material.\n");
    else if(!domain->insert(move(new_material)))
        suanpan_debug("create_new_material() fails to insert new material.\n");
}

void create_new_element(const shared_ptr<Domain>& domain, istringstream& command)
{
    string element_id;
    command >> element_id;
    unique_ptr<Element> new_element = nullptr;

    if(_strcmpi(element_id.c_str(), "CP4") == 0)
        new_cp4_(new_element, command);
    else if(_strcmpi(element_id.c_str(), "PS") == 0)
        new_ps_(new_element, command);
    else if(_strcmpi(element_id.c_str(), "QE2") == 0)
        new_qe2_(new_element, command);
    else {
        ExternalModule ext_library(element_id);
        if(ext_library.locate_module()) ext_library.new_object(new_element, command);
    }

    if(new_element != nullptr)
        domain->insert(move(new_element));
    else
        suanpan_error("create_new_element() fails to create new element.\n");
}

void create_new_bc(const shared_ptr<Domain>& domain, istringstream& command)
{
    unsigned bc_id;
    command >> bc_id;
    string dof_id;
    command >> dof_id;
    auto bc_type = tolower(dof_id[0]);
    unsigned X;
    vector<uword> node_tag;
    while(command.good()) {
        command >> X;
        node_tag.push_back(X);
    }
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
    command >> load_id;
    double magnitude;
    command >> magnitude;
    unsigned dof_id;
    command >> dof_id;
    unsigned X;
    vector<uword> node_tag;
    while(command.good()) {
        command >> X;
        node_tag.push_back(X);
    }
    domain->insert(make_shared<CLoad>(load_id, 0, magnitude, uvec(node_tag), dof_id));
}

void print_info(const shared_ptr<Domain>& domain, istringstream& command)
{
    string object_id;
    command >> object_id;

    unsigned tag;
    if(object_id == "node")
        while(command.good()) {
            command >> tag;
            domain->getNode(tag)->print();
            suanpan_info("\n");
        }
    else if(object_id == "element")
        while(command.good()) {
            command >> tag;
            domain->getElement(tag)->print();
            suanpan_info("\n");
        }
}

void disable_object(const shared_ptr<Bead>& model, istringstream& command)
{
    auto& domain = model->getCurrentDomain();

    string object_type;
    command >> object_type;
    unsigned tag;
    if(object_type == "node") {
        while(command.good()) {
            command >> tag;
            domain->disable_node(tag);
        }
    } else if(object_type == "element") {
        while(command.good()) {
            command >> tag;
            domain->disable_element(tag);
        }
    } else if(object_type == "load") {
        while(command.good()) {
            command >> tag;
            domain->disable_load(tag);
        }
    } else if(object_type == "bc" || object_type == "constraint") {
        while(command.good()) {
            command >> tag;
            domain->disable_constraint(tag);
        }
    }
}

void erase_object(const shared_ptr<Bead>& model, istringstream& command)
{
    auto& domain = model->getCurrentDomain();

    string object_type;
    command >> object_type;
    unsigned tag;
    if(object_type == "domain") {
        while(command.good()) {
            command >> tag;
            model->erase_domain(tag);
        }
    } else if(object_type == "node") {
        while(command.good()) {
            command >> tag;
            domain->erase_node(tag);
        }
    } else if(object_type == "element") {
        while(command.good()) {
            command >> tag;
            domain->erase_element(tag);
        }
    } else if(object_type == "load") {
        while(command.good()) {
            command >> tag;
            domain->erase_load(tag);
        }
    } else if(object_type == "bc" || object_type == "constraint") {
        while(command.good()) {
            command >> tag;
            domain->erase_constraint(tag);
        }
    }
}
