#pragma once

#ifdef DEBUG
#define DPRINT(...) {fprintf(stderr, "DEBUG: "); fprintf(stderr, __VA_ARGS__); fprintf(stderr, "\n");}
#else
#define DPRINT(...)
#endif