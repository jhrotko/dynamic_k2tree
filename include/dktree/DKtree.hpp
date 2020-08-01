#ifndef __D_K_TREE__
#define __D_K_TREE__

#include <array>
#include <memory>
#include <unordered_map>
#include <iostream>
#include "utils.hpp"

#include <sdsl/k2_tree.hpp>
#include "Container_0.hpp"
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
    class DKtree: public Graph<
            DKtreeEdgeIterator<DKtree<k, t_bv, t_rank, l_rank>, k2_tree<k, t_bv, t_rank, l_rank>, edge_iterator<k2_tree<k, t_bv, t_rank, l_rank>>>,
            DKtreeNodeIterator<DKtree<k, t_bv, t_rank, l_rank>>,
            DKtreeNeighbourIterator<DKtree<k, t_bv, t_rank, l_rank>, k2_tree<k, t_bv, t_rank, l_rank>, neighbour_iterator<k2_tree<k, t_bv, t_rank, l_rank>>>> {
        using k_tree = k2_tree<k, t_bv, t_rank, l_rank>;
        using k_tree_edge_it = edge_iterator<k_tree>;
        using k_tree_neighbour_it = neighbour_iterator<k_tree>;

        using dktree_edge_it = DKtreeEdgeIterator<DKtree<k, t_bv, t_rank, l_rank>, k_tree, k_tree_edge_it>;
        using dktree_node_it = DKtreeNodeIterator<DKtree<k, t_bv, t_rank, l_rank>>;
        using dktree_neighbour_it = DKtreeNeighbourIterator<DKtree<k, t_bv, t_rank, l_rank>, k_tree, k_tree_neighbour_it>;
    private:
        uint max_r = 0;
        uint n_vertices = 0;
        uint n_total_edges = 0;

        Container_0 C0;
        array<shared_ptr<k_tree>, R> k_collection;

        dktree_edge_it it_edge_begin, it_end;
        dktree_node_it it_node_begin, it_node_end;
        dktree_neighbour_it it_neighbour_begin, it_neighbour_end;

    public:
        DKtree() {}
        DKtree(uint n_vertices) : n_vertices(n_vertices) {
            C0 = Container_0(n_vertices);
            max_r = 0;
            for (size_t i = 0; i < R; i++) {
                k_collection[i] = nullptr;
            }
        }

        virtual size_t get_number_edges() const {
            return n_total_edges;
        }

        virtual size_t get_number_nodes() const {
            return n_vertices;
        }

        int get_max_r() const {
            return max_r;
        }

        virtual void add_edge(etype x, etype y)
        {
            if (contains(x, y))
                return;
            size_t max_size = MAXSZ(max(n_vertices, n_total_edges), 0);
            if (C0.size() < max_size) {
                C0.insert(x, y, n_total_edges);
                n_total_edges++;
                return;
            }

            size_t i;
            for (i = 0; i < R; i++) {
                if (k_collection[i] != nullptr)
                    max_size += k_collection[i]->get_number_edges();
                if (MAXSZ(max(n_vertices, n_total_edges), i + 1) > max_size + 1)
                    break;
            }

            if (i >= R)
                throw logic_error("Error: collection too big...");
            max_r = max(i, max_r);

            //Add new link...
            C0.elements_nodes.push_back(Edge(x, y));
            vector<tuple<etype, etype>> converted;
            for(auto element: C0.elements_nodes) {
                converted.push_back(tuple<etype, etype>(element.x(), element.y()));
            }

            shared_ptr<k_tree> tmp = make_shared<k_tree>(converted, n_vertices);
            C0.clean();
            for (size_t j = 0; j <= i; j++) {
                if (k_collection[j] != nullptr) {
                    tmp->unionOp(*k_collection[j]);
                    k_collection[j].reset();
                }
            }
            k_collection[i] = tmp;
            n_total_edges++;
        }

        virtual bool contains(etype x, etype y)
        {
            if (C0.edge_lst.contains(x, y))
                return true;

            // check in other containers
            for (size_t i = 0; i <= max_r; i++)
                if (k_collection[i] != nullptr && k_collection[i]->adj(x, y))
                    return true;
            return false;
        }

        virtual void del_edge(etype x, etype y)
        {
            if (C0.erase(x, y)) n_total_edges--;
            else {
                uint n_total_marked = 0;
                for (size_t l = 0; l <= max_r; l++) {
                    if (k_collection[l] != nullptr && k_collection[l]->erase(x, y)) {
                        n_total_edges--;

                        uint k_marked_edges = k_collection[l]->get_marked_edges();
                        n_total_marked += k_marked_edges;

                        if (k_marked_edges == k_collection[l]->get_number_edges()) {
                            n_total_marked -= k_marked_edges;
                            delete &k_collection[l];
                            k_collection[l] = nullptr;
                        }
                    }
                }

                if (n_total_marked > n_total_edges / TAU(n_total_edges)) {
                    /* Rebuild data structure... */
                    max_r = 0;
                    for (size_t i = 0; i < R; i++) {
                        k_collection[i] = make_shared<k_tree>();
                    }
                }
            }
        }

        virtual vector<etype> list_neighbour(etype x)
        {
            vector<etype> neighbours;
            C0.list_neighbours(x, neighbours);

            for (size_t l = 0; l <= max_r; l++)
                if (k_collection[l] != nullptr) {
                    vector<idx_type> lst = k_collection[l]->neigh(x);
                    neighbours.insert(neighbours.end(), lst.begin(), lst.end()); //append
                }
            return neighbours;
        }


        Container_0 first_container() const
        {
            return C0;
        }

        array<shared_ptr<k_tree>, R> k_collections() const
        {
            return k_collection;
        }

        virtual dktree_edge_it &edge_begin()
        {
            it_edge_begin = dktree_edge_it(this);
            return it_edge_begin;
        }

        virtual dktree_edge_it &edge_end()
        {
            it_end = it_edge_begin.end();
            return it_end;
        }

        virtual dktree_node_it &node_begin()
        {
            it_node_begin = dktree_node_it(this);
            return it_node_begin;
        }

        virtual dktree_node_it &node_end()
        {
            it_node_end = it_node_begin.end();
            return it_node_end;
        }

        virtual dktree_neighbour_it &neighbour_begin(etype node)
        {
            it_neighbour_begin = dktree_neighbour_it(this, node);
            return it_neighbour_begin;
        }

        virtual dktree_neighbour_it &neighbour_end()
        {
            it_neighbour_end = it_neighbour_begin.end();
            return it_neighbour_end;
        }

        friend class boost::serialization::access;
        template<class Archive>
        void serialize(Archive & ar, const unsigned int)
        {
            ar & max_r;
            ar & n_vertices;
            ar & n_total_edges;

            ar & C0;
        }

        template<class Archive>
        void load(Archive & ar, const unsigned int)
        {
            ar >> max_r;
            ar >> n_vertices;
            ar >> n_total_edges;

            ar >> C0;
        }

        void serialize(std::ostream &out, string project_dir="./") {
            boost::archive::text_oarchive oa(out);
            oa << *this;

            int status = mkdir(project_dir.append("dktree_serialize").c_str(), 0777);
            if (status < 0 && errno != EEXIST) throw "Could not create folder";

            for (size_t l = 0; l <= max_r; l++)
                if (k_collection[l] != nullptr) {
                    char filename[10];
                    sprintf (filename, "/%lu.kt", l);
                    std::ofstream ktree_files(project_dir.append(filename));
                    k_collection[l]->serialize(ktree_files);
                }
        }

        void load(std::istream &in, string project_dir="./") {
            boost::archive::text_iarchive ar(in);
            ar >> *this;

            for (size_t l = 0; l <= max_r; l++) {
                char filename[10];
                string aux = project_dir;
                aux.append("dktree_serialize");
                sprintf (filename, "/%lu.kt", l);

                ifstream load_file(aux.append(filename).c_str());
                if(load_file.good()) {
                    k_tree new_ktree;
                    new_ktree.load(load_file);

                    shared_ptr<k_tree> tmp = make_shared<k_tree>(new_ktree);
                    k_collection[l] = tmp;
                    load_file.close();
                }
            }

        }

        bool operator==(const DKtree<k, t_bv, t_rank, l_rank> &rhs) const {
            bool eval = true;
            eval &= max_r == rhs.max_r;
            eval &= n_vertices == rhs.n_vertices;
            eval &= n_total_edges == rhs.n_total_edges;
            eval &= C0 == rhs.C0;

            for (size_t l = 0; l <= max_r; l++) {
                if(k_collection[l] != nullptr && rhs.k_collection[l] != nullptr)
                    eval &= k_collection[l]->equal(*rhs.k_collection[l]);
                else if(k_collection[l] == nullptr && rhs.k_collection[l] != nullptr)
                    eval = false;
                else if(k_collection[l] != nullptr && rhs.k_collection[l] == nullptr)
                    eval = false;
            }

            return eval;
        }

        void clean_serialize(string project_dir="./") {
            project_dir.append("dktree_serialize");

            for (auto & entry : std::experimental::filesystem::directory_iterator(project_dir))
                std::experimental::filesystem::remove(entry.path());
        }
    };
}

#endif