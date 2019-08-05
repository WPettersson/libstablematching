#include <iostream>
#include <random>

#include "smti.h"

int main(int argc, char *argv[]) {
  if (argc == 1) {
    std::cout << "Usage: convert <instance_file>" << std::endl;
    return 1;
  }
  if (std::string("-d") == argv[1]) {
    int dummies = std::atoi(argv[2]);
    SMTI instance(argv[3]);
    instance.add_dummy(dummies);
    std::cout << instance.encodeSAT();
  } else {
    SMTI instance(argv[1]);
    std::cout << instance.encodeWPMaxSAT();
  }
  return 0;
}
