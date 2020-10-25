#include <stdint.h>

typedef struct block_info {
    uint64_t PR_4bit;
    uint64_t PB_4bit;
    uint64_t a;
    int64_t b;
    int64_t c;
    int64_t d;
} *block_info;