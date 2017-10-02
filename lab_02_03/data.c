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

    strcpy(s->name, "");
    clear_str(s->name, STDNT_NAME_LEN);

    strcpy(s->address.home.street, "");
    clear_str(s->address.home.street, STDNT_STREET_LEN);

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
    students->ndx.ss[pos] = pos;

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
                *pos = i * STDNTS_NDX_SLOT_CHUNK + j;
                return OK;
            }
        }

    return OOM;
}

void clear_str(char *str, size_t len)
{
    char *it = str;
    int i = 0;

    for (; i < len && *it != '\0'; it++, i++);

    for(; i < len; it++, i++)
        *it = '\0';
}

void print_students(students_t *students)
{
    for (int i = 0; i < STDNTS_NDX_SLOTS; i++)
        for (int j = STDNTS_NDX_SLOT_CHUNK - 1; j >= 0; j--)
            if (((students->ndx.slots[i]) & ((uint64_t)1 << j))
            == (uint64_t)1 << j)
            {
                print_student(&students->data[
                    students->ndx.ss[i * STDNTS_NDX_SLOT_CHUNK + j]]);
            }
}

void print_student(student_t *student)
{
    printf("Name: %s Heigth: %d\n", student->name, student->height);
}
