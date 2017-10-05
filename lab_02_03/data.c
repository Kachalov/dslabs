#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "data.h"
#include "errors.h"
#include "time.h"
#include "bits.h"

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
    ss->n_empty = 0;
    init_students_index_t(&(ss->ndx));

    return OK;
}

int init_students_index_t(students_index_t *ndx)
{
    for (int i = 0; i < STDNTS_NDX_SLOTS; i++)
        ndx->slots[i] = 0;

    return OK;
}

int student_add(students_t *students, student_t student)
{
    int err = OK;
    ndx_pos_t pos = 0;

    if ((err = student_add_pos(students, &pos)) != OK)
        return err;

    students->data[pos] = student;
    set_bit(students->ndx.slots, pos, true);
    students->ndx.ss[pos] = pos;
    students->n++;
    if (students->n_empty > 0)
        students->n_empty--;

    return err;
}

int student_del(students_t *students, student_t student)
{
    for (int i = 0; i < STDNTS_NDX_SLOTS; i++)
        for (int j = 0; j < STDNTS_NDX_SLOT_CHUNK; j++)
        {
            if (get_bit(students->ndx.slots, i * STDNTS_NDX_SLOT_CHUNK + j))
            {
                if (strcmp(students->data[i * STDNTS_NDX_SLOT_CHUNK + j].name,
                    student.name) == 0)
                {
                    set_bit(students->ndx.slots, i * STDNTS_NDX_SLOT_CHUNK + j, false);
                    students->n--;
                    students->n_empty++;
                    return OK;
                }
            }
        }

    return NOT_FOUND;
}

int student_add_pos(students_t *students, ndx_pos_t *pos)
{
    for (int i = 0; i < STDNTS_NDX_SLOTS; i++)
        for (int j = 0; j < STDNTS_NDX_SLOT_CHUNK; j++)
        {
            if (!get_bit(students->ndx.slots, i * STDNTS_NDX_SLOT_CHUNK + j))
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
    students_len_t num = 0;
    for (ndx_pos_t i = next_student(students->n + students->n_empty, students);
         i != students->n + students->n_empty; i = next_student(i, students))
    {
        printf("%d ndx{%d})\t", ++num, students->ndx.ss[i]);
        print_student(get_student(i, students));
    }
}

void print_student(student_t *student)
{
    printf("%s", student->name);
    for (int i = 0; i < (40 - strlen(student->name)) / 8; i++)
        printf("\t");
    printf("H: %d\n", student->height);
}

int cmp_students(student_t *a, student_t *b)
{
    return a->height - b->height;
}

inline ndx_pos_t next_student(ndx_pos_t ndx, students_t *s)
{
    if (ndx == s->n + s->n_empty)
    {
        if (s->n == 0)
            return 0;
        else
            ndx = 0;
    }
    else
    {
        ndx++;
    }

    for (ndx_pos_t i = ndx / STDNTS_NDX_SLOT_CHUNK; i < STDNTS_NDX_SLOTS; i++)
        for (ndx_pos_t j = ndx % STDNTS_NDX_SLOT_CHUNK; j < STDNTS_NDX_SLOT_CHUNK; j++)
        {
            if (get_bit(s->ndx.slots, i * STDNTS_NDX_SLOT_CHUNK + j))
            {
                return i * STDNTS_NDX_SLOT_CHUNK + j;
            }
        }
    return s->n + s->n_empty;
}

inline student_t *get_student(ndx_pos_t ndx, students_t *students)
{
    return &(students->data[students->ndx.ss[ndx]]);
}

tick_t sort_students(students_t *students)
{
    tick_t total = 0;

    for (ndx_pos_t i = next_student(students->n + students->n_empty, students);
         i != students->n + students->n_empty; i = next_student(i, students))
        for (ndx_pos_t j = next_student(i, students);
             j != students->n + students->n_empty; j = next_student(j, students))
        {
            if (cmp_students(get_student(i, students), get_student(j, students)) > 0)
            {
                #ifdef SORT_NDX
                    total += swap_student_ndx(students, i, j);
                #else
                    total += swap_student_data(students, i, j);
                #endif
            }
        }

    return total;
}

tick_t compress_students(students_t *students)
{
    tick_t a, b;
    ndx_pos_t ndx = 0;
    ndx_pos_t offset = 0;

    a = tick();
    for (ndx_pos_t i = 0; i < STDNTS_NDX_SLOTS; i++)
        for (ndx_pos_t j = 0; j < STDNTS_NDX_SLOT_CHUNK; j++)
        {
            ndx = i * STDNTS_NDX_SLOT_CHUNK + j;
            if (get_bit(students->ndx.slots, ndx))
            {
                if (offset != 0)
                {
                    memcpy(get_student(ndx - offset, students), get_student(ndx, students), sizeof(student_t));
                    set_bit(students->ndx.slots, ndx - offset, get_bit(students->ndx.slots, ndx));
                    set_bit(students->ndx.slots, ndx, false);
                    students->ndx.ss[ndx - offset] = ndx - offset;
                }
            }
            else
            {
                offset++;
            }
        }
    b = tick();

    return b - a;
}


tick_t swap_student_ndx(students_t *students, ndx_pos_t i, ndx_pos_t j)
{
    tick_t a, b;

    a = tick();
    students_len_t tmp = students->ndx.ss[i];
    students->ndx.ss[i] = students->ndx.ss[j];
    students->ndx.ss[j] = tmp;
    b = tick();

    return b - a;
}

tick_t swap_student_data(students_t *students, ndx_pos_t i, ndx_pos_t j)
{
    tick_t a, b;
    student_t stmp;
    student_t *si, *sj;

    a = tick();
    si = get_student(i, students);
    sj = get_student(j, students);
    memcpy(&stmp, si, sizeof(student_t));
    memcpy(si, sj, sizeof(student_t));
    memcpy(sj, &stmp, sizeof(student_t));
    b = tick();

    return b - a;
}
