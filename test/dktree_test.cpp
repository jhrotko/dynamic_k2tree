#include <gtest/gtest.h>
#include <iostream>

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

#include "../include/dktree/DKtree.hpp"
#include "sdsl/k2_tree.hpp"

using d_tree = dynamic_ktree::DKtree<2, bit_vector>;
using k_tree = sdsl::k2_tree<2>;

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

    std::vector<etype> neighbours = tree.list_neighbour(1);
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
    ASSERT_EQ(it.x(), 3);
    ASSERT_EQ(it.y(), 1);
    it++;
    ASSERT_EQ(it.x(), 1);
    ASSERT_EQ(it.y(), 4);
    it++;
    ASSERT_EQ(it.x(), 3);
    ASSERT_EQ(it.y(), 4);
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
    ASSERT_EQ(it.x(), 3);
    ASSERT_EQ(it.y(), 9);

    it++;
    ASSERT_TRUE(it == tree.edge_end());
    it++;
    ASSERT_TRUE(it == tree.edge_end());

    auto a = tree.edge_begin();
    auto b = tree.edge_end();

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
    ASSERT_EQ(*node_it, 1);
    node_it++;
    ASSERT_EQ(*node_it, 0);
    node_it++;
    ASSERT_EQ(*node_it, 2);
    node_it++;
    ASSERT_EQ(*node_it, *tree.node_end());
    node_it++;
    ASSERT_EQ(*node_it, *tree.node_end());
}

TEST(dktreeIterate, neighbour_iterator_empty) {
    d_tree tree(3);
    tree.add_edge(1, 2);
    tree.add_edge(1, 1);
    tree.add_edge(2, 0);

    ASSERT_EQ(*tree.neighbour_begin(0), *tree.neighbour_end());
}

TEST(dktreeIterate, neightbour_iterator) {
    d_tree tree(3);
    tree.add_edge(1, 2);
    tree.add_edge(1, 1);
    tree.add_edge(2, 0);

    auto neighbour_it = tree.neighbour_begin(1);
    ASSERT_EQ(*neighbour_it, 1);
    neighbour_it++;
    ASSERT_EQ(*neighbour_it, 2);
    neighbour_it++;
    ASSERT_EQ(*neighbour_it, *tree.neighbour_end());
    neighbour_it++;
    ASSERT_EQ(*neighbour_it, *tree.neighbour_end());

    neighbour_it = tree.neighbour_begin(2);
    ASSERT_EQ(*neighbour_it, 0);
    neighbour_it++;
    ASSERT_EQ(*neighbour_it, *tree.neighbour_end());

    neighbour_it = tree.neighbour_begin(0);
    ASSERT_EQ(*neighbour_it, *tree.neighbour_end());

}

TEST(dktreeIterate, neighbour_iterator_other) {
    d_tree simple_graph(4);
    simple_graph.add_edge(1, 2);
    simple_graph.add_edge(2, 3);
    simple_graph.add_edge(3, 1);
    simple_graph.add_edge(2, 1);
    simple_graph.add_edge(3, 2);
    simple_graph.add_edge(1, 3);

    auto neighbour_it = simple_graph.neighbour_begin(1);
    ASSERT_EQ(*neighbour_it, 3);
    neighbour_it++;
    ASSERT_EQ(*neighbour_it, 2);
    neighbour_it++;
    ASSERT_EQ(*neighbour_it, *simple_graph.neighbour_end());

    neighbour_it = simple_graph.neighbour_begin(2);
    ASSERT_EQ(*neighbour_it, 1);
    neighbour_it++;
    ASSERT_EQ(*neighbour_it, 3);
    neighbour_it++;
    ASSERT_EQ(*neighbour_it, *simple_graph.neighbour_end());

    neighbour_it = simple_graph.neighbour_begin(3);
    ASSERT_EQ(*neighbour_it, 2);
    neighbour_it++;
    ASSERT_EQ(*neighbour_it, 1);
    neighbour_it++;
    ASSERT_EQ(*neighbour_it, *simple_graph.neighbour_end());
}

TEST(dktreeIterate, neighbour_iterator_star) {
    d_tree star_graph(7);
    star_graph.add_edge(1, 2);
    star_graph.add_edge(1, 3);
    star_graph.add_edge(1, 4);
    star_graph.add_edge(1, 5);
    star_graph.add_edge(1, 6);

    star_graph.add_edge(2, 1);
    star_graph.add_edge(2, 4);

    star_graph.add_edge(3, 1);
    star_graph.add_edge(3, 5);

    star_graph.add_edge(4, 1);
    star_graph.add_edge(4, 2);

    star_graph.add_edge(5, 1);
    star_graph.add_edge(5, 3);
    star_graph.add_edge(5, 6);

    star_graph.add_edge(6, 1);
    star_graph.add_edge(6, 5);

    vector<int> list_neighbours_star;
    for (auto neigh_star_it = star_graph.neighbour_begin(1);
         neigh_star_it != star_graph.neighbour_end(); neigh_star_it++) {
        list_neighbours_star.push_back(*neigh_star_it);
    }

    vector<int> expected_list_neighbour = {2, 3, 4, 5, 6};
    for (int e: expected_list_neighbour) {
        ASSERT_TRUE(std::find(list_neighbours_star.begin(), list_neighbours_star.end(), e)
                    != list_neighbours_star.end());
    }
}

TEST(SerializationTest, Container0SerializaAndLoad)
{
    std::stringstream ss;
    boost::archive::text_oarchive oa(ss);

    dynamic_ktree::Container_0 serialize_c0(1000);
    serialize_c0.insert(0, 1, 0);
    serialize_c0.insert(6, 3, 1);
    serialize_c0.insert(9, 8, 2);
    serialize_c0.insert(4, 2, 3);
    serialize_c0.insert(0, 0, 4);
    serialize_c0.insert(5, 3, 5);
    oa << serialize_c0;

    dynamic_ktree::Container_0 load_c0;
    boost::archive::text_iarchive iar(ss); //exception
    iar >> load_c0;

    ASSERT_EQ(serialize_c0, load_c0);
}

TEST(SerializationTest, DKtreeSerializaAndLoad)
{
    std::stringstream ss;

    d_tree serialize_dktree(50);
    serialize_dktree.add_edge(0, 1);
    serialize_dktree.add_edge(6, 3);
    serialize_dktree.add_edge(9, 8);
    serialize_dktree.add_edge(4, 2);
    serialize_dktree.add_edge(0, 0);
    serialize_dktree.add_edge(5, 3);

    serialize_dktree.serialize(ss);

    d_tree load_dktree;
    load_dktree.load(ss);

    ASSERT_EQ(serialize_dktree, load_dktree);

}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}