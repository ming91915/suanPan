#include "commandParser.h"
#include "elementParser.h"
#include <Constraint/BC/BC.h>
#include <Domain/Domain.h>
#include <Domain/Node.h>
#include <Element/Element.h>
#include <Load/CLoad.h>

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
            if(command_id == "node") create_new_node(domain, tmp_str);
            if(command_id == "element") create_new_element(domain, tmp_str);
            if(command_id == "fix") create_new_bc(domain, tmp_str);
            if(command_id == "cload") create_new_cload(domain, tmp_str);
            if(command_id == "element") create_new_element(domain, tmp_str);
            if(command_id == "element") create_new_element(domain, tmp_str);
            if(command_id == "element") create_new_element(domain, tmp_str);
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

void create_new_element(const shared_ptr<Domain>& domain, istringstream& command)
{
    string element_id;
    command >> element_id;
    if(_strcmpi(element_id.c_str(), "CP4") == 0) {
        unique_ptr<Element> new_elemnt;
        new_cp4_(new_elemnt, command);
        domain->insert(move(new_elemnt));
    }
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
    if(dof_id == 1)
        domain->insert(make_shared<CLoad>(load_id, 0, magnitude, uvec(node_tag), 1));
    else if(dof_id == 2)
        domain->insert(make_shared<CLoad>(load_id, 0, magnitude, uvec(node_tag), 2));
    else if(dof_id == 3)
        domain->insert(make_shared<CLoad>(load_id, 0, magnitude, uvec(node_tag), 3));
    else if(dof_id == 4)
        domain->insert(make_shared<CLoad>(load_id, 0, magnitude, uvec(node_tag), 4));
    else if(dof_id == 5)
        domain->insert(make_shared<CLoad>(load_id, 0, magnitude, uvec(node_tag), 5));
    else if(dof_id == 6)
        domain->insert(make_shared<CLoad>(load_id, 0, magnitude, uvec(node_tag), 6));
}
