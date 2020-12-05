#include <gtest/gtest.h>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <string>
#include <vector>
#include "../include/dktree/DKtree_background.hpp"
#include "../include/dktree/DKtree.hpp"
#include "../include/dktree/DKtree_delay.hpp"
#include "../include/dktree/DKtree_delay_.hpp"

void split(const std::string &str, std::vector<std::string> &cont,
           const std::string &delims = " ") {
    boost::split(cont, str, boost::is_any_of(delims));
}

TEST(ReadTest, ReadFromDataset) {
    unsigned int n_vertices = 500000;
    std::ostringstream path;

    path << "datasets/" << n_vertices << "/" << n_vertices << ".tsv";
//    path << "datasets/uk-2007-05@100000/uk-2007-05@100000.tsv";
    ifstream test_case(path.str());
//    dynamic_ktree::DKtree <2> graph(n_vertices);
    dynamic_ktree::DKtree_background<2> graph(n_vertices); //per edge: 9.36066e-06 total: TOTAL TIME: 28.3361
//    dynamic_ktree::DKtree_delay<2> graph(n_vertices);
//    dynamic_ktree::DKtree_delay_munro<2> graph(n_vertices); //per edge: 8.50478e-06 total: 26.4127
    double i = 0;
    double sum = 0;
    if (test_case.is_open()) {
        std::string line;
        vector<std::string> substrings;
        const std::string delims = " ";
        clock_t end_add = clock();

        while (getline(test_case, line)) {
            split(line, substrings, delims);

            etype x = (etype) stoi(substrings[1]);
            etype y = (etype) stoi(substrings[2]);
            if (substrings[0] == "a") {
                ++i;
                clock_t aux = clock();
                graph.add_edge(x, y);
                end_add = clock();
                sum += (double) (end_add-aux) / CLOCKS_PER_SEC;
            }
        }
        cout << "TOTAL TIME: " << sum << endl;
    } else {
        cout << "Unable to open file";
        FAIL();
    }
}

TEST(a, v) {
    std::ifstream ifs("../evaluation/serialized/1000000/dktree_serialize/7.kt");
    sdsl::k2_tree<2> ktree_test = sdsl::k2_tree<2>();
    ktree_test.load(ifs);
    ifs.close();

    std::ifstream ifs2("../evaluation/serialized/1000000/dktree_serialize/7.kt");
    sdsl::k2_tree<2> ktree_test2 = sdsl::k2_tree<2>();
    ktree_test2.load(ifs2);
    ifs2.close();

    shared_ptr<sdsl::k2_tree<2>> ptr1 = make_shared<sdsl::k2_tree<2>>(ktree_test);
    shared_ptr<sdsl::k2_tree<2>> ptr2 = make_shared<sdsl::k2_tree<2>>(ktree_test2);

    ptr1->unionOp(ptr2);
}

TEST(edge, iterator) {
    int runs = 1;
    string folder("../evaluation/serialized/10000");
    double final = 0;

    dynamic_ktree::DKtree<2> tree;
    std::ifstream ifs;
    tree.load(ifs, folder, false);

    for (int i = 0; i < runs; i++) {
        int arcs = 0;
        double sum = 0;
        double total = 0;
        for (auto edge = tree.edge_begin(); edge != edge.end();) {
            clock_t aux = clock();
            ++edge;
            clock_t add = clock() - aux;
            sum += add;
            total += add;
            arcs++;
        }
        sum /= arcs;
        final += sum / CLOCKS_PER_SEC;
    }
    cout << final / (double) runs << endl;
}

TEST(neighbour, iterator) {
    std::ifstream ifs;
    dynamic_ktree::DKtree<2> ktree;
    ktree.load(ifs, "../evaluation/serialized/1000000", false);

    clock_t aux_end, aux;
    for (uint i = 1000; i < 10000; ++i) {
        cout << "i:" << i << endl;
//        aux = clock();
//        for (auto neigh = ktree.neighbour_begin(i); neigh != ktree.neighbour_end(); ++neigh) {}
//        aux_end = clock();
//        cout << "ITERATOR -- TOTAL TIME:" << (double) (aux_end - aux) / CLOCKS_PER_SEC << endl;

//        aux = clock();
//        ktree.neigh_it(i, [](etype x) {});
//        aux_end = clock();
//        cout << "IT -- TOTAL TIME:" << (double) (aux_end - aux) / CLOCKS_PER_SEC << endl;

        aux = clock();
        ktree.list_neighbour(i);
        aux_end = clock();
        cout << "LIST -- TOTAL TIME:" << (double) (aux_end - aux) / CLOCKS_PER_SEC << endl;
    }
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}