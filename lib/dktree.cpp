//#include "../include/dktree.hpp"
//
//template<uint8_t k, typename t_bv, typename t_rank, typename l_rank>
//dynamic_ktree::dktree<k, t_bv, t_rank, l_rank>::dktree(uint n_vertices) : n_vertices(n_vertices) {
//    C0 = Container_0(n_vertices);
//    max_r = 0;
//    uint aux = log(n_vertices) / log(k);
//    max_level = floor(aux) == aux ? floor(aux) - 1 : floor(aux);
//    for (size_t i = 0; i <= max_level; i++)
//        div_level_table.push_back(pow(k, max_level - i));
//    for (size_t i = 0; i < R; i++) {
//        // shared_ptr<k2_tree_extended> p(new k2_tree_extended(n_vertices));
//        k_collection[i] = nullptr;
//    }
//}
//
//template<uint8_t k, typename t_bv, typename t_rank, typename l_rank>
//void dynamic_ktree::dktree<k, t_bv, t_rank, l_rank>::insert(uint x, uint y) {
//    if (contains(x, y))
//        return;
//    size_t max_size = MAXSZ(max(n_vertices, n_total_edges), 0);
//    if (C0.n_elements < max_size) {
//        C0.insert(x, y);
//        n_total_edges++;
//        return;
//    }
//
//    size_t i = 0;
//    for (; i < R; i++) {
//        if (k_collection[i] != nullptr)
//            max_size += k_collection[i]->get_number_edges(); //TODO: rename me to size
//        if (MAXSZ(max(n_vertices, n_total_edges), i + 1) > max_size + 1)
//            break;
//    }
//
//    if (i >= R)
//        throw logic_error("Error: collection too big...");
//    max_r = max(i, max_r);
//
//    //Load edges in C0...
//    vector<tuple<idx_type, idx_type>> free_edges;
//    for (uint j = 0; j < C0.n_elements; j++) {
//        const tuple<idx_type, idx_type> e(C0.elements[j].x, C0.elements[j].y);
//        free_edges.push_back(e);
//    }
//    //Add new link...
//    const tuple<idx_type, idx_type> e(x, y);
//    free_edges.push_back(e);
//
//    assert(free_edges.size() == C0.n_elements + 1);
//    C0.clean(n_vertices);
//    if (floor(log(n_vertices) / log(k)) == (log(n_vertices) / log(k)))
//        max_level--;
//
//    shared_ptr<k_tree> tmp = make_shared<k_tree>(free_edges, n_vertices);
//    for (size_t j = 0; j <= i; j++) {
//        if (k_collection[j] != nullptr) {
//            k_tree aux = tmp->unionOp(*k_collection[j], n_vertices);
//            tmp = make_shared<k_tree>(move(aux));
//            tmp->set_level(div_level_table);
//        }
//        k_collection[j] = nullptr;
//    }
//    assert(k_collection[i] == nullptr);
//    k_collection[i] = tmp;
//    n_total_edges++;
//}
//
//template<uint8_t k, typename t_bv, typename t_rank, typename l_rank>
//void dynamic_ktree::dktree<k, t_bv, t_rank, l_rank>::erase(uint x, uint y) {
//    if (C0.erase(x, y)) n_total_edges--;
//    else {
//        uint n_total_marked = 0;
//        for (size_t l = 0; l <= max_r; l++) {
//            if (k_collection[l] != nullptr && k_collection[l]->erase(x, y)) {
//                n_total_edges--;
//// cout << "n_total_edges " << n_total_edges << endl;
//
//                uint k_marked_edges = k_collection[l]->get_marked_edges();
//                n_total_marked += k_marked_edges;
//
//                if (k_marked_edges == k_collection[l]->get_number_edges()) {
//                    n_total_marked -= k_marked_edges;
//                    delete &k_collection[l];
//                    k_collection[l] = nullptr;
//                }
//            }
//        }
//
//        if (n_total_marked > n_total_edges / TAU(n_total_edges)) {
///* Rebuild data structure... */
//            array<shared_ptr<k_tree>, R> old_k_collection = k_collection;
//            uint old_max_r = max_r;
//            max_r = 0;
//
//            for (size_t i = 0; i < R; i++) {
//                shared_ptr<k_tree> p(new k_tree(n_vertices));
//                k_collection[i] = p;
//            }
////TODO: DUVIDA O QUE E QUE ESTA PARTE DO CODIGO ESTA MESMO A FAZER?
//// ITERA E DEPOIS APAGA?
//            n_total_edges = C0.adj_lst.size();
//            for (size_t l = 0; l <= old_max_r; l++) {
//                if (old_k_collection[l] != nullptr && old_k_collection[l] != 0) {
//                    function<int(uint, uint)> func = [this](uint x, uint y) {
//                        insert(x, y);
//                        return 0;
//                    };
//                    old_k_collection[l]->edge_iterator(func);
//                    old_k_collection[l] = nullptr;
//                }
//            }
//        }
//    }
//}
//
//template<uint8_t k, typename t_bv, typename t_rank, typename l_rank>
//vector<int> dynamic_ktree::dktree<k, t_bv, t_rank, l_rank>::list_neighbour(int x) {
//    vector<int> neighbours;
//    C0.list_neighbours(x, neighbours);
//
//    for (size_t l = 0; l <= max_r; l++)
//        if (k_collection[l] != nullptr) {
//            vector<idx_type> lst = k_collection[l]->neigh(x);
//            neighbours.insert(neighbours.end(), lst.begin(), lst.end()); //append
//        }
//    return neighbours;
//}
//
//template<uint8_t k, typename t_bv, typename t_rank, typename l_rank>
//bool dynamic_ktree::dktree<k, t_bv, t_rank, l_rank>::contains(int x, int y) {
//    if (C0.edge_lst.find(x, y) != -1)
//        return true;
//
//    // check in other containers
//    for (size_t i = 0; i <= max_r; i++)
//        if (k_collection[i] != nullptr && k_collection[i]->adj(x, y))
//            return true;
//    return false;
//}