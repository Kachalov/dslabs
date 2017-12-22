#pragma once

#define EOK 0
#define EIO 1
#define EOOM 2
#define ENOTFOUND 3
#define EFORMAT 4
#define ENODATA 5
#define EMTRXSIZE 6
#define ENOFILE 7
#define ENOPATH 8


#define error(err, err_no, label) do {err = err_no; goto label;} while (0)