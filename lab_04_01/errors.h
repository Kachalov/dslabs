#pragma once

#define EOK 0
#define EIO 1
#define EOOM 2
#define ENOTFOUND 4
#define EFORMAT 5
#define ENULL 6

#define error(err, err_no, label) {err = err_no; goto label;}
