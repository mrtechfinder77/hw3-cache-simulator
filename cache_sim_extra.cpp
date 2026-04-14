#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_set>

using namespace std;

struct CacheLine {
    bool valid;
    int tag;
};

struct Cache {
    int num_entries;
    int associativity;
    int num_sets;
    vector<vector<CacheLine>> lines;
    vector<int> replacement_ptr;
};

Cache makeCache(int num_entries, int associativity) {
    Cache cache;
    cache.num_entries = num_entries;
    cache.associativity = associativity;
    cache.num_sets = num_entries / associativity;
    cache.lines = vector<vector<CacheLine>>(
        cache.num_sets,
        vector<CacheLine>(cache.associativity, {false, -1})
    );
    cache.replacement_ptr = vector<int>(cache.num_sets, 0);
    return cache;
}

bool checkHit(Cache& cache, int block_number) {
    int index = block_number % cache.num_sets;
    int tag = block_number / cache.num_sets;

    for (int i = 0; i < cache.associativity; i++) {
        if (cache.lines[index][i].valid && cache.lines[index][i].tag == tag) {
            return true;
        }
    }
    return false;
}

void insertBlock(Cache& cache, int block_number) {
    int index = block_number % cache.num_sets;
    int tag = block_number / cache.num_sets;

    for (int i = 0; i < cache.associativity; i++) {
        if (!cache.lines[index][i].valid) {
            cache.lines[index][i].valid = true;
            cache.lines[index][i].tag = tag;
            return;
        }
    }

    int victim = cache.replacement_ptr[index];
    cache.lines[index][victim].valid = true;
    cache.lines[index][victim].tag = tag;
    cache.replacement_ptr[index] =
        (cache.replacement_ptr[index] + 1) % cache.associativity;
}

int main(int argc, char* argv[]) {
    if (argc != 7) {
        cerr << "Usage: .\\cache_sim_extra.exe <L1_entries> <L1_assoc> <L2_entries> <L2_assoc> <block_size> <input_file>" << endl;
        return 1;
    }

    int l1_entries = stoi(argv[1]);
    int l1_assoc = stoi(argv[2]);
    int l2_entries = stoi(argv[3]);
    int l2_assoc = stoi(argv[4]);
    int block_size = stoi(argv[5]);
    string input_file = argv[6];

    if (l1_entries <= 0 || l1_assoc <= 0 || l1_entries % l1_assoc != 0 ||
        l2_entries <= 0 || l2_assoc <= 0 || l2_entries % l2_assoc != 0 ||
        block_size <= 0) {
        cerr << "Invalid cache configuration." << endl;
        return 1;
    }

    Cache l1 = makeCache(l1_entries, l1_assoc);
    Cache l2 = makeCache(l2_entries, l2_assoc);

    // Fully associative reference cache for miss classification
    Cache fa = makeCache(l1_entries, l1_entries);

    unordered_set<int> seen_blocks;

    ifstream fin(input_file);
    ofstream fout("cache_sim_extra_output");

    if (!fin.is_open()) {
        cerr << "Error opening input file." << endl;
        return 1;
    }

    int address;

    while (fin >> address) {
        int block_number = address / block_size;

        if (checkHit(l1, block_number)) {
            fout << address << " : HIT L1" << endl;
        }
        else if (checkHit(l2, block_number)) {
            fout << address << " : HIT L2" << endl;
            insertBlock(l1, block_number);
        }
        else {
            bool first_time = (seen_blocks.find(block_number) == seen_blocks.end());

            if (first_time) {
                fout << address << " : MISS COMPULSORY" << endl;
                seen_blocks.insert(block_number);
            }
            else if (checkHit(fa, block_number)) {
                fout << address << " : MISS CONFLICT" << endl;
            }
            else {
                fout << address << " : MISS CAPACITY" << endl;
            }

            insertBlock(fa, block_number);
            insertBlock(l2, block_number);
            insertBlock(l1, block_number);
        }
    }

    fin.close();
    fout.close();

    return 0;
}