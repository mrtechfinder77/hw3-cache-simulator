# Homework 3 - Cache Simulator

## Author
Spencer Johnson

## Description
This program simulates a cache. It takes the total number of cache entries, the associativity, and an input file of memory references. For each memory reference, the program reports whether it is a HIT or a MISS and writes the results to cache_sim_output.

## Build
g++ -o cache_sim cache_sim.cpp

## Run

Windows (PowerShell):
.\cache_sim.exe 4 2 input0

Linux/Mac:
./cache_sim 4 2 input0

## Output

The program writes results to a file named:
cache_sim_output

Each line of the output file follows this format:
[ADDRESS] : HIT
or
[ADDRESS] : MISS

## Notes
- The cache starts empty (all entries invalid)
- Each cache block holds one word
- A FIFO-style replacement policy is used