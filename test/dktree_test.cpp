#include <gtest/gtest.h>
#include "../include/dktree/dktree.hpp"
#include <iostream>

using d_tree = dynamic_ktree::dktree<2, bit_vector>;
using d_tree_it = dynamic_ktree::dk_edge_iterator<2, bit_vector>;

TEST(dktreeCreate, createEmpty) {
    d_tree tree(2);
    ASSERT_EQ(tree.get_number_edges(), 0);
}


TEST(dktreeAddLink, addLink0) {
    d_tree tree(2);
    tree.add_edge(0, 1);

    ASSERT_EQ(tree.get_number_edges(), 1);
}

TEST(dktreeAddLink, addLink) {
    d_tree tree(5);
    tree.add_edge(1, 2);
    tree.add_edge(1, 4);
    tree.add_edge(3, 0);
    tree.add_edge(1, 3);
    ASSERT_EQ(tree.get_number_edges(), 4);
}

TEST(dktreeAddLink, addSameLink) {
    d_tree tree(6);
    tree.add_edge(1, 2);
    tree.add_edge(1, 2);

    ASSERT_EQ(tree.get_number_edges(), 1);
}

TEST(dktreeContains, containsInC0) {
    d_tree tree(6);
    tree.add_edge(1, 2);

    ASSERT_TRUE(tree.contains(1, 2));
    ASSERT_FALSE(tree.contains(3, 4));
}

TEST(dktreeContain, containsInCs) {
    d_tree tree(5);
    tree.add_edge(1, 2);
    tree.add_edge(1, 4);
    tree.add_edge(3, 0);
    tree.add_edge(1, 3);

    ASSERT_TRUE(tree.contains(1, 2));
    ASSERT_TRUE(tree.contains(1, 4));
    ASSERT_TRUE(tree.contains(3, 0));
    ASSERT_TRUE(tree.contains(1, 3));
    ASSERT_FALSE(tree.contains(0, 0));
}


TEST(dktreeDelete, deleteItemC0) {
    d_tree tree(5);
    tree.add_edge(1, 2);
    ASSERT_EQ(tree.get_number_edges(), 1);

    tree.del_edge(1, 2);
    ASSERT_EQ(tree.get_number_edges(), 0);
}

TEST(dktreeDelete, deleteItem) {
    d_tree tree(4);
    tree.add_edge(1, 2);
    tree.add_edge(1, 3);
    tree.add_edge(3, 0);
    tree.add_edge(2, 3);
    tree.add_edge(3, 3);
    ASSERT_EQ(tree.get_number_edges(), 5);

    tree.del_edge(3, 3);
    ASSERT_EQ(tree.get_number_edges(), 4);

    tree.del_edge(1, 3);
    ASSERT_EQ(tree.get_number_edges(), 3);

    tree.del_edge(3, 0);
    ASSERT_EQ(tree.get_number_edges(), 2);

    tree.del_edge(1, 2);
    ASSERT_EQ(tree.get_number_edges(), 1);

    tree.del_edge(1, 2);
    ASSERT_EQ(tree.get_number_edges(), 1);

    tree.del_edge(2, 3);
    ASSERT_EQ(tree.get_number_edges(), 0);
}

TEST(dktreeDelete, listNeighbours) {
    d_tree tree(5);
    tree.add_edge(1, 2);
    tree.add_edge(1, 4);
    tree.add_edge(3, 0);
    tree.add_edge(3, 3);

    std::vector<int> neighbours = tree.list_neighbour(1);
    ASSERT_EQ(neighbours.size(), 2);
    ASSERT_EQ(neighbours[0], 2);
    ASSERT_EQ(neighbours[1], 4);

    neighbours = tree.list_neighbour(3);
    ASSERT_EQ(neighbours.size(), 2);
    ASSERT_EQ(neighbours[0], 0);
    ASSERT_EQ(neighbours[1], 3);
}

TEST(dktreeIterate, edge_iterate_empty) {
    d_tree empty_tree;
    ASSERT_TRUE(empty_tree.edge_begin() == empty_tree.edge_end());
}

TEST(dktreeIterate, edge_iterate) {
    d_tree tree(10);
    tree.add_edge(1, 2);
    tree.add_edge(1, 4);
    tree.add_edge(3, 1);
    tree.add_edge(3, 4);
    tree.add_edge(3, 9);
    tree.add_edge(5, 0);
    tree.add_edge(5, 1);
    tree.add_edge(6, 0);
    tree.add_edge(7, 0);
    tree.add_edge(9, 0);

    //increment operation
    auto it = tree.edge_begin();
    ASSERT_EQ(it.x(), 7);
    ASSERT_EQ(it.y(), 0);
    it++;
    ASSERT_EQ(it.x(), 9);
    ASSERT_EQ(it.y(), 0);
    it++;
    ASSERT_EQ(it.x(), 1);
    ASSERT_EQ(it.y(), 2);
    it++;
    ASSERT_EQ(it.x(), 1);
    ASSERT_EQ(it.y(), 4);
    it++;
    ASSERT_EQ(it.x(), 3);
    ASSERT_EQ(it.y(), 1);
    it++;
    ASSERT_EQ(it.x(), 3);
    ASSERT_EQ(it.y(), 4);
    it++;
    ASSERT_EQ(it.x(), 3);
    ASSERT_EQ(it.y(), 9);
    it++;
    ASSERT_EQ(it.x(), 5);
    ASSERT_EQ(it.y(), 0);
    it++;
    ASSERT_EQ(it.x(), 5);
    ASSERT_EQ(it.y(), 1);
    it++;
    ASSERT_EQ(it.x(), 6);
    ASSERT_EQ(it.y(), 0);

    it++;
    ASSERT_TRUE(it == tree.edge_end());
    it++;
    ASSERT_TRUE(it == tree.edge_end());

    d_tree_it a = tree.edge_begin();
    d_tree_it b = tree.edge_end();

    // SWAP operation
    swap(a, b);
    ASSERT_EQ(a, tree.edge_end());
    ASSERT_EQ(b, tree.edge_begin());
}

TEST(dktreeIterate, node_empty) {
    d_tree tree;
    ASSERT_EQ(*tree.node_begin(), *tree.node_end());
}

TEST(dktreeIterate, node_iterate) {
    d_tree tree(3);
    tree.add_edge(1, 2);
    tree.add_edge(1, 0);

    auto node_it = tree.node_begin();
    ASSERT_EQ(*node_it, 0);
    node_it++;
    ASSERT_EQ(*node_it, 1);
    node_it++;
    ASSERT_EQ(*node_it, 2);
    node_it++;
    ASSERT_EQ(node_it, tree.node_end());
    node_it++;
    ASSERT_EQ(node_it, tree.node_end());
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}