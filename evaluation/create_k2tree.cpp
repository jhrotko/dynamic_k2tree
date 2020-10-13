#include <string>
#include <vector>

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <sdsl/k2_tree.hpp>

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
    if (test_case.is_open()) {
        std::string line;
        vector<std::string> substrings;
        const std::string delims = " ";

        while (getline(test_case, line)) {
            split(line, substrings, delims);

            uint64_t x = (uint64_t) stoi(substrings[1]);
            uint64_t y = (uint64_t) stoi(substrings[2]);
            if (substrings[0] == "a") {
                edges.emplace_back(x,y);
            }
        }
    }
    unsigned int n_vertices = atoi(argv[2]);
    sdsl::k2_tree<2> ktree_test(edges, n_vertices);

    std::ofstream ss(argv[3]);
    ktree_test.serialize(ss);
    ss.close();
    return 0;
}