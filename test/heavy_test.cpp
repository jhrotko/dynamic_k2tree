//
// Created by joana on 19/07/20.
//
#include <gtest/gtest.h>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <string>
#include <vector>
#include "../include/dktree/DKtree.hpp"

void split(const std::string &str, vector<string> &cont,
           const std::string &delims = " ") {
    boost::split(cont, str, boost::is_any_of(delims));
}

TEST(ReadTest, heavy_50000) {
    ifstream test_case("datasets/5000/5000.tsv");

    if (test_case.is_open()) {
        std::string line;
        vector<std::string> substrings;
        const std::string delims = " ";

        dynamic_ktree::DKtree<2> graph(5000);
        while (getline(test_case, line)) {
            split(line, substrings, delims);

            etype x = (etype) stoi(substrings[1]);
            etype y = (etype) stoi(substrings[2]);
            if (substrings[0] == "a") {
                graph.add_edge(x, y);
                cout << "number of edges: " << graph.get_number_edges() << endl;
            }
        }
        test_case.close();
    } else  {
        cout << "Unable to open file";
        FAIL();
    }
}

//TEST(ktreeUnion, memoryLeaks) {
//    using ktree = k2_tree<2>;
//
//    ktree tree1({{1, 0, 1, 0},
//                        {0, 1, 1, 1},
//                        {1, 1, 1, 1},
//                        {0, 0, 0, 1}});
//    shared_ptr<ktree> ptr_tree1 = make_shared<ktree>(tree1);
//
//    ktree tree2({{1, 0, 1, 0},
//                        {0, 0, 0, 1},
//                        {1, 0, 1, 1},
//                        {0, 0, 0, 1}});
//    shared_ptr<ktree> ptr_tree2 = make_shared<ktree>(tree2);
//}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}