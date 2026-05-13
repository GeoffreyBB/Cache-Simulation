# Cache Simulation

A cache simulator written in C that models cache behavior and evaluates memory access performance under different cache configurations and replacement policies.

## Overview

This project simulates how a CPU cache operates by processing memory access patterns and tracking cache hits, misses, and evictions. It was developed as part of a Computer Architecture course to explore low-level memory systems and cache optimization techniques.

The simulator supports configurable cache parameters and demonstrates how cache structure impacts performance.

## Features

- Simulates set-associative caches
- Configurable:
  - Cache size
  - Block size
  - Number of sets
  - Associativity
- Tracks:
  - Cache hits
  - Cache misses
  - Evictions
- Processes memory trace files
- Implements LRU (Least Recently Used) replacement policy
- Command-line interface for running simulations

## Technologies Used

- C
- GCC
- Makefile
- Linux command-line tools

## Project Structure

```text
.
├── cache.c
├── cache.h
├── main.c
├── Makefile
└── traces/
