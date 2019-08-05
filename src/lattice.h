#ifndef LATTICE_H
#define LATTICE_H

#include <map>
#include <utility>
#include <vector>

#include "smti.h"

enum LatticeComparisons {
  Above,
  Below,
  Tied,
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
    Lattice(const SMTI & instance);

  private:
    std::map<int, int> comparisons;


};

#endif /* LATTICE_H */
