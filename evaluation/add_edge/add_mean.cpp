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

int main(int argc, char *argv[]) {
    if (argc != 5) {
        fprintf(stderr, "Usage: %s <path to dataset> <number of vertices> <number of runs> <dynamic k2tree version>\n",
                argv[0]);
        return EXIT_FAILURE;
    }
    std::ostringstream path;
    path << argv[1];

    uint n_vertices = atoi(argv[2]);
    double runs = atoi(argv[3]);
    uint dynamic_type = atoi(argv[4]);

    double final = 0;

    switch (dynamic_type) {
        case 1: {
            for (int i = 0; i < runs; ++i) {
                dynamic_ktree::DKtree<2> graph(n_vertices);
                std::ifstream test_case(path.str());
                double time_t = 0;
                double arcs = 0;
                if (test_case.is_open()) {
                    std::string line;
                    vector<std::string> substrings;
                    const std::string delims = " ";

                    while (getline(test_case, line)) {
                        split(line, substrings, delims);

                        uint64_t x = (uint64_t) stoi(substrings[1]);
                        uint64_t y = (uint64_t) stoi(substrings[2]);
                        clock_t aux = clock();
                        graph.add_edge(x, y);
                        time_t += clock() - aux;
                        ++arcs;
                    }
                }
                time_t /= arcs;
                time_t /= CLOCKS_PER_SEC;
                final += time_t;
                test_case.close();
            }
            break;
        }
        case 2: {
            for (int i = 0; i < runs; ++i) {
                dynamic_ktree::DKtree_background<2> graph(n_vertices);
                std::ifstream test_case(path.str());
                double time_t = 0;
                double arcs = 0;
                if (test_case.is_open()) {
                    std::string line;
                    vector<std::string> substrings;
                    const std::string delims = " ";

                    while (getline(test_case, line)) {
                        split(line, substrings, delims);

                        uint64_t x = (uint64_t) stoi(substrings[1]);
                        uint64_t y = (uint64_t) stoi(substrings[2]);
                        clock_t aux = clock();
                        graph.add_edge(x, y);
                        time_t += clock() - aux;
                        ++arcs;
                    }
                }
                test_case.close();
                time_t /= arcs;
                time_t /= CLOCKS_PER_SEC;
                final += time_t;
            }
            break;
        }
        case 3: {
            for (int i = 0; i < runs; ++i) {
                dynamic_ktree::DKtree_delay<2> graph(n_vertices);
                std::ifstream test_case(path.str());

                double time_t = 0;
                double arcs = 0;
                if (test_case.is_open()) {
                    std::string line;
                    vector<std::string> substrings;
                    const std::string delims = " ";

                    while (getline(test_case, line)) {
                        split(line, substrings, delims);

                        uint64_t x = (uint64_t) stoi(substrings[1]);
                        uint64_t y = (uint64_t) stoi(substrings[2]);
                        clock_t aux = clock();
                        graph.add_edge(x, y);
                        time_t += clock() - aux;
                        ++arcs;
                    }
                }
                time_t /= arcs;
                time_t /= CLOCKS_PER_SEC;
                final += time_t;
                test_case.close();
            }
            break;
        }
        case 4: {
            for (int i = 0; i < runs; ++i) {
                dynamic_ktree::DKtree_delay_munro<2> graph(n_vertices);
                std::ifstream test_case(path.str());

                double time_t = 0;
                double arcs = 0;
                if (test_case.is_open()) {
                    std::string line;
                    vector<std::string> substrings;
                    const std::string delims = " ";

                    while (getline(test_case, line)) {
                        split(line, substrings, delims);

                        uint64_t x = (uint64_t) stoi(substrings[1]);
                        uint64_t y = (uint64_t) stoi(substrings[2]);
                        clock_t aux = clock();
                        graph.add_edge(x, y);
                        time_t += clock() - aux;
                        ++arcs;
                    }
                }
                time_t /= arcs;
                time_t /= CLOCKS_PER_SEC;
                final += time_t;
                test_case.close();
            }
            break;
        }
        case 5: {
            for (int i = 0; i < runs; ++i) {
                dynamic_ktree::DKtree_background_wait<2> graph(n_vertices);
                std::ifstream test_case(path.str());

                double time_t = 0;
                double arcs = 0;
                if (test_case.is_open()) {
                    std::string line;
                    vector<std::string> substrings;
                    const std::string delims = " ";

                    while (getline(test_case, line)) {
                        split(line, substrings, delims);

                        uint64_t x = (uint64_t) stoi(substrings[1]);
                        uint64_t y = (uint64_t) stoi(substrings[2]);
                        clock_t aux = clock();
                        graph.add_edge(x, y);
                        time_t += clock() - aux;
                        ++arcs;
                    }
                }
                time_t /= arcs;
                time_t /= CLOCKS_PER_SEC;
                final += time_t;
                test_case.close();
            }
            break;
        }
    }

    final /= runs;
    cout << ((double) final) << endl;
    return 0;
}