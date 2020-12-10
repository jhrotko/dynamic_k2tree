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
    string  folder(argv[2]);
    double final = 0;

    dynamic_ktree::DKtree<2> tree;
    std::ifstream ifs;
    tree.load(ifs, folder, false);
    double arcs = 0;
    for (auto edge = tree.edge_begin(); edge != edge.end(); ++edge) {
        arcs++;
    }

    for(int i=0; i < runs; i++) {
        double time_it = 0;
        clock_t  aux = clock();
        for (auto edge = tree.edge_begin(); edge != edge.end(); ++edge) {}
        clock_t add = clock();
        time_it = (double) (add-aux)/CLOCKS_PER_SEC;

        time_it /= arcs;
        final += time_it;
    }
    cout << final / runs << endl;
    return 0;
}
