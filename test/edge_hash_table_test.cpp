#include "../include/edge_hash_table.hpp"
#include <gtest/gtest.h>
#include <iostream>

using namespace std;

TEST(edgeHashTableInsert, insertHappyPath ) { 
    vector<edge> elements = { edge(1,2), edge(3,4), edge(0,1) };
    edge_hash_table ht(elements);

    int nonempty = 0;
    std::vector<int> tbl = ht.get_table();
    for (size_t i = 0; i < tbl.size(); i++) 
        tbl[i] != EMPTY ? nonempty++ : nonempty;
    
    ASSERT_EQ(nonempty, 3);
    ASSERT_NE(ht.get_key(0), -1);
    ASSERT_NE(ht.get_key(1), -1);
    ASSERT_NE(ht.get_key(2), -1);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}