#include <stdio.h>
#include <stdlib.h>

#include "../../include/fastChunking.h"
#include "../../include/define.h"

int main(int argc, char *argv[]) {
  struct timeval startTime;
  struct timeval endTime;
  chunk_vec cv;
  FILE *s = fopen(argv[1], "r");
  if (!s)
    exit(-1);
  kv_init(cv);
  gettimeofday(&startTime, NULL);
  fastcdc_stream(s, 4 * 1024, 8 * 1024, 12 * 1024, &cv);
  gettimeofday(&endTime, NULL);
  double totalTime = 0;
  totalTime = tool::GetTimeDiff(startTime, endTime);
  printf("Total time: %.4lf\n", totalTime);
//   printf("%8s|%8s\n", "offset", "length");
//   for (int i = 0; i < kv_size(cv); i++)
    // printf("%08lu|%08lu\n", kv_A(cv, i).offset, kv_A(cv, i).len);
  kv_destroy(cv);
  fclose(s);
  return 0;
}
