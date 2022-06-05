#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/time.h>
#include "rabin.h"

// 1MiB buffer
size_t bytes;
const uint64_t SEC_2_US = 1000 * 1000;

/**
  * @brief Get the Time Diff object
  * 
  * @param start_time start time
  * @param end_time end time
  * @return double the diff time (sec)
  */
 double GetTimeDiff(struct timeval start_time, struct timeval end_time) {
     double second;
     second = (double)(end_time.tv_sec - start_time.tv_sec) * SEC_2_US + 
         end_time.tv_usec - start_time.tv_usec;
     second = second / SEC_2_US;
     return second; 
 }

int main(int argc, char* argv[]) {
    struct rabin_t *hash;
    hash = rabin_init();

    uint8_t* buf;
    unsigned int chunks = 0;
    FILE* input_file = fopen(argv[1], "rb");
    uint64_t read_size = 128 * 1024 * 1024;
    buf = (uint8_t*) malloc(read_size);

    struct timeval stime;
    struct timeval etime;
    double total_time = 0;
    while (!feof(input_file)) {
        size_t len = fread(buf, 1, read_size, input_file);
        uint8_t *ptr = buf;

        bytes += len;
        gettimeofday(&stime, NULL);
        // while (1) {
        //     int remaining = rabin_next_chunk(hash, ptr, len);

        //     if (remaining < 0) {
        //         break;
        //     }

        //     len -= remaining;
        //     ptr += remaining;

        //     // printf("%d %016llx\n",
        //         // last_chunk.length,
        //         // (long long unsigned int)last_chunk.cut_fingerprint);
        //     // fprintf(stdout, "%u\n", last_chunk.length);

        //     chunks++;
        // }
        for (size_t i = 0; i < len; i++) {
            rabin_slide(hash, buf[i]);
            fprintf(stdout, "%lu\n", hash->digest);
        }
        gettimeofday(&etime, NULL);
        total_time += GetTimeDiff(stime, etime);
    }

    fprintf(stderr, "speed (MiB/s): %lf\n", (double)(bytes) / 1024.0 / 1024.0 / total_time);
    fprintf(stderr, "mask: %u\n", ((1<<AVERAGE_BITS)-1));


    if (rabin_finalize(hash) != NULL) {
        chunks++;
        // printf("%d %016llx\n",
            // last_chunk.length,
            // (long long unsigned int)last_chunk.cut_fingerprint);
        fprintf(stdout, "%u\n", last_chunk.length);
    }

    unsigned int avg = 0;
    if (chunks > 0)
        avg = bytes / chunks;
    printf("%d chunks, average chunk size %d\n", chunks, avg);

    free(buf);
    fclose(input_file);
    return 0;
}