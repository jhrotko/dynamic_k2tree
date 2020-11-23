#include <string>
#include <vector>

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/classification.hpp>
#include "../../include/dktree/DKtree.hpp"

void split(const std::string &str, std::vector<std::string> &cont,
           const std::string &delims = " ") {
    boost::split(cont, str, boost::is_any_of(delims));
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <path to dataset> <number of vertices> <save folder name>\n", argv[0]);
        return EXIT_FAILURE;
    }

    std::ostringstream path;
    path << argv[1];

    std::ifstream test_case(path.str());
    unsigned int n_vertices = atoi(argv[2]);
    dynamic_ktree::DKtree<2> tree(n_vertices);

    if (test_case.is_open()) {
        std::string line;
        vector<std::string> substrings;
        const std::string delims = " ";

        while (getline(test_case, line)) {
            split(line, substrings, delims);

            uint x = (uint) stoi(substrings[1]);
            uint y = (uint) stoi(substrings[2]);
            if (substrings[0] == "a") {
                tree.add_edge(x, y);
            }
        }
    }

    std::ofstream ss;
    std::string folder(argv[3]);
    tree.serialize(ss, folder);
    ss.close();
    return 0;
}