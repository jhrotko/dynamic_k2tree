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

//TEST(k2treeExtended, marked_edges)
//{
//    typedef std::tuple<k2_tree_ns::idx_type, k2_tree_ns::idx_type> tuple;
//    std::vector<tuple> edges = {tuple(1, 2), tuple(0, 3), tuple(2, 2), tuple(3,3)};
//    k2_tree_extended tree(edges, 4);
//
//    ASSERT_EQ(tree.get_marked_edges(), 0);
//    ASSERT_TRUE(tree.mark_link_deleted(1,2));
//    ASSERT_EQ(tree.get_marked_edges(), 1);
//    ASSERT_EQ(tree.get_number_edges(), 3);
//
//    ASSERT_TRUE(tree.mark_link_deleted(3,3));
//    ASSERT_EQ(tree.get_marked_edges(), 2);
//    ASSERT_EQ(tree.get_number_edges(), 2);
//}

TEST(k2treeExtended, marked_edges_not_found)
{
    typedef std::tuple<k2_tree_ns::idx_type, k2_tree_ns::idx_type> tuple;
    std::vector<tuple> edges = {tuple(1, 2), tuple(0, 3), tuple(2, 2), tuple(3,3)};
    k2_tree_extended tree(edges, 4);

    ASSERT_EQ(tree.get_marked_edges(), 0);
    ASSERT_FALSE(tree.mark_link_deleted(0, 2));
    ASSERT_EQ(tree.get_marked_edges(), 0);
    ASSERT_EQ(tree.get_number_edges(), 4);
}

void check_t_l(k2_tree<2>  &tree, std::vector<unsigned> expected_t,
               vector<unsigned> expected_l)
{
    ASSERT_EQ(expected_t.size(), tree.get_t().size());
    ASSERT_EQ(expected_l.size(), tree.get_l().size());
    for (unsigned i = 0; i < expected_t.size(); i++)
        ASSERT_EQ(expected_t[i], tree.get_t().get_int(i, 1));
    for (unsigned i = 0; i < expected_l.size(); i++)
        ASSERT_EQ(expected_l[i], tree.get_l().get_int(i, 1));
}

TEST(k2treeExtended, union_operation)
{
    typedef std::tuple<k2_tree_ns::idx_type, k2_tree_ns::idx_type> tuple;
    int n_vertices = 4; 
    
    std::vector<tuple> edges = { tuple(0, 0), tuple(0, 1), tuple(1,1), tuple(2,2), tuple(2,3), tuple(3,2)};
    k2_tree_extended tree_A(edges, n_vertices);

    edges = {tuple(3,3)};
    k2_tree_extended tree_B(edges, n_vertices);

    k2_tree_extended  res_union = tree_A.unionOp(tree_B, n_vertices);
    check_t_l(res_union, {1, 0, 0, 1}, {1, 1, 0, 1, 1, 1, 1, 1});
}


int func_test(uint x, uint y)
{
    return x + y;
}

TEST(k2treeExtended, edgeIterator)
{
    //TODO: TEST ME 
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

//TEST(dktreeDelete, deleteItem)
//{
//    dk2tree tree(5);
//    tree.insert(1,2);
//    tree.print();
//    tree.insert(1,4);
//        tree.print();
//
//    tree.insert(3,0);
//        tree.print();
//
//    tree.insert(1,3);
//        tree.print();
//
//
//    ASSERT_EQ(tree.size(), 4);
//    tree.erase(1,3);
//    ASSERT_EQ(tree.size(), 3);
//    ASSERT_FALSE(tree.contains(1,3));
//}

TEST(dktreeListNeighbours, listNeighbours)
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