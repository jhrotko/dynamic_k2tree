#include <gtest/gtest.h>
#include "../include/dk2tree.hpp"

TEST(dktreeCreate, createEmpty)
{
    dk2tree tree(5);
    ASSERT_EQ(tree.get_max_level(), 1);
    ASSERT_EQ(tree.size(), 0);
}

TEST(dktreeAddLink, addLink0)
{
    dk2tree tree(5);
    tree.insert(1,2);

    ASSERT_EQ(tree.size(), 1);
    ASSERT_EQ(tree.n_edges(), 1);
}

TEST(dktreeAddLink, addLink)
{
    dk2tree tree(5);
    tree.insert(1,2);
    tree.insert(1,4);
    tree.insert(3,0);
    tree.insert(1,3);

    ASSERT_EQ(tree.size(), 4);
    ASSERT_EQ(tree.n_edges(), 4);
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}