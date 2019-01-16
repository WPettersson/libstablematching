#include <cstdlib>
#include <fstream>
#include <iostream>
#include <random>

#include "smti.h"

int main(int argc, char *argv[]) {
  //int seed = 24601;
  //std::mt19937 g(seed);
  std::random_device rd;
  std::mt19937 g(rd());
  SMTI instance(atoi(argv[1]), atoi(argv[2]), atof(argv[3]), g);
  std::string fname(argv[4]);
  std::ofstream humanfile(fname + ".instance");
  humanfile << instance.to_string();
  humanfile.close();
  std::ofstream wcnffile(fname + ".wcnf");
  wcnffile << instance.encodeWPMaxSAT();
  wcnffile.close();
  return 0;
}
