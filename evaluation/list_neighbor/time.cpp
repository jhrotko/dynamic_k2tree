#include <string>

#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>


int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <path to dataset> <number of vertices>\n", argv[0]);
        return EXIT_FAILURE;
    }

    std::ostringstream path;
    path << argv[1];
    std::ifstream test_case(path.str());

    unsigned int n_vertices = atoi(argv[2]);
    double arcs = 0;

    if (test_case.is_open()) {
        std::string line;
        while (getline(test_case, line)) {
            arcs++;
        }
    }

    std::cout << sqrt(arcs) << " " << n_vertices + arcs << std::endl;
    return 0;
}