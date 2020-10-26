#include <gtest/gtest.h>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <string>
#include <vector>
#include "../include/dktree/DKtree_background.hpp"
#include "../include/dktree/DKtree.hpp"

void split(const std::string &str, std::vector<std::string> &cont,
           const std::string &delims = " ") {
    boost::split(cont, str, boost::is_any_of(delims));
}

TEST(ReadTest, ReadFromDataset) {
    unsigned int n_vertices = 10;
    std::ostringstream path;

    path << "datasets/" << n_vertices << "/" << n_vertices << ".tsv";
//    path << "datasets/uk-2007-05@100000/uk-2007-05@100000.tsv";
    ifstream test_case(path.str());
//    dynamic_ktree::DKtree<2> graph(n_vertices);
    dynamic_ktree::DKtree_background<2> graph(n_vertices);
    uint edges = 0;

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
                clock_t aux = clock();
                graph.add_edge(x, y);
                end_add += clock() - aux;
                edges++;
            }
        }

        std::stringstream ss;
        graph.serialize(ss);
//        dynamic_ktree::DKtree<2> graph2;
//        graph2.load(ss, "./", false);
        cout << "TOTAL TIME " << (float) (end_add) / CLOCKS_PER_SEC << endl;
    } else {
        cout << "Unable to open file";
        FAIL();
    }
}
TEST(a,v){
    std::stringstream ss;
    dynamic_ktree::DKtree<2> graph2;
    graph2.load(ss, "./", false);
}
//TEST(ReadTestK2TREE, ReadFromDatasetK2TREE) {
//    std::stringstream ss;
//    unsigned int n_vertices = 50000;
//    std::ostringstream path;
//
//    path << "datasets/" << n_vertices << "/" << n_vertices << ".tsv";
////    path << "datasets/indochina-2004/indochina-2004.tsv";
//    ifstream test_case (path.str());
//
//    if (test_case.is_open()) {
//        std::string line;
//        vector<std::string> substrings;
//        const std::string delims = " ";
//        vector<tuple<uint64_t, uint64_t>> edges;
//
//        while (getline (test_case, line))
//        {
//            split(line, substrings, delims);
//
//            etype x = (etype) stoi(substrings[1]);
//            etype y = (etype) stoi(substrings[2]);
//            if (substrings[0] == "a") {
//                edges.push_back(tuple<uint64_t, uint64_t>(x,y));
//            }
//        }
//        sdsl::k2_tree<2> graph(edges, n_vertices);
//
//        clock_t start = clock();
//        graph.neigh(10);
//        clock_t end = clock();
//        cout << "Neigh time:" << (float)(end-start) / CLOCKS_PER_SEC << endl;
//
//        start = clock();
//        for(auto it = graph.neighbour_begin(10); it != graph.neighbour_end(); ++it) {}
//        end = clock();
//        cout << "iterator time: " << (float)(end-start) / CLOCKS_PER_SEC << endl;
//
//    } else {
//        cout << "Unable to open file";
//        FAIL();
//    }
//}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}