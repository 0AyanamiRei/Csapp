#include "cachelab.h"
#include <getopt.h>
#include <stdlib.h> 
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>

#define TRUE 1
#define FALSE 0

const int m = 64;
int s, E, b, t, B, S;
char *file_name = NULL; 
int hits = 0, misses = 0, evictions = 0;
int global_LRU_time = 0;

typedef struct cache_line {
    char valid;
    int tag;
    int LRU;
    // int cache_block; B=2^b  useless
}cache_line;

void get_opt(int argc, char** argv)
{
    const char* optstr = "s:E:b:t:";
    char ch;
    while ( (ch = getopt(argc, argv, optstr)) != -1 )
    {
        switch (ch) 
        {
            case 's':
                s = atoi(optarg);
                break;
            case 'E':
                E = atoi(optarg);
                break;
            case 'b':
                b = atoi(optarg);
                break;
            case 't':
                file_name = optarg;
                break;
            case '?':
                printf("ERROR!!!  unknown option: %c\n", optopt);
                break;
        }
    }
    B = 1 << b;
    S = 1 << s;
    t = m - s - b;
}

cache_line ** malloc_for_cache(const int S, const int E){
    cache_line **cache=(cache_line **)malloc(sizeof(cache_line *) * S);
    for (int i = 0; i < S; i++)
        {
            cache[i] = (cache_line *)malloc(sizeof(cache_line) * E);
            for (int j = 0; j < E; j++)
            {
                cache[i][j].valid = 0;
                cache[i][j].tag = ~0;
                cache[i][j].LRU = 0;
            }
        }
    return cache;
}

void free_for_cache(cache_line** cache, const int S, const int E){
    for(int i = 0; i < S; i ++)
            free(cache[i]);
}

FILE* file_open(char* file_name){
    FILE* fptr = fopen(file_name, "r");
    if(fptr == NULL) {
        printf("Error!");
        exit(1);
    }
    return fptr;
}

void data_load(cache_line** cache, int tag, int set){
    char conflict_miss = TRUE;
    int cache_line;
    int minLRU_line = 0;

    for(cache_line = 0; cache_line < E; cache_line ++){
        if(cache[set][cache_line].valid == 1 && cache[set][cache_line].tag == tag){
            hits++;
            conflict_miss = FALSE;
            break;
        }

        if(cache[set][cache_line].valid == 0 && cache[set][cache_line].tag == ~0){
            misses++;
            conflict_miss = FALSE;
            break;
        }

        minLRU_line = cache[set][minLRU_line].LRU > cache[set][cache_line].LRU ? cache_line : minLRU_line;
    }

    if(conflict_miss){
        evictions ++;
        misses++;
        cache_line = minLRU_line;
    }
    cache[set][cache_line].LRU = global_LRU_time;
    cache[set][cache_line].tag = tag;
    cache[set][cache_line].valid = 1;
}

void caching(FILE* fptr, cache_line** cache)
{
    char identifier;
    unsigned address;   
    int size;
    while (fscanf(fptr, " %c %x,%d", &identifier, &address, &size) > 0)
    {
        global_LRU_time ++;
        int tag_bytes = address >> (s + b);
        int set_index = (address >> b) & (~(~0 << s));
        switch (identifier)
        {
        case 'M':
           data_load(cache, tag_bytes, set_index);
           data_load(cache, tag_bytes, set_index);
            break;
        case 'L':
           data_load(cache, tag_bytes, set_index);
            break;
        case 'S':
           data_load(cache, tag_bytes, set_index);
            break;
        }
    }
}
int main(int argc, char** argv)
{
    get_opt(argc, argv);
    cache_line **cache=malloc_for_cache(S, E);
    FILE* fptr = file_open(file_name);
    caching(fptr, cache);
    printSummary(hits, misses, evictions);
    free_for_cache(cache, S, E);
    fclose(fptr);
    return 0;
}