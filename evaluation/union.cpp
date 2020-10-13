#include <sdsl/k2_tree.hpp>
#include <iostream>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <path serialized ktree> <path serialized ktree>\n", argv[0]);
        return EXIT_FAILURE;
    }

    std::ifstream ifs(argv[1]);
    std::ifstream ifs2(argv[2]);
    sdsl::k2_tree<2> ktree_test, ktree_test2;
    ktree_test.load(ifs);
    ktree_test2.load(ifs2);

    clock_t start = clock();
    ktree_test.unionOp(ktree_test2);
    clock_t end = clock();

    std::cout << "Union Time: " << (float)(end-start) / CLOCKS_PER_SEC << std::endl;
    return 0;
}