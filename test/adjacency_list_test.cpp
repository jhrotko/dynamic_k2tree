#include "../include/adjacency_list.hpp"
#include <gtest/gtest.h>
#include <iostream>

using namespace std;

TEST(adjListInsert, createAdjList)
{
    vector<item> elements = {item(1, 2), item(3, 4), item(0, 1)};
    adjacency_list adj_lst(elements);

    ASSERT_NE(adj_lst[0].x, -1);
    ASSERT_NE(adj_lst[1].x, -1);
    ASSERT_NE(adj_lst[4].x, -1);
    ASSERT_NE(adj_lst[0].next, -1);
    ASSERT_NE(adj_lst[1].next, -1);
    ASSERT_NE(adj_lst[4].next, -1);
}


TEST(adjListFind, findElement)
{
    vector<item> elements = {item(1, 2), item(3, 4), item(0, 1)};
    adjacency_list adj_lst(elements);

    ASSERT_EQ(adj_lst.find(1), 0);
    ASSERT_EQ(adj_lst.find(10), -1);
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}