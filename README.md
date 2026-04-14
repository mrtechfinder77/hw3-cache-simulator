# Homework 3 - Cache Simulator

## Author
Spencer Johnson

## Description
This program simulates a cache. It takes the total number of cache entries, the associativity, and an input file of memory references. For each memory reference, the program reports whether it is a HIT or a MISS and writes the results to `cache_sim_output`.

## Build
```bash
g++ -o cache_sim cache_sim.cpp