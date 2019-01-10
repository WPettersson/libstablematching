#include <iostream>
#include <random>

#include "smti.h"

int main(int argc, char *argv[]) {
  int seed = 24601;
  std::mt19937 g(seed);
  SMTI instance(10, 5, 0.75, g);
  std::cout << instance.to_string();
  instance.add_dummy(1);
  std::cout << instance.to_string();
  instance.add_dummy(3);
  std::cout << instance.to_string();
  instance.remove_dummy(2);
  std::cout << instance.to_string();
  instance.remove_dummy(2);
  std::cout << instance.to_string();
  std::cout << instance.encodeWPMaxSAT();
  SMTI instance_two("test.instance");
  std::cout << instance_two.to_string();
  std::cout << instance_two.encodeSAT();
  std::cout << instance_two.encodeWPMaxSAT();
  return 0;
}
