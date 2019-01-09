#include <iostream>
#include <random>

#include "smti.h"

int main(int argc, char *argv[]) {
  int seed = 24601;
  std::mt19937 g(seed);
  SMTI instance(2, 2, 0.15, g);
  std::cout << instance.to_string();
  std::cout << instance.encodeSAT();
  return 0;
}
