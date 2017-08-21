#include "argumentParser.h"
#include "commandParser.h"
#include <Step/Bead.h>
#include <suanPan.h>

using std::string;

void argumentParser(int argc, char** argv)
{
    printf("+------------------------------------------------------------+\n");
    printf("|   __        __            |  suanPan is an open source     |\n");
    printf("|  /  \\      |  \\           |        FEM framework (%u-bit)  |\n",
        SUANPAN_ARCH);
    printf("|  \\__       |__/ __   __   |                 Acrux (0.1.0)  |\n");
    printf("|     \\ |  | |   |  \\ |  |  |                                |\n");
    printf("|  \\__/ |__| |   |__X |  |  |          maintained by T.L.C.  |\n");
    printf("|                           |           all rights reserved  |\n");
    printf("+------------------------------------------------------------+\n\n");

#ifdef __VERSION__
    string version = SUANPAN_VERSION;
#elif defined(_MSC_FULL_VER)
    auto version = std::to_string(SUANPAN_VERSION);
#endif

    if(argc > 1) {
        for(auto I = 0; I < argc; ++I) {
            if(_strcmpi(argv[I], "-v") == 0 || _strcmpi(argv[I], "--version") == 0) {
                printf("A FEM Framework.\n");
                printf("\tversion Acrux 0.1.0\n");
                printf("\tdate 22/08/2017\n");
                printf(
                    "\tcompiled with %s version %s\n", SUANPAN_COMPILER, version.c_str());
            } else if(_strcmpi(argv[I], "-f") == 0 || _strcmpi(argv[I], "--file") == 0) {
                auto model = make_shared<Bead>();
                process_file(model, argv[++I]);
            } else if(_strcmpi(argv[I], "-h") == 0 || _strcmpi(argv[I], "--help") == 0) {
                printf("Hello World.\n");
            }
        }
    } else {
        auto model = make_shared<Bead>();
        string command_line;
        while(true) {
            printf("suanPan --> ");
            getline(std::cin, command_line);
            if(!command_line.empty()) {
                istringstream tmp_str(command_line);
                if(process_command(model, tmp_str) == 1) return;
            }
        }
    }
}
