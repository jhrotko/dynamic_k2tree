#include <string>
#include <vector>

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/classification.hpp>
#include "../include/dktree/DKtree.hpp"

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
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <path to dataset> <number of vertices> <number of runs>\n",
                argv[0]);
        return EXIT_FAILURE;
    }
    std::ifstream ifs(argv[1]);
    sdsl::k2_tree<2> ktree_test = sdsl::k2_tree<2>();
    ktree_test.load(ifs);
    ifs.close();

    std::ifstream ifs2(argv[2]);
    sdsl::k2_tree<2> ktree_test2 = sdsl::k2_tree<2>();
    ktree_test2.load(ifs2);
    ifs2.close();

    shared_ptr<sdsl::k2_tree<2>> ptr1 = make_shared<sdsl::k2_tree<2>>(ktree_test);
    shared_ptr<sdsl::k2_tree<2>> ptr2 = make_shared<sdsl::k2_tree<2>>(ktree_test2);
    double sum = 0;
    int runs = 5;

    for (int i = 0; i < runs; i++) {
        clock_t start = clock();
        ptr1->unionOp(ptr2);
        clock_t end = clock();

        sum += (double) (end - start) / CLOCKS_PER_SEC;
    }
    cout << sum/(double)runs << endl;
    return 0;
}