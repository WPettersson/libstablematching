#include <cstdlib>
#include <fstream>
#include <iostream>
#include <random>

#include "smti.h"

int main(int argc, char *argv[]) {
  if (argc != 3) {
    std::cout << "Usage: convert_pbo <instance_file> <prefix>" << std::endl;
    return 1;
  }
  SMTI instance(argv[1]);
  std::ofstream wcnffile2(std::string(argv[2]) + "-v2.pbo");
  wcnffile2 << instance.encodePBO2();
  wcnffile2.close();
#ifdef CPLEX_FOUND
  double size = instance.solve_cplex();
  std::cout << "CPLEX found " << size << std::endl;
#endif
  return 0;
}
