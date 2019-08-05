#include "lattice.h"

#include <set>



std::map<pair, Relation> build_comparisons(const std::vector<const Agent> & agents) {
  std::map<pair, Relation> relations;
  for(auto & agent: agents) {
    std::set<int> found;
    for(auto & group: agent.preferences()) {
      for(auto & earlier: found) {
        for(auto & now: group) {
          {
            pair these(earlier, now);
            if (relations.find(these) == relations.end()) {
              relations.emplace(these);
            }
            relations[these].under = false;
            relations[these].tied = false;
          }
          {
            pair these(now, earlier);
            if (relations.find(these) == relations.end()) {
              relations.emplace(these);
            }
            relations[these].over = false;
            relations[these].tied = false;
          }
        }
      }
      for(auto & now: group) {
        found.insert(now);
      }
    }
  }
  return relations;
}

Lattice::Lattice(const SMTI & instance) {
  std::map<pair, Relation> left_relations = build_comparisons(instance.right_agents());
  // Step 2, go over pairs, union find things if over = under = false, but tied = true
  // Step 3, go over triples, mark rels as implied or incomparable
}

