#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#include "unit.h"

int main(int argc, char **argv)
{
    srand(time(NULL));
    operate(stdout, argc == 2 ? (strcmp(argv[1], "-v") == 0) : 0);

    return 0;
}
