#pragma once

#ifdef DEBUG
#define DPRINT(...) do\
{\
    fprintf(stderr, "debug(%s:%d): ", __FILE__, __LINE__);\
    fprintf(stderr, __VA_ARGS__);\
} while (0)
#else
#define DPRINT(...) (void)0
#endif
