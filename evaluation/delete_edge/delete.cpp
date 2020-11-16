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
        fprintf(stderr, "Usage: %s <path file> <number of runs> <save folder name>\n",
                argv[0]);
        return EXIT_FAILURE;
    }
    int runs = atoi(argv[2]);
    string  folder(argv[3]);
    double final = 0;

    for(int i=0; i < runs; i++) {
        int arcs = 0;
        double sum = 0;
        std::ostringstream path;
        path << argv[1];
        std::ifstream test_case(path.str());

        dynamic_ktree::DKtree<2> tree;
        std::ifstream ifs;
        tree.load(ifs, folder, false);

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
                    clock_t start = clock();
                    tree.del_edge(x,y);
                    sum += clock() - start;
                    ++arcs;
                }
            }
        }
        sum /= arcs;
        sum /= CLOCKS_PER_SEC;
        final += sum;
        test_case.close();
    }
    cout << final / (double) runs << endl;
    return 0;
}
