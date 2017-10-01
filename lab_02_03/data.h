#pragma once

#include <inttypes.h>

#define STDNTS_VER 2

#define STDNT_NAME_LEN 20
#define STDNT_STREET_LEN 30

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

typedef struct students_item_t
{
    student_t s;
    struct students_item_t *n;
} students_item_t;

typedef struct
{
    size_t n;
    students_item_t *b;
    students_item_t *e;
} students_t;
#pragma pack(pop)

int init_student_t(student_t *student);
int init_students_t(students_t *students);
int init_student_item_t(students_item_t *student);
uint8_t students_ver(void);

int student_add(students_t *students, student_t student);
int student_del(students_t *students, student_t student);
