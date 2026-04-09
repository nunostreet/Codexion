#include "codexion.h"
#include <stdio.h>
#include <stdlib.h>


void    error_exit(const char *error)
{
    printf(BOLD_RED "%s\n"RST, error);
    exit(EXIT_FAILURE);
}
