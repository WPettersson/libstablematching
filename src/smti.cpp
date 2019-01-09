#include <iostream>
#include <sstream>

#include "smti.h"

SMTI::SMTI(int size, int pref_length, float tie_density, std::mt19937 & generator) {
  // Construct the list of preferences. This list is constantly shuffled to
  // create preferences for each agent.
  Agent::preference_options.clear();
  for(int i = 1; i <= size; ++i) {
    Agent::preference_options.push_back(i);
  }

  for(int i = 1; i <= size; ++i) {
    _ones.emplace_back(Agent(i, pref_length, tie_density, generator));
    _twos.emplace_back(Agent(i, pref_length, tie_density, generator));
  }
  make_var_map();
}

std::string SMTI::encodeSAT() {
  std::stringstream ss;
  int num_clauses = 0;
  // Clause 1 TODO Make soft with weight 1
  for (auto one: _ones) {
    ss << _one_vars[std::make_tuple(one.id(), 1)] << " 0" << std::endl;
    num_clauses++;
  }
  // Clause 2 TODO Make soft with weight 1
  for (auto two: _twos) {
    ss << _two_vars[std::make_tuple(two.id(), 1)] << " 0" << std::endl;
    num_clauses++;
  }
  // Clause 3
  for (auto one: _ones) {
    for (int i = 1; i <= one.prefs().size(); ++i) {
      ss << _one_vars[std::make_tuple(one.id(), i)] << " -" <<
           (_one_vars[std::make_tuple(one.id(), i)]+1) << " 0" << std::endl;
      num_clauses++;
    }
  }
  // Clause 4
  for (auto two: _twos) {
    for (int i = 1; i <= two.prefs().size(); ++i) {
      ss << _two_vars[std::make_tuple(two.id(), i)] << " -" <<
           (_two_vars[std::make_tuple(two.id(), i)]+1) << " 0" << std::endl;
      num_clauses++;
    }
  }
  for (auto one: _ones) {
    for (auto two: _twos) {
      int p = one.position_of(two);
      int q = two.position_of(one);
      // Clause 5
      ss << "-" << _one_vars[std::make_tuple(one.id(), p)] << " " <<
         (_one_vars[std::make_tuple(one.id(), p)]+1) << " " <<
         _two_vars[std::make_tuple(two.id(), q)] << " 0" << std::endl;
      num_clauses++;
      ss << "-" << _one_vars[std::make_tuple(one.id(), p)] << " " <<
         (_one_vars[std::make_tuple(one.id(), p)]+1) << " -" <<
         (_two_vars[std::make_tuple(two.id(), q)]+1) << " 0" << std::endl;
      num_clauses++;
      // Clause 6
      ss << "-" << _two_vars[std::make_tuple(two.id(), q)] << " " <<
         (_two_vars[std::make_tuple(two.id(), q)]+1) << " " <<
         _one_vars[std::make_tuple(one.id(), p)] << " 0" << std::endl;
      num_clauses++;
      ss << "-" << _two_vars[std::make_tuple(two.id(), q)] << " " <<
         (_two_vars[std::make_tuple(two.id(), q)]+1) << " -" <<
         (_one_vars[std::make_tuple(one.id(), p)]+1) << " 0" << std::endl;
      num_clauses++;
      int pplus = one.position_of_next_worst(two);
      //std::cout << "Looking at " << one.id() << " and " << two.id() << std::endl;
      if (pplus < 0) {
        //std::cout << " and next worse for one after " << two.id() << " is at end" << std::endl;
        continue;
      }
      int qplus = two.position_of_next_worst(one);
      if (qplus < 0) {
        //std::cout << " and next worse for two after " << one.id() << " is at end" << std::endl;
        continue;
      }
      // Clause 7
      ss << "-" << _one_vars[std::make_tuple(one.id(), pplus)] << " -" <<
        _two_vars[std::make_tuple(two.id(), qplus)] << " 0" << std::endl;
      num_clauses++;
      // Clause 8
      ss << "-" << _two_vars[std::make_tuple(two.id(), qplus)] << " -" <<
        _one_vars[std::make_tuple(one.id(), pplus)] << " 0" << std::endl;
      num_clauses++;
    }
  }
  std::stringstream start;
  start << "p cnf " << (_one_vars.size() + _two_vars.size()) << " " << num_clauses;
  start << std::endl << ss.str();
  return start.str();
}

void SMTI::make_var_map() {
  _one_vars = std::unordered_map<std::tuple<int,int>, int>();
  _two_vars = std::unordered_map<std::tuple<int,int>, int>();
  int counter = 1; // DIMACS format starts at 1.
  for(auto one: _ones) {
    int pref_length = 1;
    for(auto pref: one.prefs()) {
      _one_vars[std::make_tuple(one.id(), pref_length)] = counter;
      counter++;
      pref_length++;
    }
    _one_vars[std::make_tuple(one.id(), pref_length)] = counter;
    counter++;
  }
  for(auto two: _twos) {
    int pref_length = 1;
    for(auto pref: two.prefs()) {
      _two_vars[std::make_tuple(two.id(), pref_length)] = counter;
      counter++;
      pref_length++;
    }
    _two_vars[std::make_tuple(two.id(), pref_length)] = counter;
    counter++;
  }
}

std::string SMTI::to_string() const {
  std::stringstream ss;
  ss << _ones.size() << std::endl;
  ss << _ones.size() << std::endl;
  for(auto one: _ones) {
    ss << one.pref_list_string() << std::endl;
  }
  for(auto two: _twos) {
    ss << two.pref_list_string() << std::endl;
  }
  return ss.str();
}
