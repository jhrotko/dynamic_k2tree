#include "../include/edge_hash_table.hpp"
#include <gtest/gtest.h>
#include <iostream>

using namespace std;

/////////////////////// AUX FUNCTIONS ///////////////////////
int count_nonempty_entrances(edge_hash_table ht)
{
    int nonempty = 0;
    std::vector<int> tbl = ht.get_table();
    for (size_t i = 0; i < tbl.size(); i++){
        // cout << "tbl[" << i << "]: " << tbl[i] << endl;
        tbl[i] != EMPTY ? nonempty++ : nonempty;
    }

    return nonempty;
}
////////////////////////////////////////////////////////////

TEST(edgeHashTableInsert, createHashTable)
{
    vector<edge> elements = {edge(1, 2), edge(3, 4), edge(0, 1)};
    edge_hash_table ht(elements);

    int nonempty = count_nonempty_entrances(ht);
    
    ASSERT_EQ(nonempty, 3);
    ASSERT_NE(ht.get_key(0), EMPTY);
    ASSERT_NE(ht.get_key(1), EMPTY);
    ASSERT_NE(ht.get_key(2), EMPTY);
}

TEST(edgeHashTableInsert, insertResize)
{
    vector<edge> elements = {edge(1, 2), edge(3, 4), edge(0, 1)};
    edge_hash_table ht(elements);

    ht.insert(edge(5, 8));
    ht.insert(edge(0, 5));

    int nonempty = count_nonempty_entrances(ht);

    ASSERT_EQ(nonempty, 5);
    ASSERT_NE(ht.get_key(0), EMPTY);
    ASSERT_NE(ht.get_key(1), EMPTY);
    ASSERT_NE(ht.get_key(2), EMPTY);
    ASSERT_NE(ht.get_key(3), EMPTY);
    ASSERT_NE(ht.get_key(4), EMPTY);
}

TEST(edgeHashTableFind, find)
{
    vector<edge> elements = {edge(1, 2), edge(3, 4), edge(0, 1)};
    edge_hash_table ht(elements);

    ASSERT_NE(ht.find(edge(1,2)), -1);
}

TEST(edgeHashTableFind, findNot)
{
    vector<edge> elements = {edge(1, 2), edge(3, 4), edge(0, 1)};
    edge_hash_table ht(elements);

    ASSERT_EQ(ht.find(edge(1,1)), -1);
}

TEST(edgeHashTableErase, eraseEdge)
{
    vector<edge> elements = {edge(1, 2), edge(3, 4), edge(0, 1)};
    edge_hash_table ht(elements);

    int nonempty = count_nonempty_entrances(ht);
    
    ASSERT_EQ(nonempty, 3);
    ASSERT_NE(ht.get_key(0), EMPTY);
    ASSERT_NE(ht.get_key(1), EMPTY);
    ASSERT_NE(ht.get_key(2), EMPTY);

    ht.erase(2);

    ASSERT_EQ(ht.get_key(2), EMPTY);
}

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
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}