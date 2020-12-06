#include <string>
#include <vector>

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/classification.hpp>
#include "../../include/dktree/DKtree.hpp"
#include "../../include/dktree/DKtree_background.hpp"
#include "../../include/dktree/DKtree_background_wait.hpp"
#include "../../include/dktree/DKtree_delay.hpp"
#include "../../include/dktree/DKtree_delay_.hpp"

void split(const std::string &str, std::vector<std::string> &cont,
           const std::string &delims = " ") {
    boost::split(cont, str, boost::is_any_of(delims));
}

void build_edges(std::ostringstream &path, vector<tuple<uint64_t, uint64_t>> &edges) {
    std::ifstream test_case(path.str());

    clock_t time_t;
    if (test_case.is_open()) {
        std::string line;
        vector<std::string> substrings;
        const std::string delims = " ";

        while (getline(test_case, line)) {
            split(line, substrings, delims);

            etype x = (etype) stoi(substrings[1]);
            etype y = (etype) stoi(substrings[2]);
            if (substrings[0] == "a") {
                edges.emplace_back(tuple<uint64_t, uint64_t>(x, y));
            }
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 5) {
        fprintf(stderr, "Usage: %s <path to dataset> <number of vertices> <number of runs> <dynamic k2tree version>\n",
                argv[0]);
        return EXIT_FAILURE;
    }
    std::ostringstream path;
    path << argv[1];
    std::vector<tuple<uint64_t, uint64_t>> edges;
    build_edges(path, edges);

    uint n_vertices = atoi(argv[2]);
    uint runs = atoi(argv[3]);
    uint dynamic_type = atoi(argv[4]);

    std::vector<std::vector<clock_t>> runs_time;
    for (int i = 0; i < runs; ++i) {
        runs_time.emplace_back(std::vector<clock_t>());
    }

    switch (dynamic_type) {
        case 1: {
            for (int i = 0; i < runs; ++i) {
                dynamic_ktree::DKtree<2> graph(n_vertices);
                for (auto edge:edges) {
                    uint64_t x = get<0>(edge);
                    uint64_t y = get<1>(edge);

                    clock_t aux = clock();
                    graph.add_edge(x, y);
                    clock_t aux_end = clock();
                    runs_time[i].emplace_back(aux_end - aux);
                }
            }
            break;
        }
        case 2: {
            for (int i = 0; i < runs; ++i) {
                dynamic_ktree::DKtree_background<2> graph(n_vertices);
                for (auto edge:edges) {
                    uint64_t x = get<0>(edge);
                    uint64_t y = get<1>(edge);

                    clock_t aux = clock();
                    graph.add_edge(x, y);
                    clock_t aux_end = clock();
                    runs_time[i].emplace_back(aux_end - aux);
                }
            }
            break;
        }
        case 3: {
            for (int i = 0; i < runs; ++i) {
                dynamic_ktree::DKtree_delay<2> graph(n_vertices);
                for (auto edge:edges) {
                    uint64_t x = get<0>(edge);
                    uint64_t y = get<1>(edge);

                    clock_t aux = clock();
                    graph.add_edge(x, y);
                    clock_t aux_end = clock();
                    runs_time[i].emplace_back(aux_end - aux);
                }
            }
            break;
        }
        case 4: {
            for (int i = 0; i < runs; ++i) {
                dynamic_ktree::DKtree_delay_munro<2> graph(n_vertices);
                for (auto edge:edges) {
                    uint64_t x = get<0>(edge);
                    uint64_t y = get<1>(edge);

                    clock_t aux = clock();
                    graph.add_edge(x, y);
                    clock_t aux_end = clock();
                    runs_time[i].emplace_back(aux_end - aux);
                }
            }
            break;
        }
        case 5: {
            for (int i = 0; i < runs; ++i) {
                dynamic_ktree::DKtree_background_wait<2> graph(n_vertices);
                for (auto edge:edges) {
                    uint64_t x = get<0>(edge);
                    uint64_t y = get<1>(edge);

                    clock_t aux = clock();
                    graph.add_edge(x, y);
                    clock_t aux_end = clock();
                    runs_time[i].emplace_back(aux_end - aux);
                }
            }
            break;
        }
    }
    uint c = 0;
    for (uint edge = 0; edge < edges.size(); ++edge) {
        ++c;
        if(c == 1) {
            double sum = 0;
            for (uint i = 0; i < runs; ++i) {
                sum += runs_time[i][edge];
            }
            cout << ((double) sum / runs / CLOCKS_PER_SEC) << endl;
            c = 0;
        }
    }
    return 0;
}