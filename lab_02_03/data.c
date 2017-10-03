#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "data.h"
#include "errors.h"
#include "time.h"

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
    students->n++;

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
    for (int i = next_student(students->n, students);
         i != students->n; i = next_student(i, students))
        {
            /*printf("%d)\t", students->ndx.ss[i] / STDNTS_NDX_SLOTS * STDNTS_NDX_SLOTS +
            (STDNTS_NDX_SLOT_CHUNK - students->ndx.ss[i] % STDNTS_NDX_SLOTS));*/
            print_student(&students->data[
                students->ndx.ss[i]]);
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

inline students_len_t next_student(students_len_t ndx, students_t *s)
{
    //printf("next_ndx(%d)\n", ndx);

    if (ndx == s->n)
    {
        //printf("NDX\n");
        if (s->n == 0)
            return 0;
        else
            ndx = STDNTS_NDX_SLOT_CHUNK - 1;
    }
    else if (ndx % STDNTS_NDX_SLOT_CHUNK == 0)
    {
        ndx += 2 * STDNTS_NDX_SLOT_CHUNK - 1;
        //printf("!!!(%d, %d)\n", ndx / STDNTS_NDX_SLOT_CHUNK, ndx % STDNTS_NDX_SLOT_CHUNK);
    }
    else
    {
        ndx--;
    }

    //printf("next_ndx_new(%d)\n", ndx);

    for (int i = ndx / STDNTS_NDX_SLOT_CHUNK; i < STDNTS_NDX_SLOTS; i++)
        for (int j = ndx % STDNTS_NDX_SLOT_CHUNK; j >= 0; j--)
        {
            //printf("NEXT(%d, %d)\n", i, j);
            if (((s->ndx.slots[i]) & ((uint64_t)1 << j))
            == (uint64_t)1 << j)
            {
                //printf("next(%d)\n", i * STDNTS_NDX_SLOT_CHUNK + j);
                return i * STDNTS_NDX_SLOT_CHUNK + j;
            }
        }
    //printf("next(%d)\n", s->n);
    return s->n;
}

inline student_t *get_student(students_len_t ndx, students_t *students)
{
    return &(students->data[ndx]);
}

tick_t sort_students(students_t *students)
{
    tick_t total = 0;

    for (int i = next_student(students->n, students);
         i != students->n; i = next_student(i, students))
        for (int j = next_student(i, students);
             j != students->n; j = next_student(j, students))
            if (cmp_students(get_student(i, students), get_student(j, students)) > 0)
            {
                #ifdef SORT_NDX
                total += swap_student_ndx(students, i, j);
                #else
                total += swap_student_data(students, i, j);
                #endif
            }

    //printf("=========\n");
    return total;
}

tick_t swap_student_ndx(students_t *students,
    students_len_t i, students_len_t j)
{
    tick_t a = tick();
    students_len_t tmp = students->ndx.ss[i];
    students->ndx.ss[i] = students->ndx.ss[j];
    students->ndx.ss[j] = tmp;

    return tick() - a;
}

tick_t swap_student_data(students_t *students,
    students_len_t i, students_len_t j)
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

    //(void)swap_student_ndx(students, i, j);

    return b - a;
}
