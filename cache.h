typedef unsigned char uchar;

typedef struct cache_line_s {
uchar valid;
uchar frequency;
long int tag;
uchar* block;
} cache_line_t;

typedef struct cache_s {
uchar s; //number of sets S = 2^s
uchar t; //tag length
uchar b; //number of blocks per line in the set B = 2^b
uchar E; //number of lines per set 
cache_line_t** cache;
} cache_t;

void print_cache(cache_t cache);
cache_t initialize_cache(uchar s, uchar t, uchar b, uchar E);
uchar read_byte(cache_t cache, uchar* start, long int off);
void write_byte(cache_t cache, uchar* start, long int off, uchar new);