#pragma once

#include <inttypes.h>

#define STUDENTS_VER 1

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
    gender_t gender;
    uint8_t height;
    char name[20];
    union
    {
        struct
        {
            uint8_t house;
            uint8_t room;
            char street[30];
        } home;
        struct
        {
            uint8_t house;
            uint8_t room;
        } hostel;
    };
} data_t;

typedef struct
{
    housing_t housing;
    data_t data;
} student_t;

typedef struct _students_t
{
    student_t s;
    struct _students_t *n;
} students_t;
#pragma pack(pop)

int init_data_t(data_t *data);
int init_student_t(student_t *student);
uint8_t inline students_ver(void);

int student_add(students_t *students, student_t *student);
int student_del(students_t *students, student_t *student);
