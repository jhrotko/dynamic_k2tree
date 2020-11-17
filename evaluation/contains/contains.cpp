#include <string>
#include <vector>

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/classification.hpp>
#include "../../include/dktree/DKtree.hpp"

#include "sys/types.h"
#include "sys/sysinfo.h"

struct sysinfo memInfo;

#include <cstdlib>
#include <cstdio>

static unsigned long long lastTotalUser, lastTotalUserLow, lastTotalSys, lastTotalIdle;

int parseLine(char* line){
    // This assumes that a digit will be found and the line ends in " Kb".
    int i = strlen(line);
    const char* p = line;
    while (*p <'0' || *p > '9') p++;
    line[i-3] = '\0';
    i = atoi(p);
    return i;
}
int getValue(){ //Note: this value is in KB!
    FILE* file = fopen("/proc/self/status", "r");
    int result = -1;
    char line[128];

    while (fgets(line, 128, file) != NULL){
        if (strncmp(line, "VmRSS:", 6) == 0){
            result = parseLine(line);
            break;
        }
    }
    fclose(file);
    return result;
}

void split(const std::string &str, std::vector<std::string> &cont,
           const std::string &delims = " ") {
    boost::split(cont, str, boost::is_any_of(delims));
}

int main(int argc, char *argv[]) {
    long long totalPhysMem = 0;
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <path file> <number of runs> <save folder name>\n",
                argv[0]);
        return EXIT_FAILURE;
    }
    int runs = atoi(argv[2]);
    string  folder(argv[3]);
    double final = 0;

    for(int i=0; i < runs; i++) {
        std::ostringstream path;
        path << argv[1];
        std::ifstream test_case(path.str());
        double sum = 0;

        dynamic_ktree::DKtree<2> tree;
        std::ifstream ifs;
        tree.load(ifs, folder, false);
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
                    clock_t start = clock();
                    tree.contains(x, y);
                    clock_t time_iter = clock() - start;
                    sum = max(sum, time_iter);
//                    sysinfo (&memInfo);
//                    totalPhysMem = max( totalPhysMem, getValue());
                }
            }
        }
        sum /= CLOCKS_PER_SEC;
        final += sum;
        test_case.close();
    }
//    cout << "memory: " << totalPhysMem << endl;
    cout << final / (double) runs << endl;
    return 0;
}
