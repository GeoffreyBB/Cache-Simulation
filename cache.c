#include <stdio.h>
#include <stdlib.h>
#include "cache.h"

void print_cache(cache_t cache) {
    int S = 1 << cache.s;
    int B = 1 << cache.b;

    //iterates over each set
    for (int i = 0; i < S; i++) {
        //prints the set number
        printf("Set %d\n", i);
        //iterates over each line in the set
        for (int j = 0; j < cache.E; j++) {
            //prints cache line info, prints the valid bit, frequency, and tag of the current line
            printf("%1d %d 0x%0*lx ", cache.cache[i][j].valid,
                cache.cache[i][j].frequency, cache.t, cache.cache[i][j].tag);
            //iterates over each byte in the current block of the cache line
            for (int k = 0; k < B; k++) {
                //prints block data
                printf("%02x ", cache.cache[i][j].block[k]);
            }
            puts("");
        }
    }
}

//allocating memory for each parameter of the cache, and initiliazing it to 0
cache_t initialize_cache(uchar s, uchar t, uchar b, uchar E) {
    cache_t cache;
    cache.s = s; // set bits
    cache.t = t; // tag bits
    cache.b = b; // block bits
    cache.E = E; // number of blocks per line

    //number of sets and blocks based on the input
    int numSets = 1 << s; // bitwise to get 2^s
    int numBlocks = 1 << b; // bitwise to get 2^b

    //allocating memory for the sets in a cache
    cache.cache = (cache_line_t**) malloc((numSets * E) * sizeof(cache_line_t*)); // added * E on 16/7 @ 6:45 (see if it is correct)

    //allocating memory for the lines in each set
    for (int i = 0; i < numSets; i++) {
        cache.cache[i] = (cache_line_t*) malloc(E * sizeof(cache_line_t));

        for(int j = 0; j < E; j++) {
            cache.cache[i][j].valid = 0;
            cache.cache[i][j].frequency = 0;
            cache.cache[i][j].tag = 0; 

            //allocating memory for each block
            cache.cache[i][j].block = (uchar*) malloc(numBlocks * sizeof(uchar));
            for (int k = 0; k < numBlocks; k++) {
                cache.cache[i][j].block[k] = 0;
            }
        }
    }
    return cache;
}


uchar read_byte(cache_t cache, uchar* start, long int off) {
    //sets the address to the correct length
    int m = cache.s + cache.t + cache.b;
    long int address = off & ((1 << m) -1);
    int setIndex = (address >> cache.b) & ((1 << cache.s) - 1);
    long int tag = address >> (cache.s + cache.b);
    int blockOffset = address & ((1 << cache.b) - 1); 

    //Finds the lfu line
    int lfuIndex = 0;
    int startFreq = (int)cache.cache[setIndex][0].frequency;
    for (int i = 0; i < cache.E; i++) {
        if ((int)cache.cache[setIndex][i].frequency < startFreq) {
            lfuIndex = i;
            startFreq = (int)cache.cache[setIndex][i].frequency;           
        }
    }

    for (int i = 0; i < cache.E; i++) {
        //Checks if the line has been written to
        if (cache.cache[setIndex][i].valid == 0) {
            write_byte(cache, start, off, start[off]);
            return cache.cache[setIndex][i].block[blockOffset];
        //If line has been written to, check if it is the line we need
        } else if (cache.cache[setIndex][i].tag == tag) {
            //If so increment frequency by 1
            cache.cache[setIndex][i].frequency++;
            return cache.cache[setIndex][i].block[blockOffset];
        }
    }

    //Only data that has not been previously entered will reach here
    write_byte(cache, start, off, start[off]);
    return cache.cache[setIndex][lfuIndex].block[blockOffset];
}

void write_byte(cache_t cache, uchar* start, long int off, uchar new) {
    //Sets the address to the correct length
    int m = cache.s + cache.t + cache.b;
    long int address = off & ((1 << m) -1);
    int setIndex = (address >> cache.b) & ((1 << cache.s) - 1);
    long int tag = address >> (cache.s + cache.b);
    int blockOffset = address & ((1 << cache.b) - 1);

    //will need to change + 1, it has to do with the # of blocks.
    for (int i = 0; i < cache.E; i++) {
        if (cache.cache[setIndex][i].valid == 0) {
            cache.cache[setIndex][i].valid = 1;
            cache.cache[setIndex][i].frequency++;
            cache.cache[setIndex][i].tag = tag;
            cache.cache[setIndex][i].block[blockOffset] = new;
            cache.cache[setIndex][i].block[blockOffset + 1] = start[off + 1];
            return;
        }
    }

    //Finds the lfu line
    int lfuIndex = 0;
    int startFreq = (int)cache.cache[setIndex][0].frequency;
    for (int i = 0; i < cache.E; i++) {
        if ((int)cache.cache[setIndex][i].frequency < startFreq) {
            lfuIndex = i;
            startFreq = (int)cache.cache[setIndex][i].frequency;           
        }
    }

    //Only data that needs to be overwritten reaches here
    cache.cache[setIndex][lfuIndex].frequency++;
    cache.cache[setIndex][lfuIndex].block[blockOffset] = new; 
    return;   
}