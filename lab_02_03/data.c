#include <string.h>

#include "data.h"
#include "errors.h"

int init_data_t(data_t *d)
{
    d->gender = MALE;
    d->height = 140;
    strcpy(d->name, "");
    strcpy(d->home.street, "");
    d->home.house = 0;
    d->home.room = 0;

    return OK;
}

int init_student_t(student_t *s)
{
    init_data_t(&(s->data));
    s->housing = HOME;

    return OK;
}
