#include <stdio.h>
#include <time.h>
#include "../../../../../sdk2tree/implementations/sdk2tree/kTree.h"
#include "../../../../../sdk2tree/implementations/sdk2tree/kt_setOperations.h"

int main(int argc, char *argv[]) {
    MREP* repA = loadRepresentation(argv[1]);
    MREP* repB = loadRepresentation(argv[1]);

    double sum = 0;
    int runs = 5;

    for (int i = 0; i < runs; i++) {
        clock_t start = clock();
        MREP* result = k2tree_union(repA, repB);
        clock_t end = clock();

        sum += (double) (end - start) / CLOCKS_PER_SEC;
        destroyRepresentation(result);
    }
    printf("%f\n", sum/(double)runs);
    destroyRepresentation(repA);
    destroyRepresentation(repB);
    return 0;
}