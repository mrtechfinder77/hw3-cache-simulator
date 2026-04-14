#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

struct CacheLine {
    bool valid;
    int tag;
};

int main(int argc, char* argv[]) {
    if (argc != 4) {
        cerr << "Usage: ./cache_sim <num_entries> <associativity> <input_file>" << endl;
        return 1;
    }

    int num_entries = stoi(argv[1]);
    int associativity = stoi(argv[2]);
    string input_file = argv[3];

    if (num_entries <= 0 || associativity <= 0 || num_entries % associativity != 0) {
        cerr << "Invalid cache configuration." << endl;
        return 1;
    }

    int num_sets = num_entries / associativity;

    vector<vector<CacheLine>> cache(num_sets, vector<CacheLine>(associativity, {false, -1}));
    vector<int> replacement_ptr(num_sets, 0);

    ifstream fin(input_file);
    ofstream fout("cache_sim_output");

    if (!fin.is_open()) {
        cerr << "Error opening input file." << endl;
        return 1;
    }

    int address;

    while (fin >> address) {
        int index = address % num_sets;
        int tag = address / num_sets;

        bool hit = false;

        for (int i = 0; i < associativity; i++) {
            if (cache[index][i].valid && cache[index][i].tag == tag) {
                hit = true;
                break;
            }
        }

        if (hit) {
            fout << address << " : HIT" << endl;
        } else {
            fout << address << " : MISS" << endl;

            bool inserted = false;

            for (int i = 0; i < associativity; i++) {
                if (!cache[index][i].valid) {
                    cache[index][i].valid = true;
                    cache[index][i].tag = tag;
                    inserted = true;
                    break;
                }
            }

            if (!inserted) {
                int victim = replacement_ptr[index];
                cache[index][victim].tag = tag;
                cache[index][victim].valid = true;
                replacement_ptr[index] = (replacement_ptr[index] + 1) % associativity;
            }
        }
    }

    fin.close();
    fout.close();

    return 0;
}