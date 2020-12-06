#include <string>
#include <vector>

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <sstream>
#include <fstream>
#include <cmath>
#include <iostream>
#include "../../../../../sdk2tree/implementations/sdk2tree/kTree.h"

void split(const std::string &str, std::vector<std::string> &cont,
           const std::string &delims = " ") {
    boost::split(cont, str, boost::is_any_of(delims));
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <path to dataset> <number of vertices> <filename for serialize k2tree>\n", argv[0]);
        return EXIT_FAILURE;
    }

    std::ostringstream path;
    path << argv[1];

    std::ifstream test_case(path.str());
    std::vector<std::tuple<uint64_t, uint64_t>> edges;
    unsigned int n_vertices = atoi(argv[2]);

    unsigned int* xedges;
    unsigned int* yedges;
    uint arcs;
    if (test_case.is_open()) {
        std::string line;
        const std::string delims = " ";

        while (getline(test_case, line)) {
            std::vector<std::string> substrings;
            split(line, substrings, delims);

            uint64_t x = (uint64_t) stoi(substrings[1]);
            uint64_t y = (uint64_t) stoi(substrings[2]);
            if (substrings[0] == "a") {
                edges.emplace_back(x,y);
                ++arcs;
            }
        }
    }
    xedges =  (uint *)malloc(sizeof(uint)*arcs);
    yedges =  (uint *)malloc(sizeof(uint)*arcs);
    uint i = 0;
    for (auto edge: edges) {
        xedges[i] = std::get<0>(edge);
        yedges[i] = std::get<1>(edge);
        ++i;
    }
    MREP* ktree_test = compactCreateKTree(xedges, yedges, n_vertices, arcs, floor(log(n_vertices)/log(K)));
    saveRepresentation(ktree_test, argv[3]);

    free(yedges);
    free(xedges);
    //n + m
    std::cout << n_vertices + edges.size() << std::endl;
    return 0;
}