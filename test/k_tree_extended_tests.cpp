#include <gtest/gtest.h>
#include <tuple>
#include <vector>
#include "../include/k_tree_extended.hpp"

using namespace std;

typedef k_tree_extended<> k_tree;
TEST(ktreeExtended, createEmpty)
{
    k_tree tree(2);

    ASSERT_EQ(tree.get_number_edges(), 0);
}

TEST(ktreeExtended, createWithEdgeList)
{
    typedef tuple<k2_tree_ns::idx_type, k2_tree_ns::idx_type> tuple;
    vector<tuple> edges = {tuple(1, 2), tuple(0, 3), tuple(2, 2)};
    k_tree tree(edges, 4);

    ASSERT_EQ(tree.get_number_edges(), 3);
}

TEST(ktreeExtended, createWithEdgeListThrowError)
{
    typedef tuple<k2_tree_ns::idx_type, k2_tree_ns::idx_type> tuple;
    vector<tuple> edges = {tuple(1, 2), tuple(0, 6), tuple(2, 2)};

    ASSERT_THROW(k_tree(edges, 4), logic_error);
}

void check_t_l(k2_tree<2>  &tree, vector<unsigned> expected_t,
               vector<unsigned> expected_l)
{
    ASSERT_EQ(expected_t.size(), tree.get_t().size());
    ASSERT_EQ(expected_l.size(), tree.get_l().size());
    for (unsigned i = 0; i < expected_t.size(); i++)
        ASSERT_EQ(expected_t[i], tree.get_t().get_int(i, 1));
    for (unsigned i = 0; i < expected_l.size(); i++)
        ASSERT_EQ(expected_l[i], tree.get_l().get_int(i, 1));
}

TEST(ktreeExtended, union_operation)
{
    typedef tuple<k2_tree_ns::idx_type, k2_tree_ns::idx_type> tuple;
    int n_vertices = 4;

    vector<tuple> edges = { tuple(0, 0), tuple(0, 1), tuple(1,1), tuple(2,2), tuple(2,3), tuple(3,2)};
    k_tree tree_A(edges, n_vertices);

    edges = {tuple(3,3)};
    k_tree tree_B(edges, n_vertices);

    k_tree  res_union = tree_A.unionOp(tree_B, n_vertices);
    check_t_l(res_union, {1, 0, 0, 1}, {1, 1, 0, 1, 1, 1, 1, 1});
}


int func_test(uint x, uint y)
{
    return x + y;
}

TEST(ktreeExtended, edgeIterator)
{
    //TODO: TEST ME
    typedef tuple<k2_tree_ns::idx_type, k2_tree_ns::idx_type> tuple;
    vector<tuple> edges = {tuple(1, 2), tuple(0, 3), tuple(2, 2)};
    k_tree tree(edges, 4);

    tree.edge_iterator(func_test);
}

//class testClass {
//private:
//    int some_tribute;
//    char other_tribute;
//public:
//    testClass(int a, char b) : some_tribute(a), other_tribute(b) {}
//};
//
//TEST(ktreeExtended, item)
//{
//    typedef tuple<testClass, testClass> tuple;
//    testClass a(1, 'v');
//    testClass b(1, 'v');
//    testClass c(1, 'v');
//
//    vector<tuple> edges = {tuple(a,a), tuple(b,a), tuple(c,b)};
//    k_tree_extended<testClass> tree(edges, 4);
//}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
