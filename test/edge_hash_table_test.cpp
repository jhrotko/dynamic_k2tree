#include "../include/edge_hash_table.hpp"
#include <gtest/gtest.h>
#include <iostream>

using namespace std;

// TEST(edgeHashTableInsert, insertError ) { 
//     vector<edge> elements = { edge(1,2), edge(3,4), edge(0,1)};
//     edge_hash_table ht(2, elements);

//     ASSERT_THROW(ht.insert(6), logic_error);
// }

TEST(edgeHashTableInsert, insertHappyPath ) { 
    vector<edge> elements = { edge(1,2), edge(3,4), edge(0,1) };
    edge_hash_table ht(elements);

    int nonempty = 0;
    for (size_t i = 0; i < ht.get_table().size(); i++)
        ht.get_table()[i] != EMPTY ? nonempty++ : nonempty;
    
    ASSERT_EQ(nonempty, 3);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}