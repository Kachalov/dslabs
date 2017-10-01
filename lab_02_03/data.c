#include <string.h>
#include <stdlib.h>
#include <stdio.h>

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
    init_students_index(&(ss->ndx));

    return OK;
}

int init_students_index(students_index_t *ndx)
{
    for (int i = 0; i < STDNTS_NDX_SLOTS; i++)
        ndx->slots[i] = 0;

    return OK;
}

int student_add(students_t *students, student_t student)
{
    int err = OK;
    students_len_t pos = 0;

    if ((err = student_add_pos(students, &pos)) != OK)
        return err;

    students->data[pos] = student;
    students->ndx.slots[pos / STDNTS_NDX_SLOT_CHUNK] |=
            (uint64_t)1 << (pos % STDNTS_NDX_SLOT_CHUNK);
    students->n++;
    students->ndx.ss[pos] = students->n;

    return err;
}

int student_del(students_t *students, student_t student)
{
    for (int i = 0; i < STDNTS_NDX_SLOTS; i++)
        for (int j = STDNTS_NDX_SLOT_CHUNK - 1; j >= 0; j--)
        {
            if (((uint64_t)students->ndx.slots[i] & ((uint64_t)1 << j))
                == ((uint64_t)1 << j))
            {
                printf("POS DEL %d %d\n", i, j);
                if (strcmp(students->data[i * STDNTS_NDX_SLOT_CHUNK + j].name,
                    student.name) == 0)
                {
                    students->ndx.slots[i] &=
                            ~((uint64_t)1 << j);
                    students->n--;
                    return OK;
                }
            }
        }

    return NOT_FOUND;
}

int student_add_pos(students_t *students, students_len_t *pos)
{
    for (int i = 0; i < STDNTS_NDX_SLOTS; i++)
        for (int j = STDNTS_NDX_SLOT_CHUNK - 1; j >= 0; j--)
        {
            if (((students->ndx.slots[i]) & ((uint64_t)1 << j)) == 0)
            {
                printf("POS %d %d\n", i, j);
                *pos = i * STDNTS_NDX_SLOT_CHUNK + j;
                return OK;
            }
        }

    return OOM;
}
