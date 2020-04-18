#include <gtest/gtest.h>
#include <tuple>
#include <vector>
#include "../include/k_tree_extended.hpp"

using namespace std;

typedef k_tree_extended<2> k_tree;
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

TEST(ktreeExtended, iterate)
{
    typedef tuple<k2_tree_ns::idx_type, k2_tree_ns::idx_type> tuple;
    vector<tuple> edges = {tuple(1, 2), tuple(0, 3), tuple(2, 2)};
    k_tree tree(edges, 4);

}

//class testClass {
//public:
//    int some_tribute;
//    char other_tribute;
//    testClass() {}
//    testClass(int a, char b) : some_tribute(a), other_tribute(b) {}
//    inline bool operator==(const testClass& other) const {
//        return this->some_tribute == other.some_tribute && this->other_tribute == other.other_tribute;
//    }
//
//    inline bool operator<(const testClass &other) const {
//        return this->some_tribute < other.some_tribute;
//    }
//
//    friend ostream& operator<< (ostream & os, const testClass& c) {
//        os << "some: " << c.some_tribute << " other: " << c.other_tribute << endl;
//        return os;
//    }
//};


//TEST(ktreeItem, create)
//{
//typedef tuple<testClass, testClass> tuple;
//    testClass a(1, 'v');
//    testClass b(2, 'y');
//    testClass c(1000, 'x');
//
//    vector<tuple> edges = {tuple(a,a), tuple(a, b), tuple(a, c)};
//    k_tree_item<testClass> tree(edges, 4);
//
//    //insert
//    // erase
//    // union
//}
//
//TEST(ktreeItem, neigh) {
//    typedef tuple<testClass, testClass> tuple;
//    testClass a(1, 'v');
//    testClass b(2, 'y');
//    testClass c(1000, 'x');
//
//    vector<tuple> edges = {tuple(a,a), tuple(a, b), tuple(a, c)};
//    k_tree_item<testClass> tree(edges, 4);
//
//    //neigh
//    vector<testClass> neigh = tree.neigh(a);
//    ASSERT_EQ(neigh.size(), 3);
//    ASSERT_EQ(neigh[0], a);
//    ASSERT_EQ(neigh[1], b);
//    ASSERT_EQ(neigh[2], c);
//    ASSERT_THROW(tree.neigh(testClass(5, 'p')),  logic_error);
//}
//
//TEST(ktreeItem, adj) {
//    typedef tuple<testClass, testClass> tuple;
//    testClass a(1, 'v');
//    testClass b(2, 'y');
//    testClass c(1000, 'x');
//
//    vector<tuple> edges = {tuple(a,a), tuple(a, b), tuple(a, c)};
//    k_tree_item<testClass> tree(edges, 4);
//
//    //adj
//    ASSERT_TRUE(    tree.adj(a, b));
//    ASSERT_FALSE(    tree.adj(c,a));
//    ASSERT_THROW(tree.adj(a, testClass(5, 'p')),  logic_error);
//}
//
//TEST(ktreeItem, unionOp) {
//    typedef tuple<testClass, testClass> tuple;
//    testClass a(1, 'v');
//    testClass b(2, 'y');
//    testClass c(1000, 'x');
//
//    vector<tuple> edges = {tuple(a,a), tuple(a, b), tuple(a, c)};
//    k_tree_item<testClass> tree(edges, 4);
//
//    //adj
//    ASSERT_TRUE(    tree.adj(a, b));
//    ASSERT_FALSE(    tree.adj(c,a));
//    ASSERT_THROW(tree.insert(testClass(5, 'p')),  logic_error);
//}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
