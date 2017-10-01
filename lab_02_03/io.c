#include <stdio.h>
#include <inttypes.h>

#include "io.h"
#include "data.h"

#include "errors.h"

#define TRY(f, ex, t, e, g) {if ((e = (f) != (ex) ? (t) : e) != OK) goto g;}
#define EXCEPT(g) g:

int save_students(char *fn, students_t *students)
{
    int err = OK;
    uint8_t version = STDNTS_VER;

    FILE *fd = fopen(fn, "wb");

    if ((err =
        fd == NULL
        ? IOERR : err) != OK)
        goto fail;

//    TRY(fd != NULL, 1, IOERR, err, fail);
//    TRY(fwrite(&version, sizeof(version), 1, fd), 1, IOERR, err, fail);

    if ((err =
        fwrite(&version, sizeof(version), 1, fd) != 1
        ? IOERR : err) != OK)
        goto fail;

    for (students_item_t *si = students->b;
         si != NULL;
         si = si->n)
        if ((err =
            fwrite(&si->s, sizeof(si->s), 1, fd) != 1
            ? IOERR : err) != OK)
            goto fail;

//    EXCEPT(fail);
    fail:
    if (fd != NULL)
    {
        fclose(fd);
        fd = NULL;
    }

    return err;
}

int load_students(char *fn, students_t *students)
{
    int err = OK;
    uint8_t version = 0;
    student_t student;

    FILE *fd = fopen(fn, "rb");

    if ((err =
        fd == NULL
        ? IOERR : err) != OK)
        goto fail;

    if ((err =
        fread(&version, sizeof(version), 1, fd) != 1
        ? IOERR : err) != OK)
        goto fail;

    if ((err =
        version != STDNTS_VER
        ? OLD_FORMAT : err) != OK)
        goto fail;

    while (fread(&student, sizeof(student), 1, fd) == 1)
    {
        if ((err = student_add(students, student)) != OK)
            goto fail;
    }

    fail:
    if (fd != NULL)
    {
        fclose(fd);
        fd = NULL;
    }

    return err;
}
