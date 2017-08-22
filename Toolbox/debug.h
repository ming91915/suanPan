#ifndef DEBUG_H
#define DEBUG_H

#include <suanPan.h>

void suanpan_info(const char*, ...);
void suanpan_debug(const char*, ...);
void suanpan_warning(const char*, ...);
void suanpan_error(const char*, ...);
void suanpan_fatal(const char*, ...);

#endif
