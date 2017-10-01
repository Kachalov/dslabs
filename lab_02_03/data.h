#pragma once

#include <inttypes.h>

#define STDNTS_VER 2

#define STDNTS_MAX 100
#define STDNTS_NDX_SLOT_CHUNK 64
#define STDNTS_NDX_SLOTS (STDNTS_MAX / STDNTS_NDX_SLOT_CHUNK + \
                          STDNTS_MAX % STDNTS_NDX_SLOT_CHUNK != 0)
#define STDNT_NAME_LEN 20
#define STDNT_STREET_LEN 30

typedef uint8_t students_len_t;

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
