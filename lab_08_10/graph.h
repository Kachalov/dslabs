#pragma once

#include "lib/mtrx.h"

void fprint_graph_dot(FILE *f, mtrxp_t mtrx);
void dijkstra(mtrxp_t mtrx, int node, int *d);
int build_way(int T, mtrxp_t mtrx);
