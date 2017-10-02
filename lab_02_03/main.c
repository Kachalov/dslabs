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
    printf("student_t: %lu\n", sizeof(student_t));
    printf("students_t: %lu\n", sizeof(students_t));

    students_t ss;
    student_t student;

    init_students_t(&ss);
    init_student_t(&student);

    if (1)
    {
        strcpy(student.name, "Ira");
        clear_str(student.name, STDNT_NAME_LEN);
        student_add(&ss, student);

        strcpy(student.name, "Viniamin");
        clear_str(student.name, STDNT_NAME_LEN);
        student_add(&ss, student);

        strcpy(student.name, "Vasya");
        student.height = 200;
        clear_str(student.name, STDNT_NAME_LEN);
        student_add(&ss, student);
        student.height = 140;

        strcpy(student.name, "Viniamin");
        student_del(&ss, student);

        strcpy(student.name, "User1");
        clear_str(student.name, STDNT_NAME_LEN);
        student_add(&ss, student);

        strcpy(student.name, "User2");
        clear_str(student.name, STDNT_NAME_LEN);
        student_add(&ss, student);

        err = save_students("test.stud", &ss);
    }
    else
        err = load_students("test.stud", &ss);

    print_students(&ss);
    printf("Err code: %d\n", err);

    return 0;
}
