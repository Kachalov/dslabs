#pragma once

#ifdef DEBUG
#define DPRINT(...) do {\
fprintf(stderr, "DEBUG: "); fprintf(stderr, __VA_ARGS__); fprintf(stderr, "\n");\
} while (0)
#else
#define DPRINT(...)
#endif
