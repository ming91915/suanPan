#include "argumentParser.h"
#include "commandParser.h"
#include <Step/Bead.h>
#include <Toolbox/utility.h>
#include <suanPan.h>

#ifndef SUANPAN_DD
#define SUANPAN_DD 16
#endif
#ifndef SUANPAN_MM
#define SUANPAN_MM 9
#endif
#ifndef SUANPAN_YY
#define SUANPAN_YY 2017
#endif

using std::ofstream;
using std::string;

void argument_parser(int argc, char** argv) {
    string input_file_name = "";
    string output_file_name = "";
    ofstream output_file;
    const auto buffer_backup = cout.rdbuf();

    print_header();

    if(argc > 1) {
        for(auto i = 0; i < argc; ++i) {
            if(if_equal(argv[i], "-v") || if_equal(argv[i], "--version"))
                print_version();
            else if(if_equal(argv[i], "-h") || if_equal(argv[i], "--help"))
                print_helper();
            else if(if_equal(argv[i], "-f") || if_equal(argv[i], "--file"))
                input_file_name = argv[++i];
            else if(if_equal(argv[i], "-o") || if_equal(argv[i], "--output"))
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

void print_header() {
    suanpan_info("+------------------------------------------------------------+\n");
    suanpan_info("|   __        __              suanPan is an open source      |\n");
    suanpan_info("|  /  \\      |  \\                    FEM framework (%u-bit)  |\n", SUANPAN_ARCH);
    suanpan_info("|  \\__       |__/ __   __                     Acrux (0.1.0)  |\n");
    suanpan_info("|     \\ |  | |   |  \\ |  |                                   |\n");
    suanpan_info("|  \\__/ |__| |   |__X |  |                maintained by tlc  |\n");
    suanpan_info("|                                       all rights reserved  |\n");
    suanpan_info("+------------------------------------------------------------+\n\n");
}

void print_version() {
#ifdef __VERSION__
    string version = SUANPAN_VERSION;
#elif defined(_MSC_FULL_VER)
    auto version = std::to_string(SUANPAN_VERSION);
#endif
    suanpan_info("suanPan is an open source FEM framework.\n");
    suanpan_info("\tversion Acrux 0.1.0, date $02u/$02u/$4u, compiled with %s %s\n", SUANPAN_DD, SUANPAN_MM, SUANPAN_YY, SUANPAN_COMPILER, version.c_str());
}

void print_helper() {
    suanpan_info("\n");
    suanpan_info("\t-v,  --version\t\tcheck version information\n");
    suanpan_info("\t-h,  --help\t\tprint this helper\n");
    suanpan_info("\t-f,  --file\t\tprocess model file\n");
    suanpan_info("\t-o,  --output\t\tset output file for logging\n\n");
}

void cli_mode(const shared_ptr<Bead>& model) {
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
