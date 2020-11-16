#include <iostream>
#include <math.h>
#include "../include/dktree/utils.hpp"

int main(int argc, char *argv[]) {
    //time and memory
    if (argc != 5) {
        fprintf(stderr, "Usage: %s <function name> <time/memory> <nodes> <arcs>\n", argv[0]);
        return EXIT_FAILURE;
    }

    std::string function = std::string(argv[1]);
    std::string time = std::string(argv[2]);
    uint nodes = atoi(argv[3]);
    uint arcs = atoi(argv[4]);
    uint  complexity = 0;
    if(function == "delete") {
        if(time == "time") {
            double aux = log(nodes)/log(2);
            complexity = aux/log(2)/EPS + aux*log(log(arcs)) ;
        } else {
            complexity = nodes + arcs; //FIXME
        }
    } else if(function == "union") {
        if(time == "time") {
            complexity = nodes + arcs;
        } else {
            complexity = nodes;
        }
    }
    std::cout << complexity << std::endl;
    return 0;
}