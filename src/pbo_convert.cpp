#include <cstdlib>
#include <fstream>
#include <iostream>
#include <random>

#include "smti.h"

int main(int argc, char *argv[]) {
  if (argc != 3) {
    std::cout << "Usage: convert_pbo <instance_file> <output_file>" << std::endl;
    return 1;
  }
  SMTI instance(argv[1]);
  std::ofstream wcnffile2(argv[2]);
  wcnffile2 << instance.encodePBO2();
  wcnffile2.close();
  return 0;
}
