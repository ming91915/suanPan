#include "commandParser.h"
#include "elementParser.h"
#include "materialParser.h"
#include <Constraint/BC/BC.h>
#include <Domain/Domain.h>
#include <Domain/ExternalModule.h>
#include <Domain/Node.h>
#include <Element/Element.h>
#include <Load/CLoad.h>
#include <Step/Dynamic.h>
#include <Step/Static.h>

void process_file(const shared_ptr<Domain>& domain, const char* file_name)
{
    ifstream input_file(file_name);

    if(!input_file.is_open()) return;

    string command_line;
    string command_id;
    while(input_file.good()) {
        getline(input_file, command_line);
        if(!command_line.empty()) {
            istringstream tmp_str(command_line);
            tmp_str >> command_id;
            if(command_id == "node")
                create_new_node(domain, tmp_str);
            else if(command_id == "material")
                create_new_material(domain, tmp_str);
            else if(command_id == "element")
                create_new_element(domain, tmp_str);
            else if(command_id == "fix")
                create_new_bc(domain, tmp_str);
            else if(command_id == "cload")
                create_new_cload(domain, tmp_str);
            else if(command_id == "step")
                create_new_step(domain, tmp_str);
            else if(command_id == "disable")
                disable_object(domain, tmp_str);
        }
    }
}

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
    domain->insert(make_shared<Node>(node_id, vec(coor)));
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

    if(new_material != nullptr)
        domain->insert(move(new_material));
    else
        suanpan_error("create_new_element() fails to create new element.\n");
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

void create_new_step(const shared_ptr<Domain>& domain, istringstream& command)
{
    string step_type;
    command >> step_type;
    unsigned tag;
    command >> tag;
    auto time = 1.;
    if(command.good()) command >> time;

    //! TODO: SEPERATE STEP AND DOMAIN
    if(step_type == "static")
        auto S = make_shared<Static>(tag, nullptr, time);
    else if(step_type == "dynamic")
        auto S = make_shared<Dynamic>(tag);
}

void disable_object(const shared_ptr<Domain>& domain, istringstream& command)
{
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
