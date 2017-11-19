#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#include "unit.h"

int main(void)
{
    srand(time(NULL));
    operate(stdout);

    return 0;
}
