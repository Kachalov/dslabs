#include <string.h>
#include <stdlib.h>

#include "data.h"
#include "errors.h"

inline uint8_t students_ver(void)
{
    return STDNTS_VER;
}

int init_student_t(student_t *s)
{
    s->gender = MALE;
    s->height = 140;
    strcpy(s->name, "\0                   ");
    strcpy(s->address.home.street, "\0                             ");
    s->address.home.house = 0;
    s->address.home.room = 0;
    s->housing = HOME;

    return OK;
}

int init_students_t(students_t *ss)
{
    ss->n = 0;
    ss->b = NULL;
    ss->e = NULL;

    return OK;
}

int student_add(students_t *students, student_t student)
{
    students_item_t *si;

    si = malloc(sizeof(*si));

    init_student_item_t(si);
    si->s = student;

    if (students->e != NULL)
        students->e->n = si;
    students->e = si;
    students->n++;

    if (students->b == NULL)
        students->b = students->e;

    return OK;
}

int student_del(students_t *students, student_t student)
{
    students_item_t sc;
    init_student_item_t(&sc);
    sc.s = student;

    students_item_t *si = NULL;
    students_item_t *sj;

    for (sj = students->b;
         sj != NULL && sj->s.name != sc.s.name;
         si = sj, sj = sj->n);

    if (sj == students->b)
        students->b = si->n;

    if (sj == students->e)
        students->e = si;

    if (si != NULL)
        si->n = sj->n;

    students->n--;

    return OK;
}

int init_student_item_t(students_item_t *student)
{
    init_student_t(&(student->s));
    student->n = NULL;

    return OK;
}
