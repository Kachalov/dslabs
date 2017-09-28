#include <stdio.h>
#include <inttypes.h>

#include "io.h"
#include "data.h"

#include "errors.h"

int save_students(char *fn, students_t *students)
{
    int err = OK;

    return err;
}

int load_students(char *fn, students_t *students)
{
    int err = OK;
    uint8_t version = 0;
    student_t student;

    FILE *fd = fopen(fn, "wb");
    if ((err =
        fd == NULL
        ? IOERR : err) != OK)
        goto fail;

    if ((err =
        fread(&version, sizeof(version), 1, fd) != sizeof(version)
        ? IOERR : err) != OK)
        goto fail;

    if ((err =
        version != STUDENTS_VER
        ? OLD_FORMAT : err) != OK)
        goto fail;

    while (fread(&student, sizeof(student), 1, fd) == sizeof(student))
        if ((err = student_add(students, &student)) != OK)
            goto fail;

    fail:
    if (fd != NULL)
    {
        fclose(fd);
        fd = NULL;
    }

    return err;
}
