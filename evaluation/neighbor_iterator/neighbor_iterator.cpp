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
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <number of runs> <save folder name>\n",
                argv[0]);
        return EXIT_FAILURE;
    }
    int runs = atoi(argv[1]);
    string folder(argv[2]);
    double final = 0;

    dynamic_ktree::DKtree<2> tree;
    std::ifstream ifs;
    tree.load(ifs, folder, false);

    for (int i = 0; i < runs; i++) {
        int arcs = 0;
        double sum = 0;
        double total = 0;
        for (auto node = tree.node_begin(); node != tree.node_end(); ++node) {
            for (auto neigh = tree.neighbour_begin(*node); neigh != tree.neighbour_end();) {
                clock_t aux = clock();
                ++neigh;
                clock_t add = clock() - aux;
                sum += add;
                total += add;
                arcs++;
            }
        }
        sum /= arcs;
        final += sum / CLOCKS_PER_SEC;
    }
    cout << final / (double) runs << endl;
    return 0;
}
