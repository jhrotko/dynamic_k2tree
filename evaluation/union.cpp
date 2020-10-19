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

    float sum = 0;
    int runs = 5;

    for (int i = 0; i < runs; i++) {
        clock_t start = clock();
        ktree_test.unionOp(ktree_test2);
        clock_t end = clock();

        sum += (float) (end - start) / CLOCKS_PER_SEC;
    }
    cout << sum/(float)runs << endl;
    return 0;
}