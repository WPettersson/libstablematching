#ifndef SMTI_H
#define SMTI_H

#include <algorithm>
#include <random>
#include <string>
#include <unordered_map>

#ifdef CPLEX_FOUND
// CPLEX Include
#include <ilcplex/ilocplex.h>
// CPLEX Needs this before using STL things
ILOSTLBEGIN

// The type for our variable storage.
#include <map>
typedef std::map<int, std::map<int, IloBoolVar*>> VarMap;

#endif

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
     * Construct an instance given two sets of preference lists.
     */
    SMTI(const std::vector<std::vector<std::vector<int>>> & ones, const std::vector<std::vector<std::vector<int>>> & twos);

    /**
     * Construct an instance from a file containing scores of Globally Ranked Pairs.
     * param threshold Assume any scores below this threshold
     */
    static SMTI create_from_GRP(std::string filename, int threshold=0);

    /**
     * Return a string representation of this instance.
     */
    std::string to_string(std::string id_sep = ":", std::string bracket_start = "[", std::string bracket_end = "]") const;

    /**
     * Return the number of agents on the left.
     */
    int agents_left() const { return _ones.size(); } ;

    /**
     * Return the number of agents on the right.
     */
    int agents_right() const { return _twos.size(); } ;

    /**
     * Return a given agent.
     */
    const Agent agent_left(int id) const { return _ones.at(id); } ;

    /**
     * Return a given agent.
     */
    const Agent agent_right(int id) const { return _twos.at(id); } ;

    /**
     * Modes for preprocessing.
     * Quick: Graph based algorithm from TODO REF
     * Complete: Graph based algorithm from TODO
     */
    enum PreprocessMode { Quick, Complete };

    /**
     * Perform the preprocessing according to the above mode.
     */
    void preprocess(PreprocessMode mode);

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
     * Create a pseudo-boolean optimisation encoding of the instance, with a PB-solver-friendly encoding.
     */
    std::string encodePBO2(bool merged=false);

    /**
     * Create a Minizinc constraint programming encoding of the instance.
     *
     * param optimise If true, try to optimise the size. If false, search for a
     * complete matching.
     */
    std::string encodeMZN(bool optimise=false);

#ifdef CPLEX_FOUND
    /**
     * Formulates the problem as an IP optimisation problem, and solves it
     * using CPLEX.
     *
     * :param optimise: If false, create a COM-SMTI instance which requires
     * everyone to be in a matching. Note that this assumes that the number of
     * agents on either side is equal, it does not do this check for you.
     * :param merged: Should we use merged stability constraints?
     * :return: The number of matchings in the optimal solution.
     */
    double solve_cplex(bool optimise=true, bool merged=false);

    /**
     * Adds stability constraints to the model. One constraints is generated
     * for each possibly matched pair.
     */
    void cplex_add_single_constraints(IloEnv * env, IloModel * model,
                                      const VarMap &lr, const VarMap &rl);

    /**
     * Adds merged stability constraints. See Section 6.1 of
     * https://doi.org/10.1016/j.ejor.2019.03.017
     */
    void cplex_add_merged_constraints(IloEnv * env, IloModel * model,
                                      const VarMap &lr, const VarMap &rl);

#endif

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
