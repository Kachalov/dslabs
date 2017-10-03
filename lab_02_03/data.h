#pragma once

#include <inttypes.h>

#include "time.h"

#define STDNTS_VER 2

#define STDNTS_NDX_SLOT_CHUNK 64 //uint64_t
#define STDNTS_NDX_SLOTS 64
#define STDNTS_MAX (STDNTS_NDX_SLOTS * STDNTS_NDX_SLOT_CHUNK)
#define STDNT_NAME_LEN 20
#define STDNT_STREET_LEN 30

typedef uint16_t students_len_t;

typedef enum {
    HOME,
    HOSTEL
} housing_t;

typedef enum {
    MALE,
    FEMALE
} gender_t;

#pragma pack(push, 1)
typedef struct
{
    housing_t housing;
    gender_t gender;
    uint8_t height;
    char name[STDNT_NAME_LEN + 1];
    union
    {
        struct
        {
            uint8_t house;
            uint8_t room;
            char street[STDNT_STREET_LEN + 1];
        } home;
        struct
        {
            uint8_t house;
            uint8_t room;
        } hostel;
    } address;
} student_t;

typedef struct
{
    uint64_t slots[STDNTS_NDX_SLOTS];
    students_len_t ss [STDNTS_MAX];
} students_index_t;

typedef struct
{
    students_len_t n;
    student_t data[STDNTS_MAX];
    students_index_t ndx;
} students_t;
#pragma pack(pop)

int init_student_t(student_t *student);
int init_students_t(students_t *students);
int init_students_index(students_index_t *ndx);
uint8_t students_ver(void);

int student_add_pos(students_t *students, students_len_t *pos);
int student_add(students_t *students, student_t student);
int student_del(students_t *students, student_t student);

void print_students(students_t *students);
void print_student(student_t *student);

tick_t sort_students(students_t *students);

int cmp_students(student_t *a, student_t *b);
students_len_t next_student(students_len_t ndx, students_t *s);
student_t *get_student(students_len_t ndx, students_t *students);

tick_t swap_student_ndx(students_t *students,
    students_len_t i, students_len_t j);
tick_t swap_student_data(students_t *students,
    students_len_t i, students_len_t j);

void clear_str(char *str, size_t len);
