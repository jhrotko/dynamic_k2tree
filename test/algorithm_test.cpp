#include <gtest/gtest.h>

#include "../include/algorithm/Algorithm.hpp"
#include "../include/dktree/dktree.hpp"
#include <iostream>

using namespace std;

TEST(BFS, dktree)
{
    dynamic_ktree::dktree<2, bit_vector> t(7);
    t.add_edge(1,2);
    t.add_edge(1,3);
    t.add_edge(2,5);
    t.add_edge(2,4);
    t.add_edge(3,5);
    t.add_edge(4,5);
    t.add_edge(4,6);
    t.add_edge(5,6);
    t.add_edge(6,4);
    t.add_edge(6,5);
    t.add_edge(5,4);
    t.add_edge(5,3);
    t.add_edge(5,2);
    t.add_edge(2,1);
    t.add_edge(3,1);

    vector<int> path = Algorithm::bfs(t, 1);

    for(int node: path)
        cout << node << endl;
    ASSERT_TRUE(path[0] == 1);
    ASSERT_TRUE(path[1] == 2);
    ASSERT_TRUE(path[2] == 3);
    ASSERT_TRUE(path[3] == 4);
    ASSERT_TRUE(path[4] == 5);
    ASSERT_TRUE(path[5] == 6);
}


int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
