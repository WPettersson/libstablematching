#ifndef SMTI_H
#define SMTI_H

#include <algorithm>
#include <random>
#include <string>
#include <unordered_map>

#include "Agent.h"


// We use a tuple to convert variable subscripts to numbers according to DIMACS
// format, which means we need to hash tuples of two integers.
namespace std {
  template <>
  struct hash<std::tuple<int, int>> {
    size_t operator()(std::tuple<int, int> const& tt) const {
      return std::get<0>(tt) * 500 + std::get<1>(tt);
    }
  };
}



class SMTI {
  public:

    /**
     * Construct an instance from a file.
     */
    //explicit SMTI(std::string filename);
    //
    SMTI(int size, int pref_length, float tie_density, std::mt19937 & generator);

    /**
     * Return a string representation of this instance.
     */
    std::string to_string() const;

    /**
     * Create an encoding of the instance.
     */
    std::string encodeSAT();

    /**
     * Find a maximum sized stable matching.
     */
    void solve() const;

  private:
    void make_var_map();

    std::vector<Agent> _ones;
    std::vector<Agent> _twos;
    std::unordered_map<std::tuple<int,int>, int> _one_vars;
    std::unordered_map<std::tuple<int,int>, int> _two_vars;

};

#endif /* SMTI_H */
