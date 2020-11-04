#ifndef DYNAMIC_K2TREE_UNION_SUPPORT_HPP
#define DYNAMIC_K2TREE_UNION_SUPPORT_HPP

#include <array>
#include <queue>
#include <memory>
#include <cmath>

namespace dynamic_ktree {
    template<class k_tree, class bit_vector, class k_rank>
    class k2_tree_union_support {
    private:
        std::queue<std::array<uint8_t, 3>> Q;
        bit_vector C_t, C_l;
        uint C_t_size, C_l_size;

        uint64_t n_total_edges = 0;
        uint pA, pB, idx_l, idx_t;

        std::shared_ptr<k_tree> tmp;
        std::array<std::shared_ptr<k_tree>, R> *k_collection;
        uint k_collection_i = R;
        uint max_height, iteration_size, times = 0;

        bool finish;

        uint get_ones(std::shared_ptr<k_tree> kA) {
            return kA->k_t.size() + kA->k_l.size();
        }

        void initialize() {
            Q = std::queue<std::array<uint8_t, 3>>();
            Q.push({0, 1, 1});
            pA = 0;
            pB = 0;
            idx_l = 0;
            idx_t = 0;
        }

        void result() {
            assert(C_t_size >= idx_t);
            C_t.resize(idx_t);
            assert(C_l_size >= idx_l);
            C_l.resize(idx_l);

            tmp->k_t = bit_vector(C_t);
            tmp->k_l = bit_vector(C_l);
            tmp->k_t_rank = k_rank(&tmp->k_t);
            tmp->k_l_rank = k_rank(&tmp->k_l);
            tmp->k_height = std::ceil(std::log(tmp->get_number_nodes()) / std::log(tmp->k_k));
            tmp->k_height = tmp->k_height > 1 ?tmp-> k_height : 1;
            tmp->set_edges(n_total_edges);
            n_total_edges = 0;
//            cout << "T:";
//            for (auto el:tmp->k_t) {
//                cout << el;
//            }
//            cout << endl;
//
//            cout << "L:";
//            for (auto el:tmp->k_l) {
//                cout << el;
//            }
//            cout << endl;
        }

    public:
        uint max_i = 0;

        k2_tree_union_support() {
            finish = true;
        }

        k2_tree_union_support(std::vector<tuple<uint64_t, uint64_t>> &edges,
                              uint n_vertices,
                              std::array<std::shared_ptr<k_tree>, R> *k_collection,
                              uint i) : max_i(i) {
            this->k_collection = k_collection;
            this->tmp = make_shared<k_tree>(edges, n_vertices);

//            max_j = 2*(n/log(n)*log(n))*log^0(n), where n is n_vertices
//                  = 2*(n/log(n)*log(n))
            iteration_size = 0;
            uint total_iterations = 0;
            for (uint j = 0; j <= max_i; ++j) {
                if ((*k_collection)[j] != nullptr) {
                    total_iterations += get_ones((*k_collection)[j]);
                }
            }
            total_iterations += get_ones(tmp);
            if(total_iterations == 0)
                total_iterations = 1;

            uint C0_size = edges.size();
            iteration_size = std::ceil(total_iterations / C0_size);
            if(total_iterations % C0_size != 0) iteration_size++;
            assert(iteration_size <= total_iterations);

            for (uint j = 0; j <= max_i; ++j) {
                if ((*k_collection)[j] != nullptr) {
                    k_collection_i = j;
                    break;
                }
            }
//            cout << k_collection_i << endl;
            if (k_collection_i == R) { // all nullptr, nothing to do
//                cout << "no unions needed" << endl;
                (*(this->k_collection))[max_i] = this->tmp;
                finish = true;
//
//                cout << "T:";
//                for (auto el:tmp->k_t) {
//                    cout << el;
//                }
//                cout << endl;
//
//                cout << "L:";
//                for (auto el:tmp->k_l) {
//                    cout << el;
//                }
//                cout << endl;

                return;
            }
            finish = false;
            times = 0;
            initialize();
        }

        void unionOperation() {
            if (has_finish())
                return;
//            cout << "performed union" << endl;
            assert((*k_collection)[k_collection_i] != nullptr);

            uint8_t k = tmp->k_k;
            uint t_size_A = tmp->k_t.size();
            uint t_size_B = (*k_collection)[k_collection_i]->k_t.size();
            max_height = max(tmp->k_height, (*k_collection)[k_collection_i]->k_height);

            array<uint8_t, 3> head = Q.front();
            if (head[0] == 0 && head[1] == 1 && head[2] == 1) {
                const uint l_size_A = tmp->k_l.size();
                const uint l_size_B = (*k_collection)[k_collection_i]->k_l.size();

                // C Initialization
                uint calc = 1;
                uint max_bits = 0;
                for (uint j = 0; j < max_height; j++) {
                    calc *= k * k;
                    max_bits += calc;
                }
                //instead of creating a new one, just add it inside the k_t and k_l
                C_t_size = max_bits < t_size_A + t_size_B ? max_bits : t_size_A + t_size_B;
                C_t = bit_vector(C_t_size);

                calc *= k * k;
                max_bits += calc;
                C_l_size = max_bits < l_size_A + l_size_B ? max_bits : l_size_A + l_size_B;
                C_l = bit_vector(C_l_size);
            }

            while (!Q.empty()) {
                if (times >= iteration_size)
                    break;
                std::array<uint8_t, 3> next = Q.front();
                Q.pop();
                ++times;

                uint8_t l = next[0];
                uint8_t rA = next[1];
                uint8_t rB = next[2];
                for (size_t i = 0; i < k * k; ++i) {
                    uint8_t bA = 0;
                    uint8_t bB = 0;
                    if (rA == 1) {
                        if (l < max_height - 1)
                            bA = tmp->k_t[pA];
                        else
                            bA = tmp->k_l[pA - t_size_A];
                        pA++;
                    }
                    if (rB == 1) {
                        if (l < max_height - 1)
                            bB = (*k_collection)[k_collection_i]->k_t[pB];
                        else
                            bB = (*k_collection)[k_collection_i]->k_l[pB - t_size_B];
                        pB++;
                    }

                    if (l < max_height - 1) {
                        if (bA || bB) {
                            uint8_t l_aux = l + 1;
                            Q.push({l_aux, bA, bB});
                            C_t[idx_t] = 1;
                        }
                        idx_t++;
                    } else {
                        if (bA || bB) {
                            C_l[idx_l] = 1;
                            n_total_edges++;
                        }
                        idx_l++;
                    }
                }
            }

            if (Q.empty()) { //finished a smaller k2_tree but must continue to do unions
                (*k_collection)[k_collection_i].reset();
                result(); //update tmp

                for (; k_collection_i <= max_i; ++k_collection_i) {
                    if ((*k_collection)[k_collection_i] != nullptr) {
                        break;
                    }
                }
                if (k_collection_i > max_i) {
                    (*k_collection)[max_i] = tmp;
                    finish = true;
//                    cout << "Finish union, no more ktree" << endl;
                    return;
                }
                assert(k_collection_i <= max_i);
                initialize();
                if(times == iteration_size) {
                    times = 0;
                    return;
                }
                unionOperation();
                return;
            } else { // did not fully finish the union
                times = 0;
                finish = false;
//                cout << "next turn continue union" << endl;
                return;
            }
        }

        bool has_finish() {
            return finish;
        }
    };
}
#endif //DYNAMIC_K2TREE_UNION_SUPPORT_HPP
