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
} // namespace std


class SMTI {
  public:

    /**
     * Construct an instance from a file.
     */
    explicit SMTI(std::string filename);

    /**
     * Construct an instance using the given parameters.
     */
    SMTI(int size, int pref_length, float tie_density, std::mt19937 & generator);

    /**
     * Return a string representation of this instance.
     */
    std::string to_string() const;

    /**
     * Adds dummy variables to the instance.  We add num_dummy agents to either
     * side, and each dummy finds every agent of the other side equally
     * preferable. In other words, in an instance with N regular agents per
     * side and X dummies, we will find a matching that finds at least (N-X)
     * pairings of agents.
     *
     * param num_dummy
     */
    void add_dummy(int num_dummy);

    /**
     * Removes dummy variables from this instance. Note that there is currently
     * no safety checking: if you attempt to remove more dummies than you have
     * added, unexpected behaviour (most likely a crash) will occur.
     */
    void remove_dummy(int num_dummy);

    /**
     * Create a SAT encoding of the instance.
     */
    std::string encodeSAT();

    /**
     * Create a Weighted Partial MaxSAT encoding of the instance.
     */
    std::string encodeWPMaxSAT();

    /**
     * Create a pseudo-boolean optimisation encoding of the instance.
     */
    std::string encodePBO();

    /**
     * Find a maximum sized stable matching.
     */
    void solve() const;

  private:
    /**
     * Create the maps from IDs/positions in preference lists to variable
     * indices. Indices, and therefore variables, start at 1 in the land of
     * SAT.
     */
    void make_var_map();

    int _size;
    int _num_dummies;
    std::vector<Agent> _ones;
    std::vector<Agent> _twos;
    std::unordered_map<std::tuple<int,int>, int> _one_vars;
    std::unordered_map<std::tuple<int,int>, int> _two_vars;

};

#endif /* SMTI_H */
