#include <cstdlib>
#include <fstream>
#include <iostream>
#include <random>

#include "smti.h"

int main(int argc, char *argv[]) {
  //int seed = 24601;
  //std::mt19937 g(seed);
  std::random_device rd;
  unsigned int seed = rd();
  if (argc == 6) {
    seed = atoi(argv[5]);
  }
  std::mt19937 g(seed);
  SMTI instance(atoi(argv[1]), atoi(argv[2]), atof(argv[3]), g);
  std::string fname(argv[4]);
  std::ofstream humanfile(fname + ".instance");
  humanfile << instance.to_string();
  humanfile.close();
  std::ofstream wcnffile(fname + ".pbo");
  wcnffile << instance.encodePBO();
  wcnffile.close();
  std::ofstream wcnffile2(fname + "-v2.pbo");
  wcnffile2 << instance.encodePBO2();
  wcnffile2.close();
  std::ofstream wcnffile3(fname + "-v3.pbo");
  wcnffile3 << instance.encodePBO2(true);
  wcnffile3.close();
#ifdef CPLEX_FOUND
  double size = instance.solve_cplex(true, false);
  std::cout << "unmerged found " << size << " expect objective " << (2*(atoi(argv[1]) - size)) << std::endl;
  size = instance.solve_cplex(true, true);
  std::cout << "merged found " << size << " expect objective " << (2*(atoi(argv[1]) - size)) << std::endl;
#endif
  return 0;
}
