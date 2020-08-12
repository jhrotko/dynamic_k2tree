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
    std::stringstream ss;
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
            }
        }
        graph.serialize(ss);
        test_case.close();
    } else  {
        cout << "Unable to open file";
        FAIL();
    }

    std::ostringstream path_delete;
    path_delete << "datasets/" << n_vertices << "/" << n_vertices << "-deletions.tsv";
    ifstream test_case_delete (path_delete.str());

    if (test_case_delete.is_open()) {
        std::string line;
        vector<std::string> substrings;
        const std::string delims = " ";

        dynamic_ktree::DKtree<2> graph;
        graph.load(ss);
        clock_t start = clock();
        while (getline (test_case_delete, line))
        {
            split(line, substrings, delims);

            etype x = (etype) stoi(substrings[1]);
            etype y = (etype) stoi(substrings[2]);
            if(substrings[0] == "d") {
                graph.del_edge(x,y);
            }
        }
        clock_t end = clock();
        cout << "TIME " << end-start << endl;
        test_case_delete.close();
    } else  {
        cout << "Unable to open file";
        FAIL();
    }
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}