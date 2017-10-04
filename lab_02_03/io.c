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

    for (ndx_pos_t i = 0; i < STDNTS_NDX_SLOTS; i++)
        for (ndx_pos_t j = 0; j < STDNTS_NDX_SLOT_CHUNK; j++)
        {
            if ((students->ndx.slots[i] & ((uint64_t)1 << j)) == (uint64_t)1 << j)
            {
                if ((err =
                             fwrite(&students->data[i * STDNTS_NDX_SLOT_CHUNK + j],
                                    sizeof(student_t), 1, fd) != 1
                             ? IOERR : err) != OK)
                    goto fail;
            }
        }

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
        students->n++;
    }

    fail:
    if (fd != NULL)
    {
        fclose(fd);
        fd = NULL;
    }

    return err;
}
