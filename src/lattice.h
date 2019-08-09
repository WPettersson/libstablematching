#ifndef LATTICE_H
#define LATTICE_H

#include <list>
#include <map>
#include <utility>
#include <vector>

#include "smti.h"

enum LatticeComparisons {
  Above,
  Below,
  ImpliedAbove,
  ImpliedBelow,
  Incomparable
};

/**
 * A tracker of possible relationships between two agents.
 */
class Relation {
  public:
    Relation() : over(true), under(true), tied(true) { }

    // Remember, these are all for a pair <x, y>
    // Can x be over y?
    bool over;
    // Can y be under x?
    bool under;
    // Can x and y be tied?
    bool tied;
};

/**
 * A pair of agents.
 */
typedef std::pair<int,int> pair;

class Lattice {
  public:
    Lattice(const std::vector<Agent> agents);

    /**
     * Get a list of the widths of each rung in the lattice, where
     * a rung is a set of mutually incomparable vertices.
     */
    std::vector<int> getWidths() const;

  private:
    /**
     * The vertices in the lattice. Each vertex has a "head" agent, somewhat
     * arbitrarily decided, and then a list of other agents which are exactly
     * equivalent (always tied).
     */
    std::map<int, std::list<int>> vertices;
    /**
     * A pair is two agent IDs (a, b) who are the "head" agents of each
     * respective vertex (a > b), and the comparison (see LatticeComparisons
     * above) is the relationshop between a and b. For instance, if the
     * comparison is Above, that implies that a is Above b.
     */
    std::map<pair, LatticeComparisons> comparisons;
    /**
     * A list of widths of various incomparable blobs?
     */
    std::vector<int> widths;


};

#endif /* LATTICE_H */
