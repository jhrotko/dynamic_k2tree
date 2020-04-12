#include "../include/adjacency_list.hpp"
#include <gtest/gtest.h>
#include <iostream>

using namespace std;

TEST(adjList, create)
{
    adjacency_list adj_lst(5);

    ASSERT_EQ(adj_lst.size(), 0);

}

TEST(adjList, insert)
{
    adjacency_list adj_lst(5);

    adj_lst.insert(4,2);
    ASSERT_EQ(adj_lst.size(), 1);
}

TEST(adjList, insertExists)
{
    adjacency_list adj_lst(5);

    adj_lst.insert(4,2);
    adj_lst.insert(4,2);
    ASSERT_EQ(adj_lst.size(), 1);
}

TEST(adjList, insertOverflow)
{
    adjacency_list adj_lst(5);

    adj_lst.insert(100,2);
    ASSERT_EQ(adj_lst.size(), 0);
}

TEST(adjList, next)
{
    adjacency_list adj_lst(5);

    adj_lst.insert(4,2);
    ASSERT_EQ(adj_lst[4], 2);
    ASSERT_EQ(adj_lst[10], -1);
}

TEST(adjList, clear)
{
    adjacency_list adj_lst(5);

    adj_lst.insert(4,2);
    adj_lst.insert(3,1);
//    adj_lst.insert(3,0);
    ASSERT_EQ(adj_lst.size(), 2);
    adj_lst.clear();
    ASSERT_EQ(adj_lst.size(), 0);

}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}