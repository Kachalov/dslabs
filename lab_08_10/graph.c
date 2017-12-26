#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "lib/errors.h"
#include "graph.h"

void fprint_graph_dot(FILE *f, mtrxp_t mtrx)
{
    fprintf(f, "digraph g {\n");

    for (int i = 0; i < mtrx->n; i++)
        fprintf(f, "q%d [label=\"%d\"];\n", i, i);

    for (int i = 0; i < mtrx->n; i++)
        for (int j = i + 1; j < mtrx->n; j++)
            if (mtrx->d[i][j] != 0)
            {
                if (mtrx->d[j][i] == 0)
                    fprintf(f, "q%d -> q%d [dir=none, color=green, label=%d];\n", i, j, mtrx->d[i][j]);
                else
                    fprintf(f, "q%d -> q%d [dir=none, label=%d];\n", i, j, mtrx->d[j][i]);
            }

    fprintf(f, "}\n");
}

void dijkstra(mtrxp_t mtrx, int node, int *d)
{
    // TODO: malloc instead of VLA
    int s[mtrx->n];
    int sum = 0;

    for (int i = 0; i < mtrx->n; i++)
        for (int j = 0; j < mtrx->n; j++)
            sum += mtrx->d[i][j];

    for (int i = 0; i < mtrx->n; i++)
        for (int j = 0; j < mtrx->n; j++)
            if (mtrx->d[i][j] == 0)
                mtrx->d[i][j] = (uint32_t) -2;

    for (int i = 0; i < mtrx->n; i++)
    {
        s[i] = 0;
        d[i] = mtrx->d[node][i];
    }

    s[node] = 1;
    for (int i = 0; i < mtrx->n - 1; i++)
    {
        int w = 0;
        for (int j = 1; j < mtrx->n; j++)
            if (!s[w])
            {
                if (!s[j] && d[j] <= d[w])
                    w = j;
            }
            else w++;

        s[w] = 1;
        for (int j = 1; j < mtrx->n; j++ )
            if (!s[j])
                if (d[w] + mtrx->d[w][j] < d[j])
                    d[j] = d[w] + mtrx->d[w][j];
    }
}

int build_way(int t, mtrxp_t mtrx)
{
    int arcs = 0;
    // TODO: malloc instead of VLA
    int arr[mtrx->n * (mtrx->n - 1)/2][2];
    int len[mtrx->n];

    mtrxp_t graph = NULL;
    alloc_mtrx(mtrx->n, mtrx->n, &graph, NULL, NULL);

    for (int i = 0; i < mtrx->n; i++)
        for (int j = i + 1; j < mtrx->n; j++)
            if (mtrx->d[i][j] == 0)
            {
                arr[arcs][0] = i;
                arr[arcs][1] = j;
                arcs++;
            }

    printf("Count of new roads: %d\n", arcs);

    if (arcs < 4)
    {
        for (int i = 0; i < arcs; i++)
            mtrx->d[arr[i][0]][arr[i][1]] = 1;

        free_mtrx(&graph);
        return EOK;
    }

    for (int i = 0; i < arcs - 2; i++)
        for (int j = i + 1; j < arcs - 1; j++)
            for (int z = j + 1; z < arcs; z++)
            {
                for (int y = 1; y < mtrx->n; y++)
                    len[y] = 30;
                len[0] = 0;

                for (int ik = 0; ik < mtrx->n; ik++)
                    for (int jk = 0; jk < mtrx->n; jk++)
                        graph->d[ik][jk] = mtrx->d[ik][jk];

                graph->d[arr[i][0]][arr[i][1]] = 1;
                graph->d[arr[j][0]][arr[j][1]] = 1;
                graph->d[arr[z][0]][arr[z][1]] = 1;
                graph->d[arr[i][1]][arr[i][0]] = 1;
                graph->d[arr[j][1]][arr[j][0]] = 1;
                graph->d[arr[z][1]][arr[z][0]] = 1;

                dijkstra(graph, 0, len);

                int m = 0;
                int mn = 30;

                for (int y = 1; y < mtrx->n; y++)
                    if (len[y] > m)
                        m = len[y];

                for (int y = 1; y < mtrx->n; y++)
                    if (len[y] < mn)
                        mn = len[y];

                if ((m <= t) && (mn > 0))
                {
                    mtrx->d[arr[i][0]][arr[i][1]] = 1;
                    mtrx->d[arr[j][0]][arr[j][1]] = 1;
                    mtrx->d[arr[z][0]][arr[z][1]] = 1;

                    free_mtrx(&graph);
                    return EOK;
                }
            }

    free_mtrx(&graph);
    return ENOPATH;
}
