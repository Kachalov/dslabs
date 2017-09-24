/**
 *  @author Alexey Kachalov <kachalov@kistriver.com>
 */

#include <stdio.h>

#include "data.h"
#include "errors.h"

int main(void)
{
    printf("housing_t: %lu\n", sizeof(housing_t));
    printf("data_t: %lu\n", sizeof(data_t));
    printf("student_t: %lu\n", sizeof(student_t));

    student_t s;
    init_student_t(&s);

    return 0;
}
