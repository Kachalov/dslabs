/**
 *  @author Alexey Kachalov <kachalov@kistriver.com>
 */

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

#include "data.h"
#include "io.h"
#include "errors.h"

void gen_student(student_t *s)
{
    int name_l;

    char fn[][5] = {
        "A.",
        "B.",
        "V.",
        "I.",
        "E.",
        "N.",
        "F.",
        "P."
    };

    char ln[][STDNT_NAME_LEN] = {
        "Ivanov",
        "Petrov",
        "Smirnov",
        "Borisov",
        "Petushkov",
        "Pankratov"
    };

    strcpy(s->name, ln[rand() % (sizeof(ln) / STDNT_NAME_LEN)]);

    name_l = strlen(s->name);
    memcpy(s->name + name_l, " ", 1);
    strcpy(s->name + name_l + 1, fn[rand() % (sizeof(fn) / 5)]);

    name_l = strlen(s->name);
    memcpy(s->name + name_l, " ", 1);
    strcpy(s->name + name_l + 1, fn[(rand() + 3) % (sizeof(fn) / 5)]);

    clear_str(s->name, STDNT_NAME_LEN);
    s->height = 160 + (rand() + 8) % 41;

    sprintf(s->name, "%d", s->height);
}

int main(void)
{
    int err = OK;
    tick_t sort_time = 0;

    srand(time(NULL));

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
        student.height = 170;
        clear_str(student.name, STDNT_NAME_LEN);
        student_add(&ss, student);

        strcpy(student.name, "Viniamin");
        student.height = 181;
        clear_str(student.name, STDNT_NAME_LEN);
        student_add(&ss, student);

        strcpy(student.name, "Vasya");
        student.height = 200;
        clear_str(student.name, STDNT_NAME_LEN);
        student_add(&ss, student);

        strcpy(student.name, "Viniamin");
        student_del(&ss, student);

        strcpy(student.name, "User1");
        student.height = 174;
        clear_str(student.name, STDNT_NAME_LEN);
        student_add(&ss, student);

        strcpy(student.name, "User2");
        student.height = 171;
        clear_str(student.name, STDNT_NAME_LEN);
        student_add(&ss, student);

        strcpy(student.name, "User1");
        student_del(&ss, student);

        {
            int num = ss.n;
            for (int i = 0; i < 10 - num; i++)
            {
                gen_student(&student);
                err = student_add(&ss, student);
                if (err != OK)
                    printf("ERR: %d on %d\n", err, ss.n);
            }
        }

        err = save_students("test.stud", &ss);
    }
    else
        err = load_students("test.stud", &ss);

    sort_time = sort_students(&ss);

    printf("========== Students ==========\n");
    print_students(&ss);
    printf("========== %d students ==========\n", ss.n);
    #ifdef SORT_NDX
        printf("Sort type: index\n");
    #else
        printf("Sort type: data\n");
    #endif
    printf("Sort ticks: %"PRIu64"\n", sort_time);
    printf("Err code: %d\n", err);

    return 0;
}
