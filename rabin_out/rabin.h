#ifndef _RABIN_H
#define _RABIN_H

#include <stdint.h>
#include <stdio.h>

#define POLYNOMIAL 0x3DA3358B4DC173LL
#define POLYNOMIAL_DEGREE 53
#define WINSIZE 128
#define AVERAGE_BITS 13
#define MINSIZE (4*1024)
#define MAXSIZE (16*1024)

struct rabin_t {
    uint8_t window[WINSIZE];
    unsigned int wpos;
    unsigned int count;
    unsigned int pos;
    unsigned int start;
    uint64_t digest;
};

struct chunk_t {
    unsigned int start;
    unsigned int length;
    uint64_t cut_fingerprint;
};

extern struct chunk_t last_chunk;

struct rabin_t *rabin_init(void);
void rabin_reset(struct rabin_t *h);
void rabin_slide(struct rabin_t *h, uint8_t b);
void rabin_append(struct rabin_t *h, uint8_t b);
int rabin_next_chunk(struct rabin_t *h, uint8_t *buf, unsigned int len);
struct chunk_t *rabin_finalize(struct rabin_t *h);

#endif
