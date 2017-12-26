#include <stdio.h>
#include <stdlib.h>

#include "lib/errors.h"
#include "lib/mtrx.h"
#include "graph.h"

int main(int argc, char **argv)
{
    int err = EOK;
    int t = 1;
    mtrxp_t graph = NULL;

    if (argc != 2)
    {
        printf("Usage: main.run filename\n");
        return 1;
    }
    if ((err = read_mtrx(argv[1], &graph)) != EOK)
        error(err, err, exit);

    print_mtrx(graph);

    printf("Max path len: ");
    scanf("%d", &t);

    if (build_way(t, graph) == EOK)
    {
        printf("Path has been found\n");
        FILE *fout = fopen("graph.dot", "w");
        fprint_graph_dot(fout, graph);
        fclose(fout);
        system("dot -Tpng -o graph.png graph.dot");
    }
    else
    {
        printf("Could not find path\n");
    }

    exit:
    free_mtrx(&graph);

    if (err != EOK)
        printf("Err code: %d\n", err);

    return err != EOK;
}
