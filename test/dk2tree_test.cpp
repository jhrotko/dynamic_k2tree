#include <gtest/gtest.h>
#include "../include/dk2tree.hpp"

/////////////////**** DYNAMIC TREE ****/////////////////

TEST(dktreeCreate, createEmpty)
{
    dk2tree tree(2);
    ASSERT_EQ(tree.size(), 0);
}

TEST(dktreeAddLink, addLink0)
{
    dk2tree tree(2);
    tree.insert(0, 1);

    ASSERT_EQ(tree.size(), 1);
}

TEST(dktreeAddLink, addLink)
{
    dk2tree tree(5);
    tree.insert(1, 2);
    tree.insert(1, 4);
    tree.insert(3, 0);
    tree.insert(1, 3);
    ASSERT_EQ(tree.size(), 4);
}

TEST(dktreeAddLink, addSameLink)
{
    dk2tree tree(6);
    tree.insert(1, 2);
    tree.insert(1, 2);

    ASSERT_EQ(tree.size(), 1);
}

TEST(dktreeContains, containsInC0)
{
    dk2tree tree(6);
    tree.insert(1, 2);

    ASSERT_TRUE(tree.contains(1, 2));
    ASSERT_FALSE(tree.contains(3, 4));
}

TEST(dktreeContain, containsInCs)
{
    dk2tree tree(5);
    tree.insert(1, 2);
    tree.insert(1, 4);
    tree.insert(3, 0);
    tree.insert(1, 3);

    ASSERT_TRUE(tree.contains(1, 2));
    ASSERT_TRUE(tree.contains(1, 4));
    ASSERT_TRUE(tree.contains(3, 0));
    ASSERT_TRUE(tree.contains(1, 3));
    ASSERT_FALSE(tree.contains(0, 0));
}

TEST(dktreeDelete, deleteItemC0)
{
    dk2tree tree(5);
    tree.insert(1, 2);
    ASSERT_EQ(tree.size(), 1);

    tree.erase(1, 2);
    ASSERT_EQ(tree.size(), 0);
}

TEST(dktreeDelete, deleteItem)
{
    dk2tree tree(4);
    tree.insert(1,2);
    tree.insert(1,3);
    tree.insert(3,0);
    tree.insert(2,3);
    tree.insert(3,3);
    ASSERT_EQ(tree.size(), 5);

    tree.erase(3,3);
    ASSERT_EQ(tree.size(), 4);

    tree.erase(1,3);
    ASSERT_EQ(tree.size(), 3);

    tree.erase(3,0);
    ASSERT_EQ(tree.size(), 2);

    tree.erase(1,2);
    ASSERT_EQ(tree.size(), 1);

    tree.erase(1,2);
    ASSERT_EQ(tree.size(), 1);

    tree.erase(2,3);
    ASSERT_EQ(tree.size(), 0);
}

TEST(dktreeDelete, listNeighbours)
{
    dk2tree tree(5);
    tree.insert(1, 2);
    tree.insert(1, 4);
    tree.insert(3, 0);
    tree.insert(3, 3);

    std::vector<int> neighbours = tree.list_neighbour(1);
    ASSERT_EQ(neighbours.size(), 2);
    ASSERT_EQ(neighbours[0], 2);
    ASSERT_EQ(neighbours[1], 4);

    neighbours = tree.list_neighbour(3);
    ASSERT_EQ(neighbours.size(), 2);
    ASSERT_EQ(neighbours[0], 0);
    ASSERT_EQ(neighbours[1], 3);
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}