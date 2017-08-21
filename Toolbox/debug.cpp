#include "debug.h"
#include <cstdarg>

void suanpan_info(const char* M, ...)
{
    va_list arguments;
    va_start(arguments, M);
    vprintf(M, arguments);
    va_end(arguments);
}

void suanpan_debug(const char* M, ...)
{
#ifdef SUANPAN_DEBUG
    va_list arguments;
    va_start(arguments, M);
    vprintf(M, arguments);
    va_end(arguments);
#endif
}

void suanpan_warning(const char* M, ...)
{
    va_list arguments;
    va_start(arguments, M);
    vprintf(M, arguments);
    va_end(arguments);
}

void suanpan_error(const char* M, ...)
{
    va_list arguments;
    va_start(arguments, M);
    vprintf(M, arguments);
    va_end(arguments);
}

void suanpan_fatal(const char* M, ...)
{
    va_list arguments;
    va_start(arguments, M);
    vprintf(M, arguments);
    va_end(arguments);
}