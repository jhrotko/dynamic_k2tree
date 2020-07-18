#include "../include/dktree/AdjacencyList.hpp"
#include <gtest/gtest.h>
#include <iostream>

using namespace std;

TEST(adjList, create)
{
    AdjacencyList adj_lst(5);

    ASSERT_EQ(adj_lst.size(), 0);

}

TEST(adjList, insert)
{
    AdjacencyList adj_lst(5);

    adj_lst.insert(4,2);
    ASSERT_EQ(adj_lst.size(), 1);
}

TEST(adjList, insertExists)
{
    AdjacencyList adj_lst(5);

    adj_lst.insert(4,2);
    adj_lst.insert(4,2);
    ASSERT_EQ(adj_lst.size(), 1);
}

TEST(adjList, insertOverflow)
{
    AdjacencyList adj_lst(5);

    adj_lst.insert(100,2);
    ASSERT_EQ(adj_lst.size(), 0);
}

TEST(adjList, next)
{
    AdjacencyList adj_lst(5);

    adj_lst.insert(4,2);
    ASSERT_EQ(adj_lst[4].next(), 2);
    ASSERT_TRUE(adj_lst[2].end());
}

TEST(adjList, clear)
{
    AdjacencyList adj_lst(5);

    adj_lst.insert(4,2);
    adj_lst.insert(3,1);
//    adj_lst.add_edge(3,0);
    ASSERT_EQ(adj_lst.size(), 2);
    adj_lst.clear();
    ASSERT_EQ(adj_lst.size(), 0);

}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}