#include <gtest/gtest.h>
#include "../include/dk2tree.hpp"

TEST(k2treeExtended, createEmpty)
{
    k2_tree_extended tree(2);

    ASSERT_EQ(tree.get_k(), 2);
}

TEST(k2treeExtended, createWithEdgeList)
{
    typedef std::tuple<k2_tree_ns::idx_type, k2_tree_ns::idx_type> tuple;
    std::vector<tuple> edges = {tuple(1, 2), tuple(0, 3), tuple(2, 2)};
    k2_tree_extended tree(edges, 4);

    ASSERT_EQ(tree.get_number_edges(), 3);
}

TEST(k2treeExtended, createWithEdgeListThrowError)
{
    typedef std::tuple<k2_tree_ns::idx_type, k2_tree_ns::idx_type> tuple;
    std::vector<tuple> edges = {tuple(1, 2), tuple(0, 6), tuple(2, 2)};

    ASSERT_THROW(k2_tree_extended(edges, 4), std::logic_error);
}

TEST(k2treeExtended, checkLinkRecursive)
{
    typedef std::tuple<k2_tree_ns::idx_type, k2_tree_ns::idx_type> tuple;
    std::vector<tuple> edges = {tuple(1, 2), tuple(0, 3), tuple(2, 2)};
    k2_tree_extended tree(edges, 4);

    ASSERT_TRUE(tree.check_link(1, 2));
    ASSERT_FALSE(tree.check_link(1, 1));
    ASSERT_FALSE(tree.check_link(100, 0));
}

TEST(k2treeExtended, marked_edges)
{
    typedef std::tuple<k2_tree_ns::idx_type, k2_tree_ns::idx_type> tuple;
    std::vector<tuple> edges = {tuple(1, 2), tuple(0, 3), tuple(2, 2)};
    k2_tree_extended tree(edges, 4);

    ASSERT_EQ(tree.get_marked_edges(), 0);
    ASSERT_TRUE(tree.mark_link_deleted(1, 2));
    ASSERT_EQ(tree.get_marked_edges(), 1);
}

int func_test(uint x, uint y)
{
    return x + y;
}

TEST(k2treeExtended, edgeIterator)
{
    typedef std::tuple<k2_tree_ns::idx_type, k2_tree_ns::idx_type> tuple;
    std::vector<tuple> edges = {tuple(1, 2), tuple(0, 3), tuple(2, 2)};
    k2_tree_extended tree(edges, 4);

    tree.edge_iterator(func_test);
}

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
    dk2tree tree(5);
    tree.insert(1,2);
    tree.insert(1,4);
    tree.insert(3,0);
    tree.insert(1,3);

    ASSERT_EQ(tree.size(), 4);
    tree.erase(1,3);    
    ASSERT_EQ(tree.size(), 3);
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