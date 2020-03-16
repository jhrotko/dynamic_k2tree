#include <gtest/gtest.h>
#include "../include/dk2tree.hpp"

TEST(dktreeCreate, createEmpty)
{
    dk2tree tree(5);
    ASSERT_EQ(tree.get_max_level(), 1);
    ASSERT_EQ(tree.size(), 0);
}

TEST(dktreeAddLink, addLink)
{
    dk2tree tree(5);
    tree.insert(1,2);

    ASSERT_EQ(tree.size(), 1);
    ASSERT_EQ(tree.n_edges(), 1);
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}