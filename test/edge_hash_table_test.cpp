#include "../include/dktree/EdgeHashTable.hpp"
#include <gtest/gtest.h>

using namespace std;
using edge = tuple<etype, etype>;

TEST(edgeHashTableInsert, createHashTable)
{
    vector<NodeEdge> elements = {NodeEdge(1, 2), NodeEdge(3, 4), NodeEdge(0, 1)};
    EdgeHashTable ht(elements);

    ASSERT_EQ(ht.size(), 3);
}


TEST(edgeHashTableFind, find)
{
    vector<NodeEdge> elements = {NodeEdge(1, 2), NodeEdge(3, 4), NodeEdge(0, 1)};
    EdgeHashTable ht(elements);

    ASSERT_EQ(ht.find(1,1), -1);
    ASSERT_EQ(ht.find(1,2), 0);
    ASSERT_EQ(ht.find(3,4), 1);
    ASSERT_EQ(ht.find(0,1), 2);
}

TEST(edgeHashTableInsert, insert)
{
    vector<NodeEdge> elements = {NodeEdge(1, 2), NodeEdge(3, 4), NodeEdge(0, 1)};
    EdgeHashTable ht(elements);

    ht.insert(5,6, 3);

    ASSERT_EQ(ht.size(), 4);
    ASSERT_EQ(ht.find(5,6), 3);
}

TEST(edgeHashTableErase, eraseedgeExist)
{
    vector<NodeEdge> elements = {NodeEdge(1, 2), NodeEdge(3, 4), NodeEdge(0, 1)};
    EdgeHashTable ht(elements);

    ASSERT_EQ(ht.size(), 3);
    ht.erase(1,2);
    ASSERT_EQ(ht.size(), 2);
    ht.erase(0,0);
    ASSERT_EQ(ht.size(), 2);
}

TEST(edgeHashTableErase, clear)
{
    vector<NodeEdge> elements = {NodeEdge(1, 2), NodeEdge(3, 5), NodeEdge(0, 1)};
    EdgeHashTable ht(elements);

    ASSERT_EQ(ht.size(), 3);
    ht.clear();
    ASSERT_EQ(ht.size(), 0);
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}