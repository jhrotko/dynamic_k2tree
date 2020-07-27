#include <gtest/gtest.h>
#include "test_utils.hpp"
#include "../include/dktree/DKtree.hpp"
#include <iostream>
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
    ASSERT_EQ(*node_it, 0);
    node_it++;
    ASSERT_EQ(*node_it, 1);
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

TEST(dktreeEmpty, memoryEmpty) {
    d_tree empty_graph;
    ASSERT_ANY_THROW(empty_graph.add_edge(1,2));
}

TEST(dktreeSerialize, serializeAndLoad) {
    d_tree graph(7);
    graph.add_edge(1, 2);
    graph.add_edge(1, 3);
    graph.add_edge(1, 4);
    graph.add_edge(1, 5);
    graph.add_edge(1, 6);
    graph.add_edge(2, 1);
    graph.add_edge(2, 4);
    graph.add_edge(3, 1);
    graph.add_edge(3, 5);
    graph.add_edge(4, 1);
    graph.add_edge(4, 2);
    graph.add_edge(5, 1);
    graph.add_edge(5, 3);
    graph.add_edge(5, 6);
    graph.add_edge(6, 1);
    graph.add_edge(6, 5);
    d_tree unserialized_tree;
    std::stringstream ss;

    //Serialize
    graph.serialize(ss);

    //Loads
    unserialized_tree.load(ss);

    //Lets check
    ASSERT_EQ(graph.get_number_nodes(), unserialized_tree.get_number_nodes());
    ASSERT_EQ(graph.get_number_edges(), unserialized_tree.get_number_edges());
    //Compare C0
    ASSERT_EQ(graph.first_container().size(), unserialized_tree.first_container().size());
    for (int i = 0; i < graph.first_container().size(); ++i) {
        ASSERT_EQ(graph.first_container().elements[i], unserialized_tree.first_container().elements[i]);
    }
//
//    for (int i = 0; i < graph.first_container().edge_free.size(); ++i) {
//        ASSERT_EQ(graph.first_container().edge_free[i], unserialized_tree.first_container().edge_free[i]);
//    }
//
//    auto ht_begin = graph.first_container().edge_lst.cbegin();
//    auto ht_end = graph.first_container().edge_lst.cend();
//    auto ht_un_begin = unserialized_tree.first_container().edge_lst.cbegin();
//    for(; ht_begin != ht_end; ht_begin++, ht_un_begin++) {
//        ASSERT_EQ(ht_begin->first.x(), ht_un_begin->first.x());
//        ASSERT_EQ(ht_begin->first.y(), ht_un_begin->first.y());
//        ASSERT_EQ(ht_begin->second, ht_un_begin->second);
//    }
//
//    for (int j = 0; j < graph.first_container().adj_lst.size(); ++j) {
//        ASSERT_EQ(graph.first_container().adj_lst[j], unserialized_tree.first_container().adj_lst[j]);
//    }


//    //Compare k_collections
//    ASSERT_EQ(graph.get_max_r(), unserialized_tree.get_max_r());
//    for (size_t l = 0; l <= graph.get_max_r(); l++) {
//        if(graph.k_collections()[l] == nullptr){
//            ASSERT_TRUE(unserialized_tree.k_collections()[l] == nullptr);
//        } else {
//            ASSERT_EQ(graph.k_collections()[l]->t().size(), unserialized_tree.k_collections()[l]->t().size());
//            ASSERT_EQ(graph.k_collections()[l]->l().size(), unserialized_tree.k_collections()[l]->l().size());
//            for (unsigned i = 0; i < graph.k_collections()[l]->t().size(); i++)
//                ASSERT_EQ(graph.k_collections()[l]->t().get_int(i, 1), unserialized_tree.k_collections()[l]->t().get_int(i, 1));
//
//            for (unsigned i = 0; i < graph.k_collections()[l]->l().size(); i++)
//                ASSERT_EQ(graph.k_collections()[l]->l().get_int(i, 1), unserialized_tree.k_collections()[l]->l().get_int(i, 1));
//        }
//    }
}


int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}