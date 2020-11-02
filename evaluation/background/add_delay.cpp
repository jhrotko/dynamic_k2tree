#include <string>
#include <vector>

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/classification.hpp>
#include "../../include/dktree/DKtree_delay.hpp"

void split(const std::string &str, std::vector<std::string> &cont,
           const std::string &delims = " ") {
    boost::split(cont, str, boost::is_any_of(delims));
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <path to dataset> <number of vertices>\n", argv[0]);
        return EXIT_FAILURE;
    }

    std::ostringstream path;
    path << argv[1];

    std::ifstream test_case(path.str());
    unsigned int n_vertices = atoi(argv[2]);
    int i=0;

    dynamic_ktree::DKtree_delay<2> graph(n_vertices);
    clock_t time_t;
    if (test_case.is_open()) {
        std::string line;
        vector<std::string> substrings;
        const std::string delims = " ";

        while (getline(test_case, line)) {
            split(line, substrings, delims);

            etype x = (etype) stoi(substrings[1]);
            etype y = (etype) stoi(substrings[2]);
            if (substrings[0] == "a") {
                clock_t aux = clock();
                graph.add_edge(x,y);
                clock_t aux_end = clock();
                time_t += aux_end-aux;
                i++;
            }
        }
    }
    cout << ( (float) time_t / CLOCKS_PER_SEC ) << endl;
    return 0;
}