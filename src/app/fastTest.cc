#include "../../include/fastChunking.h"
#include "../../include/define.h"

using namespace std;

int main(int argc, char *argv[]) {
  string logger_name = "my_test";
  tool::Logging(logger_name.c_str(), "Test\n");
  return 0;
}
