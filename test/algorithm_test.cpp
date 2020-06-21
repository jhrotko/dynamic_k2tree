#include <gtest/gtest.h>

#include "../include/algorithm/Algorithm.hpp"
#include "../include/dktree/DKtree.hpp"
#include <iostream>

using namespace std;
using namespace dynamic_ktree;

namespace {

    template<class G, class E>
    struct Case
    {
        using graph_type = G;
        using edge_it_type = E;
    };

    template<typename G>
    class Algorithm_Test : public ::testing::Test {
    protected:
        G *graph_;

        Algorithm_Test() {}
        virtual ~Algorithm_Test() {}

        virtual void SetUp() {
            graph_ = new G(7); //undirect graph
            graph_->add_edge(1, 2);
            graph_->add_edge(1, 3);
            graph_->add_edge(2, 5);
            graph_->add_edge(2, 4);
            graph_->add_edge(3, 5);
            graph_->add_edge(3, 1);
            graph_->add_edge(4, 5);
            graph_->add_edge(4, 6);
            graph_->add_edge(5, 6);
            graph_->add_edge(6, 4);
            graph_->add_edge(6, 5);
            graph_->add_edge(5, 4);
            graph_->add_edge(5, 3);
            graph_->add_edge(5, 2);
            graph_->add_edge(2, 1);
        }

        virtual void TearDown() {
            delete graph_;
        }
    };

    typedef ::testing::Types<DKtree<2, bit_vector>> graph_implementations;

    TYPED_TEST_CASE(Algorithm_Test, graph_implementations);

    TYPED_TEST(Algorithm_Test, BFS) {
        vector<int> path = Algorithm<TypeParam>::bfs(*(this->graph_), 1);

        ASSERT_EQ(path[0], 1);
        ASSERT_EQ(path[1], 2);
        ASSERT_EQ(path[2], 3);
        ASSERT_EQ(path[3], 4);
        ASSERT_EQ(path[4], 5);
        ASSERT_EQ(path[5], 6);
    }

    TYPED_TEST(Algorithm_Test, DFS) {
        vector<int> path = Algorithm<TypeParam>::dfs(*(this->graph_), 1);

        ASSERT_EQ(path[0], 1);
        ASSERT_EQ(path[1], 2);
        ASSERT_EQ(path[2], 4);
        ASSERT_EQ(path[3], 6);
        ASSERT_EQ(path[4], 5);
        ASSERT_EQ(path[5], 3);
    }

//    TYPED_TEST(Algorithm_Test, Count_Triangles) {
//        int num_triangles = Algorithm<TypeParam>::count_triangles(*(this->graph_));
//
//        ASSERT_EQ(num_triangles, 2);
//    }
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
