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

TEST(ReadTest, ReadFromDataset) {
    unsigned int n_vertices = 50000;

    std::ostringstream path;
    path << "datasets/" << n_vertices << "/" << n_vertices << ".tsv";
    ifstream test_case (path.str());

    if (test_case.is_open()) {
        std::string line;
        vector<std::string> substrings;
        const std::string delims = " ";

        dynamic_ktree::DKtree<2> graph(n_vertices);
        while (getline (test_case, line))
        {
            split(line, substrings, delims);

            etype x = (etype) stoi(substrings[1]);
            etype y = (etype) stoi(substrings[2]);
            if (substrings[0] == "a") {
                graph.add_edge(x, y);
//                cout << "number of edges: " << graph.get_number_edges() << endl;
            }
        }
        test_case.close();
    } else  {
        cout << "Unable to open file";
        FAIL();
    }
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}