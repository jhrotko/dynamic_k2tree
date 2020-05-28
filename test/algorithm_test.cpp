#include <gtest/gtest.h>

#include "../include/algorithm/Algorithm.hpp"
#include "../include/dktree/dktree.hpp"
#include <iostream>

using namespace std;

namespace {

    template<class G>
    class Algorithm_Test : public ::testing::Test {
    protected:
        G *graph_;

        Algorithm_Test() {}
        virtual ~Algorithm_Test() {}

        virtual void SetUp() {
            graph_ = new G(7);
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

    typedef ::testing::Types<dynamic_ktree::dktree<2, bit_vector>> graph_implementations;

    TYPED_TEST_CASE(Algorithm_Test, graph_implementations);

    TYPED_TEST(Algorithm_Test, BFS) {
        vector<int> path = Algorithm::bfs(*(this->graph_), 1);

        ASSERT_TRUE(path[0] == 1);
        ASSERT_TRUE(path[1] == 2);
        ASSERT_TRUE(path[2] == 3);
        ASSERT_TRUE(path[3] == 4);
        ASSERT_TRUE(path[4] == 5);
        ASSERT_TRUE(path[5] == 6);
    }

    TYPED_TEST(Algorithm_Test, DFS) {
        vector<int> path = Algorithm::dfs(*(this->graph_), 1);

        ASSERT_TRUE(path[0] == 1);
        ASSERT_TRUE(path[1] == 2);
        ASSERT_TRUE(path[2] == 4);
        ASSERT_TRUE(path[3] == 6);
        ASSERT_TRUE(path[4] == 5);
        ASSERT_TRUE(path[5] == 3);
    }
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
