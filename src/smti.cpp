#include <fstream>
#include <sstream>

#include "smti.h"

SMTI::SMTI(int size, int pref_length, float tie_density, std::mt19937 & generator) :
  _size(size), _num_dummies(0) {
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
}

SMTI::SMTI(std::string filename) : _num_dummies(0) {
  std::ifstream infile(filename);
  std::string line;
  getline(infile, line);
  _size = std::stoi(line);
  getline(infile, line);
  int second_size = std::stoi(line);

  for(int i = 0; i < _size; ++i) {
    int id;
    getline(infile, line);
    std::stringstream prefstream(line);
    prefstream >> id;
    std::string token;
    std::vector<std::vector<int>> preferences;
    std::vector<int> tie;
    bool in_tie = false;
    while (prefstream >> token) {
      if (token == ":") {
        continue;
      }
      if (token.front() == '[') {
        token.erase(0, 1); // Remove [
        in_tie = true;
      }
      if (token.back() == ']') {
        token.pop_back();
        int i = std::stoi(token);
        tie.push_back(i);
        preferences.push_back(std::move(tie));
        tie = std::vector<int>();
        in_tie = false;
      } else if (in_tie) {
        int i = std::stoi(token);
        tie.push_back(i);
      } else { // Not in tie
        int i = std::stoi(token);
        std::vector<int> here;
        here.push_back(i);
        preferences.push_back(std::move(here));
      }
    }
    _ones.emplace_back(id, preferences);
  }
  for(int i = 0; i < _size; ++i) {
    int id;
    getline(infile, line);
    std::stringstream prefstream(line);
    prefstream >> id;
    std::string colon;
    prefstream >> colon;
    std::string token;
    std::vector<std::vector<int>> preferences;
    std::vector<int> tie;
    bool in_tie = false;
    while (prefstream >> token) {
      if (token.front() == '[') {
        token.erase(0, 1); // Remove [
        in_tie = true;
      }
      if (token.back() == ']') {
        token.pop_back();
        int i = std::stoi(token);
        tie.push_back(i);
        preferences.push_back(std::move(tie));
        tie = std::vector<int>();
        in_tie = false;
      } else if (in_tie) {
        int i = std::stoi(token);
        tie.push_back(i);
      } else { // Not in tie
        int i = std::stoi(token);
        std::vector<int> here;
        here.push_back(i);
        preferences.push_back(std::move(here));
      }
    }
    _twos.emplace_back(id, preferences);
  }
}


void SMTI::add_dummy(int num_dummy) {
  // Create the preferences of each dummy.
  std::vector<int> dummy_prefs;
  for(int i = 1; i <= _size; ++i) {
    dummy_prefs.push_back(i);
  }
  std::vector<std::vector<int>> prefs;
  prefs.push_back(dummy_prefs);

  // Create and add the actual dummy agents
  for(int i = 1; i <= num_dummy; ++i) {
    _ones.emplace_back(Agent(_size + i, prefs, true));
    _twos.emplace_back(Agent(_size + i, prefs, true));
  }

  // Add the dummies as compatible to all existing agents.
  for(auto & one: _ones) {
    one.add_dummy_pref_up_to(_size + 1, _size + num_dummy);
  }
  for(auto & two: _twos) {
    two.add_dummy_pref_up_to(_size + 1, _size + num_dummy);
  }
  _size += num_dummy;
  _num_dummies += num_dummy;
}

void SMTI::remove_dummy(int num_dummy) {
  // Remove the dummies.
  for(int i = 0; i < num_dummy; ++i) {
    _ones.pop_back();
    _twos.pop_back();
  }
  // Remove them as preference options.
  for(auto & one: _ones) {
    one.remove_dummies(num_dummy);
  }
  for(auto & two: _twos) {
    two.remove_dummies(num_dummy);
  }
  _size -= num_dummy;
  _num_dummies -= num_dummy;
}

