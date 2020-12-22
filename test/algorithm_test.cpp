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
        G *graph;
        Algorithm<G> *algo;

        Algorithm_Test() {}

        virtual ~Algorithm_Test() {}

        virtual void SetUp() {
            graph = new G(7); //undirect graph
            graph->add_edge(1, 2);
            graph->add_edge(1, 3);
            graph->add_edge(2, 5);
            graph->add_edge(2, 4);
            graph->add_edge(3, 5);
            graph->add_edge(3, 1);
            graph->add_edge(4, 5);
            graph->add_edge(4, 6);
            graph->add_edge(5, 6);
            graph->add_edge(6, 4);
            graph->add_edge(6, 5);
            graph->add_edge(5, 4);
            graph->add_edge(5, 3);
            graph->add_edge(5, 2);
            graph->add_edge(2, 1);

            algo = new Algorithm<G>(*graph);
        }

        virtual void TearDown() {
            delete graph;
            delete algo;
        }
    };

    typedef ::testing::Types<DKtree<2, bit_vector>> graph_implementations;

    TYPED_TEST_CASE(Algorithm_Test, graph_implementations);

    TYPED_TEST(Algorithm_Test, BFS) {
        vector<etype> path = this->algo->bfs(1);

        ASSERT_EQ(path[0], 1);
        ASSERT_EQ(path[1], 3);
        ASSERT_EQ(path[2], 2);
        ASSERT_EQ(path[3], 5);
        ASSERT_EQ(path[4], 4);
        ASSERT_EQ(path[5], 6);
    }

    TYPED_TEST(Algorithm_Test, DFS) {
        vector<etype> path = this->algo->dfs(1);

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

        this->algo->set_graph(simple_graph);
        int num_triangles = this->algo->count_triangles_dummy();

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

        this->algo->set_graph(star_graph);
        int num_triangles = this->algo->count_triangles_dummy();

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

        this->algo->set_graph(simple_graph);
        int num_triangles = this->algo->count_triangles_dummy_hash();

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

        this->algo->set_graph(star_graph);
        int num_triangles = this->algo->count_triangles_dummy_hash();

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

        this->algo->set_graph(star_graph);
        unsigned int num_triangles = this->algo->count_triangles();

        ASSERT_EQ(num_triangles, 3);

        float coefficient = this->algo->clustering_coefficient();
        ASSERT_TRUE(coefficient == 0.0125f);
    }

    TYPED_TEST(Algorithm_Test, pageRank_1) {
        TypeParam simple_graph(2);
        simple_graph.add_edge(0,1);
        simple_graph.add_edge(1,0);

        this->algo->set_graph(simple_graph);
        unordered_map<uint, double> pr_simple = this->algo->pageRank();
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

        this->algo->set_graph(simple_graph);
        unordered_map<uint, double> pr_simple = this->algo->pageRank();
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

        this->algo->set_graph(simple_graph);
        unordered_map<uint, double> pr_simple = this->algo->pageRank();
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
