#include <sdsl/k2_tree.hpp>
#include <iostream>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <path serialized ktree> <path serialized ktree>\n", argv[0]);
        return EXIT_FAILURE;
    }

    std::ifstream ifs(argv[1]);
    sdsl::k2_tree<2> ktree_test;
    ktree_test.load(ifs);
    ifs.close();

    std::ifstream ifs2(argv[2]);
    sdsl::k2_tree<2> ktree_test2 = sdsl::k2_tree<2>();
    ktree_test2.load(ifs2);
    ifs2.close();

    shared_ptr<sdsl::k2_tree<2>> ptr1 = make_shared<sdsl::k2_tree<2>>(ktree_test);
    shared_ptr<sdsl::k2_tree<2>> ptr2 = make_shared<sdsl::k2_tree<2>>(ktree_test2);
    double sum = 0;
    int runs = 5;
    string test_path(argv[1]);
    if(test_path.find("-20") != std::string::npos)
        runs = 1;

    for (int i = 0; i < runs; i++) {
        clock_t start = clock();
        ptr1->unionOp(ptr2);
        clock_t end = clock();

        sum += (double) (end - start) / CLOCKS_PER_SEC;
    }
    cout << sum/(double)runs << endl;
    return 0;
}