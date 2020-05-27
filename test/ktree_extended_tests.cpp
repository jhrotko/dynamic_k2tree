#include <gtest/gtest.h>
#include <tuple>
#include <vector>
#include "../include/dktree/ktree_extended.hpp"

using namespace std;

typedef ktree_extended<2> k_tree;
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

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
