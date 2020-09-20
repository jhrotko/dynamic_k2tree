#include <gtest/gtest.h>

#include "../include/algorithm/Algorithm.hpp"
#include "../include/dktree/DKtree.hpp"

using namespace std;
using namespace dynamic_ktree;

namespace {

    template<class G, class E>
    struct Case {
        using graph_type = G;
        using edge_it_type = E;
    };

    template<class G>
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
        vector<long unsigned int> path = Algorithm<TypeParam>::bfs(*(this->graph_), 1);

        ASSERT_EQ(path[0], 1);
        ASSERT_EQ(path[1], 2);
        ASSERT_EQ(path[2], 3);
        ASSERT_EQ(path[3], 4);
        ASSERT_EQ(path[4], 5);
        ASSERT_EQ(path[5], 6);
    }

    TYPED_TEST(Algorithm_Test, DFS) {
        vector<long unsigned int> path = Algorithm<TypeParam>::dfs(*(this->graph_), 1);

        ASSERT_EQ(path[0], 1);
        ASSERT_EQ(path[1], 2);
        ASSERT_EQ(path[2], 4);
        ASSERT_EQ(path[3], 6);
        ASSERT_EQ(path[4], 5);
        ASSERT_EQ(path[5], 3);
    }


    TYPED_TEST(Algorithm_Test, Count_Triangles_Basic_Simpl) {
        TypeParam simple_graph(4);
        simple_graph.add_edge(1, 2);
        simple_graph.add_edge(2, 3);
        simple_graph.add_edge(3, 1);
        simple_graph.add_edge(2, 1);
        simple_graph.add_edge(3, 2);
        simple_graph.add_edge(1, 3);

        int num_triangles = Algorithm<TypeParam>::count_triangles_dummy(simple_graph);

        ASSERT_EQ(num_triangles, 6);
    }

    TYPED_TEST(Algorithm_Test, Count_Triangles_Basic_Star) {
        TypeParam star_graph(7);
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

        int num_triangles = Algorithm<TypeParam>::count_triangles_dummy(star_graph);

        ASSERT_EQ(num_triangles, 18);
    }

    TYPED_TEST(Algorithm_Test, Count_Triangles_Basic_Hash) {
        TypeParam simple_graph(4);
        simple_graph.add_edge(1, 2);
        simple_graph.add_edge(2, 3);
        simple_graph.add_edge(3, 1);
        simple_graph.add_edge(2, 1);
        simple_graph.add_edge(3, 2);
        simple_graph.add_edge(1, 3);

        int num_triangles = Algorithm<TypeParam>::count_triangles_dummy_hash(simple_graph);

        ASSERT_EQ(num_triangles, 6);
    }

    TYPED_TEST(Algorithm_Test, Count_Triangles_Hash_Star) {
        TypeParam star_graph(7);
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

        int num_triangles = Algorithm<TypeParam>::count_triangles_dummy_hash(star_graph);

        ASSERT_EQ(num_triangles, 18);
    }

    TYPED_TEST(Algorithm_Test, count_triangles_heavy_hitters) {
        TypeParam star_graph(7);
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

        unsigned int num_triangles = Algorithm<TypeParam>::count_triangles(star_graph);

        ASSERT_EQ(num_triangles, 3);

        float coefficient = Algorithm<TypeParam>::clustering_coefficient(star_graph);
        ASSERT_TRUE(coefficient == 0.0125f);
    }

    TYPED_TEST(Algorithm_Test, pageRank_1) {
        TypeParam simple_graph(2);
        simple_graph.add_edge(0,1);
        simple_graph.add_edge(1,0);

        unordered_map<uint, double> pr_simple = Algorithm<TypeParam>::pageRank(simple_graph);
        unordered_map<uint, double> expected;
        expected[0] = .5f;
        expected[1] = .5f;
        ASSERT_EQ(expected, pr_simple);
    }

    TYPED_TEST(Algorithm_Test, pageRank_2) {
        TypeParam simple_graph(5);
        simple_graph.add_edge(0,1);
        simple_graph.add_edge(1,0);
        simple_graph.add_edge(2,1);
        simple_graph.add_edge(4,0);

        unordered_map<uint, double> pr_simple = Algorithm<TypeParam>::pageRank(simple_graph);
        unordered_map<uint, double> expected;
        expected[0] = 0.4625;
        expected[1] = 0.4625;
        expected[2] = 0.03750;
        expected[3] = 0.03750;
        expected[4] = 0.03750;

        for(size_t i = 0; i < 6; i++)
            ASSERT_NEAR(expected[i], pr_simple[i], 0.1);
    }

    TYPED_TEST(Algorithm_Test, pageRank_3) {
        TypeParam simple_graph(6);
        simple_graph.add_edge(1,2);
        simple_graph.add_edge(1,3);
        simple_graph.add_edge(2,3);
        simple_graph.add_edge(2,4);
        simple_graph.add_edge(2,5);
        simple_graph.add_edge(3,2);
        simple_graph.add_edge(4,1);
        simple_graph.add_edge(5,2);


        unordered_map<uint, double> pr_simple = Algorithm<TypeParam>::pageRank(simple_graph);
        unordered_map<uint, double> expected;
        expected[0] = 0.0291262;
        expected[1] = 0.142472;
        expected[2] = 0.367838;
        expected[3] = 0.193899;
        expected[4] = 0.133348;
        expected[5] = 0.133348;
        for (int i = 0; i < 6; ++i) {
            ASSERT_NEAR(expected[i], pr_simple[i], 0.00001);
        }
    }

}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
