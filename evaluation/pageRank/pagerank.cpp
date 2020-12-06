#include "../../include/dktree/DKtree.hpp"
#include "../../include/algorithm/Algorithm.hpp"
#include <string>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <graph serialized folder name> <number of runs> \n",
                argv[0]);
        return EXIT_FAILURE;
    }
    string folder(argv[1]);
    std::ifstream ifs;
    dynamic_ktree::DKtree<2> tree;
    tree.load(ifs, folder, false);

    int runs = atoi(argv[2]);
    double final = 0;
    double sum = 0;
    for (int i = 0; i < runs; i++) {
        clock_t start = clock();
        Algorithm<dynamic_ktree::DKtree<2>>::pageRank(tree, 1.0e-3f, 100);
        clock_t end = clock();
        sum += (double)(end - start) / CLOCKS_PER_SEC;
        final += sum;
    }

    cout << final / (double) runs << endl;
    return 0;
}
