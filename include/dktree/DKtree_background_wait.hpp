#ifndef __D_K_TREE_BACKGROUND_WAIT__
#define __D_K_TREE_BACKGROUND_WAIT__

#include <array>
#include <tuple>
#include <memory>
#include <iostream>
#include <thread>
#include "utils.hpp"

#include <sdsl/k2_tree.hpp>
#include "Container0.hpp"
#include "Container0NeighIterator.hpp"
#include "DKtreeEdgeIterator.hpp"
#include "DKtreeNodeIterator.hpp"
#include "DKtreeNeighbourIterator.hpp"
#include "../graph/Graph.hpp"

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <sys/stat.h>
#include <experimental/filesystem>
#include <cstdio>
#include <mutex>
#include <condition_variable>

using namespace sdsl;
using namespace std;
using namespace k2_tree_ns;


std::condition_variable cv_w;
std::mutex mtx_lck_w;
std::atomic<bool> needs_work_w, run_w;


template<class dktree_background, class k_tree>
void union_collection_wait(dktree_background &graph) {
    while (run_w) {
        {
//                cout << "ok" << endl;
            std::unique_lock<std::mutex> lk(mtx_lck_w);
            while (!needs_work_w && run_w) {
                cv_w.wait(lk);
            }
        }
        if(!run_w) break;
        //Add new link...
        shared_ptr<k_tree> tmp = make_shared<k_tree>(graph.converted, graph.n_vertices);
        graph.converted.clear();

        size_t i, max_size;
        max_size = MAXSZ(graph.max_size_background, 0);
        for (i = 0; i < R; ++i) {
            if (graph.k_collection[i] != nullptr)
                max_size += graph.k_collection[i]->total_edges();
            if (MAXSZ(graph.max_size_background, i + 1) > max_size + 1)
                break;
        }
        if (i >= R)
            throw logic_error("Error: collection too big...");
        graph.max_r = max(i, graph.max_r);

        for (size_t j = 0; j <= i; ++j) {
            if (graph.k_collection[j] != nullptr) {
                tmp->unionOp(graph.k_collection[j]);
                graph.k_collection[j].reset();
            }
        }
        graph.k_collection[i] = tmp;
        {
            std::unique_lock<std::mutex> lk(mtx_lck_w);
            needs_work_w = false;
            cv_w.notify_all();
        }
//            cout << "notified in thread" << endl;
//            cout << "run_w: " << run_w << endl;
    }
//        cout << "really dead" << endl;
}

namespace dynamic_ktree {
#define R 8
    template<uint8_t k = 2,
            typename t_bv = bit_vector,
            typename t_rank = typename t_bv::rank_1_type>
    class DKtree_background_wait : public Graph<
            DKtreeEdgeIterator<
                    DKtree_background_wait<k, t_bv, t_rank>, k2_tree<k, t_bv, t_rank>, edge_iterator<k2_tree<k, t_bv, t_rank>>>,
            DKtreeNodeIterator<DKtree_background_wait<k, t_bv, t_rank>>,
            DKtreeNeighbourIterator<DKtree_background_wait<k, t_bv, t_rank>, k2_tree<k, t_bv, t_rank>, neighbour_iterator<k2_tree<k, t_bv, t_rank>>, Container0,
                    Container0NeighIterator<Container0>>

