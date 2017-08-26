#include "argumentParser.h"
#include "commandParser.h"
#include <Step/Bead.h>
#include <suanPan.h>

using std::string;
using std::ofstream;

void argument_parser(int argc, char** argv)
{
    string input_file_name = "";
    string output_file_name = "";
    ofstream output_file;
    const auto buffer_backup = cout.rdbuf();

    print_header();

    if(argc > 1) {
        for(auto i = 0; i < argc; ++i) {
            if(_strcmpi(argv[i], "-v") == 0 || _strcmpi(argv[i], "--version") == 0)
                print_version();
            else if(_strcmpi(argv[i], "-h") == 0 || _strcmpi(argv[i], "--help") == 0)
                print_helper();
            else if(_strcmpi(argv[i], "-f") == 0 || _strcmpi(argv[i], "--file") == 0)
                input_file_name = argv[++i];
            else if(_strcmpi(argv[i], "-o") == 0 || _strcmpi(argv[i], "--output") == 0)
                output_file_name = argv[++i];
        }

        if(output_file_name != "") {
            output_file.open(output_file_name);
            if(output_file.is_open())
                cout.rdbuf(output_file.rdbuf());
            else
                suanpan_error("argumentParser() cannot open the output file.\n");
        }

        if(input_file_name != "") {
            const auto model = make_shared<Bead>();
            if(process_file(model, input_file_name.c_str()) == SUANPAN_EXIT) return;
            cout.rdbuf(buffer_backup);
            cli_mode(model);
        }
    } else {
        const auto model = make_shared<Bead>();
        cli_mode(model);
    }
}

void print_header()
{
    suanpan_info("+------------------------------------------------------------+\n");
    suanpan_info("|   __        __            |  suanPan is an open source     |\n");
    suanpan_info("|  /  \\      |  \\           |        FEM framework (%u-bit)  |\n",
        SUANPAN_ARCH);
    suanpan_info("|  \\__       |__/ __   __   |                 Acrux (0.1.0)  |\n");
    suanpan_info("|     \\ |  | |   |  \\ |  |  |                                |\n");
    suanpan_info("|  \\__/ |__| |   |__X |  |  |             maintained by tlc  |\n");
    suanpan_info("|                           |           all rights reserved  |\n");
    suanpan_info("+------------------------------------------------------------+\n\n");
}

void print_version()
{
#ifdef __VERSION__
    string version = SUANPAN_VERSION;
#elif defined(_MSC_FULL_VER)
    auto version = std::to_string(SUANPAN_VERSION);
#endif
    suanpan_info("suanPan is an open source FEM framework.\n");
    suanpan_info("\tversion Acrux 0.1.0, date 23/08/2017, compiled with %s %s\n",
        SUANPAN_COMPILER, version.c_str());
}

void print_helper()
{
    suanpan_info("\n");
    suanpan_info("\t-v,  --version\t\tcheck version information\n");
    suanpan_info("\t-h,  --help\t\tprint this helper\n");
    suanpan_info("\t-f,  --file\t\tprocess model file\n");
    suanpan_info("\t-o,  --output\t\tset output file for logging\n\n");
}

void cli_mode(const shared_ptr<Bead>& model)
{
    string command_line;
    while(true) {
        suanpan_info("suanPan --> ");
        getline(std::cin, command_line);
        if(!command_line.empty() && command_line[0] != '#') {
            istringstream tmp_str(command_line);
            if(process_command(model, tmp_str) == SUANPAN_EXIT) return;
        }
    }
}