std::string SMTI::encodeSAT() {
  make_var_map();
  std::stringstream ss;
  int num_clauses = 0;
  // Clause 1
  for (auto one: _ones) {
    ss << _one_vars[std::make_tuple(one.id(), 1)] << " 0" << std::endl;
    num_clauses++;
    ss << "-" << _one_vars[std::make_tuple(one.id(), one.num_prefs() + 1)] <<
          " 0" << std::endl;
    num_clauses++;
  }
  // Clause 2
  for (auto two: _twos) {
    ss << _two_vars[std::make_tuple(two.id(), 1)] << " 0" << std::endl;
    num_clauses++;
    ss << "-" << _two_vars[std::make_tuple(two.id(), two.num_prefs() + 1)] <<
          " 0" << std::endl;
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
      if (p == -1) {
        continue;
      }
      int q = two.position_of(one);
      if (q == -1) {
        continue;
      }
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
      if (pplus < 0) {
        continue;
      }
      int qplus = two.position_of_next_worst(one);
      if (qplus < 0) {
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

std::string SMTI::encodeWPMaxSAT() {
  make_var_map();
  std::stringstream ss;
  int num_clauses = 0;
  int top_weight = 500;
  // Clause 1
  for (auto one: _ones) {
    ss << top_weight << " ";
    ss << _one_vars[std::make_tuple(one.id(), 1)] << " 0" << std::endl;
    num_clauses++;
    ss << "1 " << " ";
    ss << "-" << _one_vars[std::make_tuple(one.id(), one.num_prefs() + 1)] <<
          " 0" << std::endl;
    num_clauses++;
  }
  // Clause 2
  for (auto two: _twos) {
    ss << top_weight << " ";
    ss << _two_vars[std::make_tuple(two.id(), 1)] << " 0" << std::endl;
    num_clauses++;
    ss << "1 " << " ";
    ss << "-" << _two_vars[std::make_tuple(two.id(), two.num_prefs() + 1)] <<
          " 0" << std::endl;
    num_clauses++;
  }
  // Clause 3
  for (auto one: _ones) {
    for (int i = 1; i <= one.prefs().size(); ++i) {
      ss << top_weight << " ";
      ss << _one_vars[std::make_tuple(one.id(), i)] << " -" <<
           (_one_vars[std::make_tuple(one.id(), i)]+1) << " 0" << std::endl;
      num_clauses++;
    }
  }
  // Clause 4
  for (auto two: _twos) {
    for (int i = 1; i <= two.prefs().size(); ++i) {
      ss << top_weight << " ";
      ss << _two_vars[std::make_tuple(two.id(), i)] << " -" <<
           (_two_vars[std::make_tuple(two.id(), i)]+1) << " 0" << std::endl;
      num_clauses++;
    }
  }
  for (auto one: _ones) {
    for (auto two: _twos) {
      int p = one.position_of(two);
      if (p == -1) {
        continue;
      }
      int q = two.position_of(one);
      if (q == -1) {
        continue;
      }
      // Clause 5
      ss << top_weight << " ";
      ss << "-" << _one_vars[std::make_tuple(one.id(), p)] << " " <<
         (_one_vars[std::make_tuple(one.id(), p)]+1) << " " <<
         _two_vars[std::make_tuple(two.id(), q)] << " 0" << std::endl;
      num_clauses++;
      ss << top_weight << " ";
      ss << "-" << _one_vars[std::make_tuple(one.id(), p)] << " " <<
         (_one_vars[std::make_tuple(one.id(), p)]+1) << " -" <<
         (_two_vars[std::make_tuple(two.id(), q)]+1) << " 0" << std::endl;
      num_clauses++;
      // Clause 6
      ss << top_weight << " ";
      ss << "-" << _two_vars[std::make_tuple(two.id(), q)] << " " <<
         (_two_vars[std::make_tuple(two.id(), q)]+1) << " " <<
         _one_vars[std::make_tuple(one.id(), p)] << " 0" << std::endl;
      num_clauses++;
      ss << top_weight << " ";
      ss << "-" << _two_vars[std::make_tuple(two.id(), q)] << " " <<
         (_two_vars[std::make_tuple(two.id(), q)]+1) << " -" <<
         (_one_vars[std::make_tuple(one.id(), p)]+1) << " 0" << std::endl;
      num_clauses++;
      int pplus = one.position_of_next_worst(two);
      if (pplus < 0) {
        continue;
      }
      int qplus = two.position_of_next_worst(one);
      if (qplus < 0) {
        continue;
      }
      // Clause 7
      ss << top_weight << " ";
      ss << "-" << _one_vars[std::make_tuple(one.id(), pplus)] << " -" <<
        _two_vars[std::make_tuple(two.id(), qplus)] << " 0" << std::endl;
      num_clauses++;
      // Clause 8
      ss << top_weight << " ";
      ss << "-" << _two_vars[std::make_tuple(two.id(), qplus)] << " -" <<
        _one_vars[std::make_tuple(one.id(), pplus)] << " 0" << std::endl;
      num_clauses++;
    }
  }
  std::stringstream start;
  start << "p wcnf " << (_one_vars.size() + _two_vars.size()) << " " << num_clauses;
  start << " " << top_weight;
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