    > {
        using k_tree = k2_tree<k, t_bv, t_rank>;
        using k_tree_edge_it = edge_iterator<k_tree>;
        using k_tree_neighbour_it = neighbour_iterator<k_tree>;

        using dktree_edge_it = DKtreeEdgeIterator<DKtree_background_wait<k, t_bv, t_rank>, k_tree, k_tree_edge_it>;
        using dktree_node_it = DKtreeNodeIterator<DKtree_background_wait<k, t_bv, t_rank>>;
        using dktree_neighbour_it = DKtreeNeighbourIterator<DKtree_background_wait<k, t_bv, t_rank>, k_tree, k_tree_neighbour_it, Container0,
                Container0NeighIterator<Container0>>;
    private:
        uint64_t n_total_edges = 0;
        Container0 C0;
        dktree_edge_it it_edge_begin, it_end;
        dktree_node_it it_node_begin, it_node_end;
        dktree_neighbour_it it_neighbour_begin, it_neighbour_end;
        tuple<etype, etype> tmp_edge;

    public:
        uint max_r = 0;
        uint64_t n_vertices = 0;
        std::thread background_union;
        array<k_tree, R> k_collection_background;
        vector<tuple<uint64_t, uint64_t>> converted;
        uint tmp_i, max_r_background, max_size_background;
        array<shared_ptr<k_tree>, R> k_collection;

        DKtree_background_wait() : background_union() {}

        ~DKtree_background_wait() {
            {
                std::unique_lock<std::mutex> lk(mtx_lck_w);
                while (needs_work_w) {
                    cv_w.wait(lk);
                }
            }
            {
                std::unique_lock<std::mutex> lk(mtx_lck_w);
                run_w = false;
                cv_w.notify_all();
            }
        }

        DKtree_background_wait(uint n_vertices) : n_vertices(n_vertices) {
            C0 = Container0(n_vertices);
            max_r = 0;
            for (size_t i = 0; i < R; i++) {
                k_collection[i] = nullptr;
            }
            it_neighbour_end = dktree_neighbour_it().end();

            needs_work_w = false;
            run_w = true;

            background_union = std::thread([&]() {
                union_collection_wait<DKtree_background_wait < k, t_bv, t_rank>, k_tree > (*this);
            });
            background_union.detach();
        }

        virtual uint64_t get_number_edges() const {
            return n_total_edges;
        }

        virtual uint64_t get_number_nodes() const {
            return n_vertices;
        }

        int get_max_r() const {
            return max_r;
        }

        virtual void add_edge(etype x, etype y) {
            if (contains(x, y))
                return;
            size_t max_size = MAXSZ(max(n_vertices, n_total_edges), 0);
            if (C0.size() < max_size) {
                C0.insert(x, y, n_total_edges);
                ++n_total_edges;
                return;
            }

            {
                std::unique_lock<std::mutex> lk(mtx_lck_w);
                if (needs_work_w) {
//                    cout << "needs work" << endl;
                    while (needs_work_w) {
                        cv_w.wait(lk);
                    }
                }
            }
            for (uint64_t j = 0; j < C0.size_non_marked(); j++) {
                if (C0.edge_free[j] != -1) {
                    Edge converted_edge = C0.elements_nodes[C0.edge_free[j]];
                    converted.emplace_back(tuple<uint64_t, uint64_t>(converted_edge.x(), converted_edge.y()));
                }
            }
            converted.emplace_back(tuple<uint64_t, uint64_t>(x, y));
            max_size_background = max(n_vertices, n_total_edges);
            max_r_background = max_r;
            tmp_edge = tuple<etype, etype>(x, y);

            for (size_t j = 0; j < R; ++j) {
                if (k_collection[j] != nullptr)
                    k_collection_background[j] = *k_collection[j];
                else
                    k_collection_background[j] = k_tree();
            }

            {
                std::unique_lock<std::mutex> lk(mtx_lck_w);
                needs_work_w = true;
//                cout << "notifying" << endl;
                cv_w.notify_all();
            }

            C0.clean();
            ++n_total_edges;
        }

        virtual bool contains(etype x, etype y) {
            if (C0.contains(x, y)) {
                return true;
            }
            // check in other containers
            if (needs_work_w) {
                if (get<0>(tmp_edge) == x && get<1>(tmp_edge) == y)
                    return true;
                for (size_t i = 0; i <= max_r_background; i++)
                    if (k_collection_background[i].get_number_nodes() > 0 && k_collection_background[i].adj(x, y))
                        return true;
            } else {
                for (size_t i = 0; i <= max_r; i++)
                    if (k_collection[i] != nullptr && k_collection[i]->adj(x, y))
                        return true;
            }
            return false;
        }

        virtual void del_edge(etype x, etype y) {
            if (C0.erase(x, y)) {
                n_total_edges--;
                return;
            } else {
                while (needs_work_w) {}
                uint64_t n_total_marked = 0;
                for (size_t l = 0; l < R; l++) {
                    if (k_collection[l] != nullptr && k_collection[l]->erase(x, y)) {
                        n_total_edges--;

                        uint64_t k_marked_edges = k_collection[l]->get_marked_edges();
                        n_total_marked += k_marked_edges;

                        if (k_marked_edges == k_collection[l]->total_edges()) {
                            n_total_marked -= k_marked_edges;
                            k_collection[l].reset();
                        }
                        break;
                    }
                }
                /* Rebuild data structure... */
                if (n_total_marked > n_total_edges / TAU(n_total_edges)) {
                    const size_t old_max_r = max_r;
                    array<shared_ptr<k_tree>, R> old_collection;
                    max_r = 0;
                    for (size_t i = 0; i <= old_max_r; i++) {
                        if (k_collection[i] != nullptr) {
                            old_collection[i] = k_collection[i];
                            k_collection[i].reset();
                        }
                    }

                    n_total_edges = C0.size();
                    for (size_t j = 0; j <= old_max_r; j++) {
                        if (old_collection[j] != nullptr) {
                            old_collection[j]->edge_it(
                                    [this](uint64_t i, uint64_t j) -> void {
                                        this->add_edge(i, j);
                                    });
                        }
                    }
                }
            }
        }

        virtual vector<etype> list_neighbour(etype x) {
            vector<etype> neighbours;
            C0.list_neighbours(x, neighbours);

            if (needs_work_w) {
                if (get<0>(tmp_edge) == x)
                    neighbours.push_back(get<1>(tmp_edge));
                for (size_t l = 0; l <= max_r; l++)
                    if (k_collection_background[l] != k_tree()) {
                        vector<idx_type> lst = k_collection_background[l].neigh(x);
                        neighbours.insert(neighbours.end(), lst.begin(), lst.end()); //append
                    }
            } else {
                for (size_t l = 0; l <= max_r; l++)
                    if (k_collection[l] != nullptr) {
                        vector<idx_type> lst = k_collection[l]->neigh(x);
                        neighbours.insert(neighbours.end(), lst.begin(), lst.end()); //append
                    }
            }
            return neighbours;
        }


        Container0 first_container() const {
            return C0;
        }

        array<shared_ptr<k_tree>, R> k_collections() const {
            return k_collection;
        }

        virtual dktree_edge_it &edge_begin() {
            it_edge_begin = dktree_edge_it(this);
            it_end = it_edge_begin.end();
            return it_edge_begin;
        }

        virtual dktree_edge_it &edge_end() {
            return it_end;
        }

        virtual dktree_node_it &node_begin() {
            it_node_begin = dktree_node_it(this);
            it_node_end = it_node_begin.end();
            return it_node_begin;
        }

        virtual dktree_node_it &node_end() {
            return it_node_end;
        }

        virtual dktree_neighbour_it &neighbour_begin(etype node) {
            it_neighbour_begin = dktree_neighbour_it(this, &C0, node);
            return it_neighbour_begin;
        }

        bool is_last_neigh_it(k_tree_neighbour_it &it, size_t i) const {
            return it == k_collection[i]->neighbour_end();
        }

        bool is_invalid_tree(size_t i) const {
            return k_collection[i] == nullptr;
        }

        k_tree_neighbour_it &get_neighbour_begin(size_t i, etype node) const {
            return k_collection[i]->neighbour_begin(node);
        }

        virtual dktree_neighbour_it &neighbour_end() {
            return it_neighbour_end;
        }

        friend class boost::serialization::access;

        template<class Archive>
        void serialize(Archive &ar, const unsigned int) {
            ar & max_r;
            ar & n_vertices;
            ar & n_total_edges;

            ar & C0;
        }

        template<class Archive>
        void load(Archive &ar, const unsigned int) {
            ar >> max_r;
            ar >> n_vertices;
            ar >> n_total_edges;

            ar >> C0;
        }

        void serialize(std::ostream &out, string project_dir = "./") {
            boost::archive::text_oarchive oa(out);
            oa << *this;

            int status = mkdir(project_dir.append("dktree_serialize").c_str(), 0777);
            if (status < 0 && errno != EEXIST) throw "Could not create dir";

            while (needs_work_w) {}
            for (size_t l = 0; l <= max_r; l++) {
                if (k_collection[l] != nullptr) {
                    char filename[10];
                    sprintf(filename, "/%lu.kt", l);
                    string project_dir_copy = project_dir;
                    std::ofstream ktree_files(project_dir_copy.append(filename));
                    k_collection[l]->serialize(ktree_files);
                }
            }
        }

        void load(std::istream &in, string project_dir = "./", bool clear = true) {
            boost::archive::text_iarchive ar(in);
            ar >> *this;

            for (size_t l = 0; l <= max_r; l++) {
                char filename[10];
                string aux = project_dir;
                aux.append("dktree_serialize");
                sprintf(filename, "/%lu.kt", l);

                ifstream load_file(aux.append(filename).c_str());
                if (load_file.good()) {
                    k_tree new_ktree;
                    new_ktree.load(load_file);

                    shared_ptr<k_tree> tmp = make_shared<k_tree>(new_ktree);
                    k_collection[l] = tmp;
                    load_file.close();
                }
            }

            if (clear)
                clean_serialize(project_dir);
        }

        bool operator==(const DKtree_background_wait<k, t_bv, t_rank> &rhs) const {
            bool eval = true;
            eval &= max_r == rhs.max_r;
            eval &= n_vertices == rhs.n_vertices;
            eval &= n_total_edges == rhs.n_total_edges;
            eval &= C0 == rhs.C0;

            for (size_t l = 0; l <= max_r; l++) {
                if (k_collection[l] != nullptr && rhs.k_collection[l] != nullptr)
                    eval &= k_collection[l]->equal(*rhs.k_collection[l]);
                else if (k_collection[l] == nullptr && rhs.k_collection[l] != nullptr)
                    eval = false;
                else if (k_collection[l] != nullptr && rhs.k_collection[l] == nullptr)
                    eval = false;
            }

            return eval;
        }

        void clean_serialize(string project_dir = "./") {
            project_dir.append("dktree_serialize");

            for (auto &entry : std::experimental::filesystem::directory_iterator(project_dir))
                std::experimental::filesystem::remove(entry.path());
        }
    };
}

#endif