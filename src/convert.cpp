#include <iostream>
#include <random>

#include "smti.h"

int main(int argc, char *argv[]) {
  if (argc == 1) {
    std::cout << "Usage: convert <instance_file>" << std::endl;
    return 1;
  }
  SMTI instance(argv[1]);
  std::cout << instance.encodeWPMaxSAT();
  return 0;
}
