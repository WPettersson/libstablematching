#include "lattice.h"

#include <set>

#include "union_find.hpp"


std::map<pair, Relation> build_comparisons(const std::vector<Agent> & agents) {
  std::map<pair, Relation> relations;
  for(auto & agent: agents) {
    std::set<int> found;
    for(const auto & group: agent.preferences()) {
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

Lattice::Lattice(const std::vector<Agent> agents) {
  std::map<pair, Relation> relations = build_comparisons(agents);
  UnionFind<int> nodes;
  // Step 2, go over pairs, union find things if over = under = false, but tied = true
  for(const Agent & agent: agents) {
    for(const Agent & other: agents) {
      // Only consider distinct pairs, and only consider each pair once.
      if (agent.id() <= other.id()) {
        continue;
      }
      pair these(agent.id(), other.id());
      // If one is always better/worse, they aren't similar.
      if ((relations[these].under == true) || (relations[these].over == true)) {
        continue;
      }
      // If the two might be tied, then they must always be tied (when both are
      // present.
      if (relations[these].tied == true) {
        nodes.add_element(agent.id(), other.id());
      }
    }
  }
  // Step 1b.
  // Build up the vertices of my lattice-graph thing.
  for(const auto & node: nodes.all_elements()) {
    int id = node.first;
    int parent = nodes.find_element(id);
    if (vertices.find(parent) == vertices.end()) {
      vertices.emplace(std::piecewise_construct,
                       std::forward_as_tuple(parent),
                       std::forward_as_tuple());
    }
    vertices[parent].push_back(id);
  }

  // Step 2a. Determine relationships between nodes in union find. Only work
  // out Above, Below, or Incomparable.
  for(const auto & nodeA: vertices) {
    int parentA = nodeA.first;
    for(const auto & nodeB: vertices) {
      int parentB = nodeB.first;
      // Only consider distinct pairs, and only consider each pair once.
      if (parentA <= parentB) {
        continue;
      }
      pair these(parentA, parentB);
      // Can everything in node A be above everything in node B?
      // Also need at least one a,b in AxB such that a cannot be below b
      bool aAlwaysAboveB = true;
      bool aNotBelowB = false;
      for(int a: nodeA.second) {
        for(int b: nodeB.second) {
          pair ab(a, b);
          if (relations[ab].under == false) {
            aNotBelowB = true;
          }
          if (relations[ab].over == false) {
            aAlwaysAboveB = false;
          }
          if (!aAlwaysAboveB) {
            break;
          }
        }
        if (!aAlwaysAboveB) {
          break;
        }
      }
      // Mark as A above B, and then go to next vertex (aka list of IDs)
      if (aAlwaysAboveB && aNotBelowB) {
        comparisons[these] = Above;
        continue;
      }
      // Can everything in node A be below everything in node B?
      // Also need at least one a,b in AxB such that a cannot be above b
      bool bAlwaysAboveA = true;
      bool bNotBelowA = false;
      for(int a: nodeA.second) {
        for(int b: nodeB.second) {
          pair ab(a, b);
          if (relations[ab].under == false) {
            bAlwaysAboveA = false;
          }
          if (relations[ab].over == false) {
            bNotBelowA = true;
          }
          if (!bAlwaysAboveA) {
            break;
          }
        }
        if (!bAlwaysAboveA) {
          break;
        }
      }
      if (bAlwaysAboveA && bNotBelowA) {
        comparisons[these] = Below;
        continue;
      }
      // Neither of the above, so must be incomparable.
      comparisons[these] = Incomparable;
    }
  }
  // Step 3, go over triples, mark rels as implied or incomparable
  for(const auto & nodeA: nodes.all_elements()) {
    int agentA = nodeA.first;
    for(const auto & nodeB: nodes.all_elements()) {
      int agentB = nodeB.first;
      // Only consider distinct pairs, and only consider each pair once.
      if (agentA <= agentB) {
        continue;
      }
      for(const auto & nodeC: nodes.all_elements()) {
        int agentC = nodeC.first;
        if (agentB <= agentC) {
          continue;
        }
        // At this point, agentA > agentB > agentC
        pair pairAB(agentA, agentB);
        pair pairAC(agentA, agentC);
        pair pairBC(agentB, agentC);
        // If A > B and B > C and A > C, then we can say that A > C is implied,
        // which means less edges in our lattice. Not sure if good or bad idea??
        if ((comparisons[pairAB] == Above) &&
            (comparisons[pairBC] == Above) &&
            (comparisons[pairAC] == Above)) {
          comparisons[pairAC] = ImpliedAbove;
        }
        if ((comparisons[pairAB] == Below) &&
            (comparisons[pairBC] == Below) &&
            (comparisons[pairAC] == Below)) {
          comparisons[pairAC] = ImpliedBelow;
        }
        if ((comparisons[pairAB] == Incomparable) &&
            ((comparisons[pairBC] == Above) || (comparisons[pairBC] == ImpliedAbove)) &&
            ((comparisons[pairAC] == Below) || (comparisons[pairAC] == ImpliedBelow))) {
          comparisons[pairBC] = Incomparable;
          comparisons[pairAC] = Incomparable;
        }
        if ((comparisons[pairAB] == Incomparable) &&
            ((comparisons[pairBC] == Below) || (comparisons[pairBC] == ImpliedBelow)) &&
            ((comparisons[pairAC] == Above) || (comparisons[pairAC] == ImpliedAbove))) {
          comparisons[pairBC] = Incomparable;
          comparisons[pairAC] = Incomparable;
        }
      }
    }
  }
  // Step 4 - Calculate widths
  std::set<int> visited;
  for(const auto & vertex: vertices) {
    int header = vertex.first;
    if (visited.find(header) != visited.end()) {
      // Already found the rung that contains this vertex.
      continue;
    }
    int rankWidth  = vertex.second.size();
    for(const auto & other: vertices) {
      int otherHeader = other.first;
      if (visited.find(otherHeader) != visited.end()) {
        // Already found the rung that contains other.
        continue;
      }
      if (header > otherHeader) {
        pair these(header, otherHeader);
        if (comparisons[these] == Incomparable) {
          rankWidth += other.second.size();
        }
      }
      if (header < otherHeader) {
        pair these(otherHeader, header);
        if (comparisons[these] == Incomparable) {
          rankWidth += other.second.size();
        }
      }
    }
    widths.push_back(rankWidth);
  }
}


