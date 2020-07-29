#include "../include/dktree/EdgeHashTable.hpp"
#include <boost/archive/text_oarchive.hpp>
#include <gtest/gtest.h>

using namespace std;
using edge = tuple<etype, etype>;

TEST(edgeHashTableInsert, createHashTable)
{
    vector<edge> elements = {edge(1, 2), edge(3, 4), edge(0, 1)};
    EdgeHashTable ht(elements);
    
    ASSERT_EQ(ht.size(), 3);
}


TEST(edgeHashTableFind, find)
{
    vector<edge> elements = {edge(1, 2), edge(3, 4), edge(0, 1)};
    EdgeHashTable ht(elements);

    ASSERT_EQ(ht.find(1,1), -1);
    ASSERT_EQ(ht.find(1,2), 0);
    ASSERT_EQ(ht.find(3,4), 1);
    ASSERT_EQ(ht.find(0,1), 2);
}

TEST(edgeHashTableInsert, insert)
{
    vector<edge> elements = {edge(1, 2), edge(3, 4), edge(0, 1)};
    EdgeHashTable ht(elements);

    ht.insert(5,6, 3);

    ASSERT_EQ(ht.size(), 4);
    ASSERT_EQ(ht.find(5,6), 3);
}

TEST(edgeHashTableErase, eraseedgeExist)
{
    vector<edge> elements = {edge(1, 2), edge(3, 4), edge(0, 1)};
    EdgeHashTable ht(elements);

    ASSERT_EQ(ht.size(), 3);
    ht.erase(1,2);
    ASSERT_EQ(ht.size(), 2);
    ht.erase(0,0);
    ASSERT_EQ(ht.size(), 2);
}

TEST(edgeHashTableErase, clear)
{
    vector<edge> elements = {edge(1, 2), edge(3, 5), edge(0, 1)};
    EdgeHashTable ht(elements);

    ASSERT_EQ(ht.size(), 3);
    ht.clear();
    ASSERT_EQ(ht.size(), 0);
}

TEST(edgeHashTableSerialization, SerializaAndLoad)
{
    EdgeHashTable unserializedHt;

    std::stringstream ss;
    {
        boost::archive::text_oarchive oa(ss);

    vector<edge> elements = {edge(1, 2), edge(3, 5), edge(0, 1)};
    EdgeHashTable ht(elements);
        oa << ht;
    }

//    ASSERT_EQ(ht, unserializedHt);
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}