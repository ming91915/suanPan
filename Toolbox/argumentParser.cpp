#include "argumentParser.h"
#include "commandParser.h"
#include <Domain/Domain.h>
#include <suanPan.h>

void argumentParser(int argc, char** argv)
{
    printf("+------------------------------------------------------------+\n");
    printf("|                                                            |\n");
    printf(
        "|    suanPan --- An Open Source FEM Framework (%u-bit)       |\n", SUANPAN_ARCH);
    printf("|                                                            |\n");
    printf("|                                           Acrux (0.1.0)    |\n");
    printf("|                                                            |\n");
    printf("|                 maintained by T.L.C. all right reserved    |\n");
    printf("|                                                            |\n");
    printf("+------------------------------------------------------------+\n");

#ifdef __VERSION__
    string version = SUANPAN_VERSION;
#elif defined(_MSC_FULL_VER)
    auto version = std::to_string(SUANPAN_VERSION);
#endif

    for(auto I = 0; I < argc; ++I) {
        if(_strcmpi(argv[I], "-v") == 0) {
            printf("A FEM Framework.\n");
            printf("\tversion Acrux 0.1.0\n");
            printf("\tdate 14/08/2017\n");
            printf("\tcompiled with %s version %s\n", SUANPAN_COMPILER, version.c_str());
        } else if(_strcmpi(argv[I], "-f") == 0) {
            auto domain = make_shared<Domain>();
            process_file(domain, argv[++I]);
        }
    }
}