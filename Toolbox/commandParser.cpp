#include "commandParser.h"

void create_new_element(const shared_ptr<Domain>& D, const char* C)
{
    ifstream input_file(C);

    if(!input_file.is_open()) return;

    string command_line;
}
