#include <cstdlib>
#include <fstream>
#include <iostream>
#include <random>
#include <string>

#include "smti.h"

int main(int argc, char *argv[]) {
  //int seed = 24601;
  //std::mt19937 g(seed);
  SMTI instance = SMTI::create_from_GRP(argv[1], std::stoi(argv[2]));
  instance.preprocess(SMTI::PreprocessMode::Complete);
  std::ofstream pped(argv[3]);
  pped << instance.to_string(std::string(" "), std::string("("), std::string(")"));
  pped.close();
  return 0;
}
