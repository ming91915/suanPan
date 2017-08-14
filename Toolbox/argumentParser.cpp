#include "argumentParser.h"
#include "commandParser.h"
#include <Domain/Domain.h>
#include <suanPan.h>

void argumentParser(int argc, char** argv)
{
    printf("+------------------------------------------------------------+\n");
    printf("|                                                            |\n");
    printf("|    suanPan --- An Open Source FEM Framework (%u-bit)       |\n", __ARCH__);
    printf("|                                                            |\n");
    printf("|                                           Acrux (0.1.0)    |\n");
    printf("|                                                            |\n");
    printf("|                 maintained by T.L.C. all right reserved    |\n");
    printf("|                                                            |\n");
    printf("+------------------------------------------------------------+\n");

    for(auto I = 0; I < argc; ++I) {
        if(_strcmpi(argv[I], "-v") == 0) {
            printf("A FEM Framework.\n");
            printf("Version: Acrux 0.1.0.\n");
            printf("Date: 29/07/2017.\n");
        } else if(_strcmpi(argv[I], "-f") == 0) {
            auto domain = make_shared<Domain>();
            process_file(domain, argv[++I]);
        }
    }
}