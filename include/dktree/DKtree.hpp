#ifndef __D_K_TREE__
#define __D_K_TREE__

#include <array>
#include <memory>
#include <unordered_map>
#include <iostream>
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

using namespace sdsl;
using namespace std;
using namespace k2_tree_ns;


namespace dynamic_ktree {
#define R 8

    template<uint8_t k = 2,
            typename t_bv = bit_vector,
            typename t_rank = typename t_bv::rank_1_type,
            typename l_rank = typename t_bv::rank_1_type>
    class DKtree : public Graph<
            DKtreeEdgeIterator <
            DKtree<k, t_bv, t_rank, l_rank>, k2_tree<k, t_bv, t_rank, l_rank>, edge_iterator<k2_tree<k, t_bv, t_rank, l_rank>>>,
                   DKtreeNodeIterator<DKtree<k, t_bv, t_rank, l_rank>>,
                   DKtreeNeighbourIterator<DKtree<k, t_bv, t_rank, l_rank>, k2_tree<k, t_bv, t_rank, l_rank>, neighbour_iterator<k2_tree<k, t_bv, t_rank, l_rank>>, Container0,
                           Container0NeighIterator < Container0>>

    > {
    using k_tree = k2_tree<k, t_bv, t_rank, l_rank>;
    using k_tree_edge_it = edge_iterator<k_tree>;
    using k_tree_neighbour_it = neighbour_iterator<k_tree>;

    using dktree_edge_it = DKtreeEdgeIterator<DKtree<k, t_bv, t_rank, l_rank>, k_tree, k_tree_edge_it>;
    using dktree_node_it = DKtreeNodeIterator<DKtree<k, t_bv, t_rank, l_rank>>;
    using dktree_neighbour_it = DKtreeNeighbourIterator<DKtree<k, t_bv, t_rank, l_rank>, k_tree, k_tree_neighbour_it, Container0,
            Container0NeighIterator < Container0>>;
    private:
    uint max_r = 0;
    uint64_t n_vertices = 0;
    uint64_t n_total_edges = 0;

    Container0 C0;

    dktree_edge_it it_edge_begin, it_end;
    dktree_node_it it_node_begin, it_node_end;
    dktree_neighbour_it it_neighbour_begin, it_neighbour_end;

    public:
    array<shared_ptr<k_tree>, R> k_collection;

    DKtree() {}

    DKtree(uint
    n_vertices) :
    n_vertices(n_vertices) {
            C0 = Container0(n_vertices);
            max_r = 0;
            for (size_t i = 0; i < R; i++) {
                k_collection[i] = nullptr;
            }
            it_neighbour_end = dktree_neighbour_it().end();
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
            n_total_edges++;
            return;
        }
        size_t i = 0;
        for (; i < R; i++) {
            if (k_collection[i] != nullptr)
                max_size += k_collection[i]->total_edges();
            if (MAXSZ(max(n_vertices, n_total_edges), i + 1) > max_size + 1)
                break;
        }

        if (i >= R)
            throw logic_error("Error: collection too big...");
        max_r = max(i, max_r);

        //Add new link...
        vector<tuple<uint64_t, uint64_t>> converted;
        for (uint64_t j = 0; j < C0.size_non_marked(); j++) {
            if (C0.edge_free[j] != -1) {
                Edge converted_edge = C0.elements_nodes[C0.edge_free[j]];
                converted.emplace_back(tuple<uint64_t, uint64_t>(converted_edge.x(), converted_edge.y()));
            }
        }
        converted.emplace_back(tuple<uint64_t, uint64_t>(x, y));
        shared_ptr<k_tree> tmp = make_shared<k_tree>(converted, n_vertices);
        C0.clean();

        for (size_t j = 0; j <= i; j++) {
            if (k_collection[j] != nullptr) {
                tmp->unionOp(k_collection[j]);
                k_collection[j].reset();
            }
        }

        k_collection[i] = tmp;
        n_total_edges++;
    }

    virtual bool contains(etype x, etype y) {
        if (C0.contains(x, y))
            return true;

        // check in other containers
        for (size_t i = 0; i < R; i++)
            if (k_collection[i] != nullptr && k_collection[i]->adj(x, y))
                return true;
        return false;
    }

