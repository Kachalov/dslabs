#include <inttypes.h>

typedef enum {
    HOME,
    HOSTEL
} housing_t;

typedef enum {
    MALE,
    FEMALE
} gender_t;

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

int init_data_t(data_t *data);
int init_student_t(student_t *student);
