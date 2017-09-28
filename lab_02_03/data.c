#include <string.h>

#include "data.h"
#include "errors.h"

uint8_t inline students_ver(void)
{
    return STUDENTS_VER;
}

int init_data_t(data_t *d)
{
    d->gender = MALE;
    d->height = 140;
    strcpy(d->name, "");
    strcpy(d->home.street, "");
    d->home.house = 0;
    d->home.room = 0;

    return OK;
}

int init_student_t(student_t *s)
{
    init_data_t(&(s->data));
    s->housing = HOME;

    return OK;
}

int student_add(students_t *students, student_t *student)
{
    return OK;
}

int student_del(students_t *students, student_t *student)
{
    return OK;
}
