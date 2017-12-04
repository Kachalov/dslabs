#pragma once

#include <inttypes.h>

typedef uint64_t tick_t;

extern tick_t ticks;

tick_t tick(void);
