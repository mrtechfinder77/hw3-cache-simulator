# Homework 3 - Cache Simulator

## Author
Spencer Johnson

## Description
This program simulates a cache. It takes the total number of cache entries, the associativity, and an input file of memory references. For each memory reference, the program reports whether it is a HIT or a MISS and writes the results to cache_sim_output.

## Build
g++ -o cache_sim cache_sim.cpp

## Run
.\cache_sim.exe <num_entries> <associativity> <input_file>

Example:
.\cache_sim.exe 4 2 input0

## Output
The program writes results to:
cache_sim_output

Each line follows:
[ADDRESS] : HIT
or
[ADDRESS] : MISS

## Extra Credit
An additional file, `cache_sim_extra.cpp`, includes:
- multi-word block support
- two-level cache support (L1 and L2)
- miss classification as compulsory, conflict, or capacity

Example extra credit run:
.\cache_sim_extra.exe 2 1 4 1 4 input_all_types.txt
