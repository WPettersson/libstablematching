/**
 * This file contains the preprocessing algorithms for stable matching
 * problems. The only externally accessible function is SMTI::preprocess(),
 * which is defined at the bottom of this page, but most of the meat of the
 * algorithms exists in an anonymous namespace declared first.
 */

#include <unordered_set>
#include <iostream>
#include "Agent.h"
#include "Graph.h"
#include "smti.h"

namespace {
  /*
   * Perform one reduction. Returns the number of preferences removed, or -1 if
   * no preferences were removed but a new agent was marked as "always
   * allocated".
   */
int single_reduction(std::unordered_map<int, Agent> & to_preprocess,
                     std::unordered_map<int, Agent> & other_side,
                     std::unordered_set<int> & these_always_allocated,
                     std::unordered_set<int> & other_always_allocated,
                     bool supp) {
  int num_removed = 0;
  bool new_always_allocated = false;
  for (auto & pair : to_preprocess) {
    auto & agent = pair.second;
    Graph g;
    int n_1 = 0;
    for (auto rank = 0; rank < agent.preferences().size(); ++rank) {
      auto &pref_tie = agent.preference_group(rank);
      // No point in checking the last rank if we already know this agent must
      // be allocated, or if we don't care about P'
      if ((pref_tie == agent.preferences().back()) &&
          (!supp || these_always_allocated.find(agent.id()) !=
                        these_always_allocated.end())) {
        continue;
      }
      for (size_t ind = 0; ind < pref_tie.size(); ind++) {
        int position = pref_tie[ind];
        g.addVertex(1, position);
        Agent &other = other_side.at(position);
        for (int l = 0; l <= other.rank_of(agent.id()); l++) {
          for (size_t k = 0; k < other.preferences()[l].size(); k++) {
            int other_cand = other.preference_group(l)[k];
            if (other_cand == agent.id()) { // Don't add the current candidate to the graph
              continue;
            }
            if (!g.containsVertex(0, other_cand)) {
              g.addVertex(0, other_cand);
              n_1 += 1;
            }
            g.addEdge(position, other_cand);
          }
        }
      }
      // If n_1 is sufficiently small, then the largest matching must also be
      // small, as the matching can use each vertex from n_1 at most once, so
      // we don't even need to try to find a bigger matching.
      bool matching_cant_exist = (2 * n_1 + 1 <= g.size());
      if (!matching_cant_exist) {
        for (auto position : agent.preference_group(rank)) {
          g.augment(position);
        }
      }
      // Add P' in this, which we only do on the first iteration (rank == 0)
      // Yes, I'm abusing while statements, so I can break out easier.
      while (rank == 0) {
        // Don't add P' if we don't need to.
        if (matching_cant_exist || (g.size() - g.matchingSize() >= n_1 + 1)) {
          break;
        }
        // First add all positions that must be filled.
        for (int position : other_always_allocated) {
          // If position is acceptable to i, then skip it.
          if (other_side.at(position).is_compatible(agent))
            continue;
          g.addVertex(1, position);
          for (auto &group : other_side.at(position).preferences()) {
            for (auto candidate : group) {
              if (!g.containsVertex(0, candidate)) {
                g.addVertex(0, candidate);
                n_1 += 1;
              }
              g.addEdge(position, candidate);
            }
          }
          g.augment(position);
        }
        // I'm using a while loop as an if statement, so I need to break out.
        break;
      }
      if (matching_cant_exist || (g.size() - g.matchingSize() >= n_1 + 1)) {
        // preprocess on rank!
        // Firstly, they must be allocated, so mark as such (if we're in that
        // mode)
        if (supp && these_always_allocated.find(agent.id()) ==
                        these_always_allocated.end()) {
          these_always_allocated.insert(agent.id());
          new_always_allocated = true;
        }
        // Now remove entries from preference lists after this rank.
        auto removed = agent.remove_after(rank);
        for (auto other_id : removed) {
          other_side.at(other_id).remove_preference(agent.id());
        }
        break;
      }
    }
  }
  if ((num_removed == 0) && (new_always_allocated)) {
    return -1;
  }
  return num_removed;
}
}

void SMTI::preprocess(PreprocessMode mode) {
  std::unordered_set<int> left_always_assigned, right_always_assigned;
  bool keep_going = true;
  while (keep_going) {
    keep_going = false;
    int removed = single_reduction(_ones, _twos,
                                  left_always_assigned,
                                  right_always_assigned,
                                  mode == Complete);
    if (removed == -1) {
      keep_going = true;
      removed = 0;
    }
    int right = single_reduction(_twos, _ones,
                                 right_always_assigned,
                                 left_always_assigned,
                                 mode == Complete);
    if (right == -1) {
      keep_going = true;
      right = 0;
    }
    removed += right;
    if (removed > 0) {
      keep_going = true;
    }
  }
}
