/**
 *  @author Alexey Kachalov <kachalov@kistriver.com>
 */

#include <stdio.h>
#include <string.h>

#include "data.h"
#include "io.h"
#include "errors.h"

int main(void)
{
    int err = OK;

    printf("housing_t: %lu\n", sizeof(housing_t));
    printf("data_t: %lu\n", sizeof(data_t));
    printf("student_t: %lu\n", sizeof(student_t));
    printf("students_t: %lu\n", sizeof(students_t));

    students_t ss;
    student_t student;

    init_students_t(&ss);
    init_student_t(&student);

    err = save_students("test.stud", &ss);
//    err = load_students("test.stud", &ss);
//    printf("%d\n", (int) ss.n);
    printf("Err code: %d\n", err);

    return 0;
}