    virtual void del_edge(etype x, etype y) {
        if (C0.erase(x, y)) {
            n_total_edges--;
            return;
        } else {
            for (size_t l = 0; l < R; l++)
                if (k_collection[l] != nullptr && k_collection[l]->erase(x, y)) {
                    n_total_edges--;
                    break;
                }

            uint64_t n_total_marked = 0;
            for (size_t l = 0; l < R; l++)
                if (k_collection[l] != nullptr) {
                    uint64_t k_marked_edges = k_collection[l]->get_marked_edges();
                    n_total_marked += k_marked_edges;

                    if (k_marked_edges == k_collection[l]->total_edges()) {
                        n_total_marked -= k_marked_edges;
                        k_collection[l].reset();
                    }
                }

            /* Rebuild data structure... */
            if (n_total_marked > n_total_edges / TAU(n_total_edges)) {
                size_t max_size = C0.size();
                size_t i = 0;
                for (; i < R; ++i) {
                    if (k_collection[i] != nullptr)
                        max_size += k_collection[i]->total_edges();
                    if (MAXSZ(max(n_vertices, n_total_edges), i + 1) > n_total_edges)
                        break;
                }

                shared_ptr<k_tree> tmp;
                bool oo = false;
                if (C0.size() == 0) {
                    oo = contains(1, 1);
                    add_edge(1, 1);
                    //tmp = make_shared<k_tree>(n_vertices);
                }
                if (C0.size() > 0) {
                    //Add new link...
                    vector<tuple<uint64_t, uint64_t>> converted;
                    for (uint64_t j = 0; j < C0.size_non_marked(); j++) {
                        if (C0.edge_free[j] != -1) {
                            Edge converted_edge = C0.elements_nodes[C0.edge_free[j]];
                            converted.emplace_back(tuple<uint64_t, uint64_t>(converted_edge.x(), converted_edge.y()));
                        }
                    }
                    tmp = make_shared<k_tree>(converted, n_vertices);
                    C0.clean();
                }
                for (uint j = 0; j < R; ++j) {
                    if (k_collection[j] != nullptr) {
                        tmp->unionOp(k_collection[j]);
                        k_collection[j].reset();
                    }
                }
                k_collection[i] = tmp;
                if (!oo) k_collection[i]->erase(1, 1);
            }
        }
    }

    virtual vector<etype> list_neighbour(etype x) {
        vector<etype> neighbours;
        C0.list_neighbours(x, neighbours);

        for (size_t l = 0; l <= max_r; l++)
            if (k_collection[l] != nullptr) {
                vector<idx_type> lst = k_collection[l]->neigh(x);
                neighbours.insert(neighbours.end(), lst.begin(), lst.end()); //append
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

    virtual dktree_neighbour_it &neighbour_end() {
        return it_neighbour_end;
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


    friend class boost::serialization::access;

    template<class Archive>
    void serialize(Archive &ar, const unsigned int) {
        ar & max_r;
        ar & n_vertices;
        ar & n_total_edges;
        ar & C0;
    }

    void serialize(std::ostream &out, string project_dir = "./") {
        int status = mkdir(project_dir.append("dktree_serialize").c_str(), 0777);
        if (status < 0 && errno != EEXIST) throw "Could not create dir";

        for (size_t l = 0; l <= max_r; l++) {
            if (k_collection[l] != nullptr) {
                char filename[10];
                sprintf(filename, "/%lu.kt", l + 1);
                string project_dir_copy = project_dir;
                std::ofstream ktree_files(project_dir_copy.append(filename));
                k_collection[l]->serialize(ktree_files);
                ktree_files.close();
            }
        }
        std::ofstream ss(project_dir.append("/0.kt"));
        boost::archive::text_oarchive oas(ss);
        oas << *this;
        ss.close();
    }

    void load(std::istream &in, string project_dir = "./", bool clear = true) {
        string project_dir_copy = project_dir;
        std::ifstream ifs(project_dir_copy.append("dktree_serialize/0.kt"));
        boost::archive::text_iarchive arf(ifs);
        arf >> *this;
        ifs.close();

        for (size_t l = 0; l <= max_r; l++) {
            char filename[10];
            string aux = project_dir;
            aux.append("dktree_serialize");
            sprintf(filename, "/%lu.kt", l + 1);

            ifstream load_file(aux.append(filename).c_str());
            if (load_file.good()) {
                shared_ptr<k_tree> new_ktree = make_shared<k_tree>();
                new_ktree->load(load_file);

                k_collection[l] = new_ktree;
                load_file.close();
            }
        }

        if (clear)
            clean_serialize(project_dir);
    }

    bool operator==(const DKtree<k, t_bv, t_rank, l_rank> &rhs) const {
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
