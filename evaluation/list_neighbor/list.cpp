#include <string>
#include <vector>

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/classification.hpp>
#include "../../include/dktree/DKtree.hpp"

#include <cstdlib>
#include <cstdio>

void split(const std::string &str, std::vector<std::string> &cont,
           const std::string &delims = " ") {
    boost::split(cont, str, boost::is_any_of(delims));
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <path file> <number of runs> <save folder name>\n",
                argv[0]);
        return EXIT_FAILURE;
    }
    std::ostringstream path;
    path << argv[1];
    int runs = atoi(argv[2]);
    string folder(argv[3]);
    double final = 0;

    for (int i = 0; i < runs; i++) {
        std::ifstream test_case(path.str());

        dynamic_ktree::DKtree<2> tree;
        std::ifstream ifs;
        tree.load(ifs, folder, false);
        double sum = 0;
        double times = 0;

        if (test_case.is_open()) {
            std::string line;
            vector<std::string> substrings;
            const std::string delims = "\t";

            while (getline(test_case, line)) {
                split(line, substrings, delims);

                etype x = (etype) stoi(substrings[1]);

                clock_t start = clock();
                tree.list_neighbour(x);
                sum += clock() - start;
                ++times;
            }
        }
        test_case.close();
        sum /= times;
        sum /= CLOCKS_PER_SEC;
        final += sum;
    }
    cout << final / (double) runs << endl;
    return 0;
}
