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
    dynamic_ktree::DKtree<2> graph(n_vertices);

    if (test_case.is_open()) {
        std::string line;
        vector<std::string> substrings;
        const std::string delims = " ";

        while (getline (test_case, line))
        {
            split(line, substrings, delims);

            etype x = (etype) stoi(substrings[1]);
            etype y = (etype) stoi(substrings[2]);
            if (substrings[0] == "a") {
                graph.add_edge(x, y);
            }
        }
//        graph.contains(27519, 49974);
//        graph.serialize(ss);
        test_case.close();
    } else  {
        cout << "Unable to open file";
        FAIL();
    }

    std::ostringstream path_delete;
    path_delete << "datasets/" << n_vertices << "/" << n_vertices << "-deletions.tsv";
//    path_delete << "datasets/" << n_vertices << "/" << n_vertices << ".tsv";
    ifstream test_case_delete (path_delete.str());

    if (test_case_delete.is_open()) {
        std::string line;
        vector<std::string> substrings;
        const std::string delims = " ";

//        dynamic_ktree::DKtree<2> graph_loaded;
//        graph_loaded.load(ss);
//        ASSERT_EQ(graph, graph_loaded);
        clock_t start = clock();
        uint64_t total = graph.get_number_edges();
        uint64_t removed = 0;
        while (getline (test_case_delete, line))
        {
            split(line, substrings, delims);

            etype x = (etype) stoi(substrings[1]);
            etype y = (etype) stoi(substrings[2]);
            if(substrings[0] == "d") {
//            if(substrings[0] == "a") {
                removed++;
                graph.del_edge(x,y);
            }
        }
        clock_t end = clock();
        cout << "TOTAL TIME " << (float)(end-start) / CLOCKS_PER_SEC << endl;
        cout << "Total edges: " << graph.get_number_edges() << endl;
        cout << "Supposly: " << total-removed << endl;
        test_case_delete.close();
        ASSERT_EQ(graph.get_number_edges(), total-removed);
//        ASSERT_EQ(graph.get_number_edges(), 555705);
    } else  {
        cout << "Unable to open file";
        FAIL();
    }
}


TEST(k2_tree_test_marked, edge_it) {
    vector<vector<int>> mat({{1, 0, 0, 0, 0, 0, 0, 0},
                             {0, 0, 0, 0, 0, 0, 0, 0},
                             {0, 0, 0, 0, 0, 0, 0, 0},
                             {0, 0, 0, 0, 0, 0, 0, 0},
                             {0, 0, 0, 0, 0, 0, 0, 0},
                             {0, 0, 1, 0, 0, 0, 0, 0},
                             {0, 0, 0, 0, 0, 0, 0, 0},
                             {0, 0, 0, 1, 0, 0, 0, 0}});

    auto tree_a = k2_tree<2>(mat);
    tree_a.erase(0,0);
    tree_a.erase(7, 3);

    vector<vector<int>> mat2({{0, 0, 0, 0, 0, 0, 0, 0},
                              {0, 0, 0, 0, 0, 0, 0, 0},
                              {0, 0, 0, 0, 0, 0, 0, 0},
                              {0, 0, 0, 0, 0, 0, 0, 0},
                              {0, 0, 0, 0, 0, 0, 0, 0},
                              {0, 0, 0, 0, 1, 0, 0, 0},
                              {0, 0, 0, 0, 0, 0, 0, 0},
                              {0, 0, 0, 0, 1, 0, 0, 0}});

    auto tree_b = k2_tree<2>(mat2);
    tree_b.erase(7,4);
    tree_a.unionOp(tree_b);

    std::vector<unsigned> expected_t = {1,0,1,1, 1,0,0,0, 0,1,0,1, 1,0,1,0};
    std::vector<unsigned> expected_l = {0,0,0,0, 0,0,1,0, 0,0,0,0, 0,0,1,0, 0,0,0,0};

    ASSERT_EQ(expected_t.size(), tree_a.t().size());
    ASSERT_EQ(expected_l.size(), tree_a.l().size());
    for (unsigned i = 0; i < expected_t.size(); i++){

        ASSERT_EQ(expected_t[i], tree_a.t().get_int(i, 1));
    }
    for (unsigned i = 0; i < expected_l.size(); i++) {
        cout << "i:" << i << endl;
        ASSERT_EQ(expected_l[i], tree_a.l().get_int(i, 1));
    }
}

TEST(k2_tree_test_msarked, edges_it) {
    vector<vector<int>> mat({{1, 0, 0, 0, 1},
                             {0, 0, 0, 0, 0},
                             {0, 0, 1, 1, 0},
                             {0, 0, 0, 0, 0},
                             {0, 0, 1, 0, 1}});

    k2_tree<2> tree = k2_tree<2>(mat);
    k2_tree<2> t = tree;
    tree.edge_it([t] (uint64_t i, uint64_t j)-> void { ASSERT_TRUE(t.adj(i,j)); });
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}