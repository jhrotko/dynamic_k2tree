#include "../include/edge_hash_table.hpp"
#include <gtest/gtest.h>
#include <iostream>

using namespace std;

TEST(edgeHashTableInsert, createHashTable)
{
    vector<edge> elements = {edge(1, 2), edge(3, 4), edge(0, 1)};
    edge_hash_table ht(elements);
    
    ASSERT_EQ(ht.size(), 3);
}


TEST(edgeHashTableFind, find)
{
    vector<edge> elements = {edge(1, 2), edge(3, 4), edge(0, 1)};
    edge_hash_table ht(elements);

    ASSERT_EQ(ht.find(1,1), -1);
    ASSERT_EQ(ht.find(1,2), 0);
    ASSERT_EQ(ht.find(3,4), 1);
    ASSERT_EQ(ht.find(0,1), 2);
}

TEST(edgeHashTableInsert, insert)
{
    vector<edge> elements = {edge(1, 2), edge(3, 4), edge(0, 1)};
    edge_hash_table ht(elements);

    ht.insert(5,6, 3);


    ASSERT_EQ(ht.size(), 4);
    ASSERT_EQ(ht.find(5,6), 3);
}

TEST(edgeHashTableErase, eraseEdgeExist)
{
    vector<edge> elements = {edge(1, 2), edge(3, 4), edge(0, 1)};
    edge_hash_table ht(elements);


    ASSERT_EQ(ht.size(), 3);
    ht.erase(2);

}
/*
TEST(edgeHashTableErase, eraseEdgeNotExist)
{
    vector<edge> elements = {edge(1, 2), edge(3, 4), edge(0, 1)};
    edge_hash_table ht(elements);

    int nonempty = count_nonempty_entrances(ht);
    
    ASSERT_EQ(nonempty, 3);
    ASSERT_NE(ht.get_key(0), EMPTY);
    ASSERT_NE(ht.get_key(1), EMPTY);
    ASSERT_NE(ht.get_key(2), EMPTY);

    ASSERT_THROW(ht.erase(4), logic_error);
}*/

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}