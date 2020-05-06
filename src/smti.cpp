#include <fstream>
#include <sstream>
#include <set>

#include "smti.h"

SMTI::SMTI(int size, int pref_length, float tie_density, std::mt19937 & generator) :
  _size(size), _num_dummies(0) {
  // Construct the list of preferences. This list is constantly shuffled to
  // create preferences for each agent.
  Agent::preference_options.clear();
  for(int i = 1; i <= size; ++i) {
    Agent::preference_options.push_back(i);
  }

  std::vector<std::vector<int>> two_prefs(size+1);
  for(int i = 1; i <= size; ++i) {
    _ones.emplace(i, Agent(i, pref_length, tie_density, generator));
    for(auto pref: _ones.at(i).prefs()) {
      two_prefs[pref].push_back(i);
    }
  }
  for(int i = 1; i <= size; ++i) {
    std::shuffle(two_prefs[i].begin(), two_prefs[i].end(), generator);
    _twos.emplace(i, Agent(i, two_prefs[i], tie_density, generator));
  }
}


SMTI::SMTI(std::string filename) : _num_dummies(0) {
  std::ifstream infile(filename);
  std::string line;
  getline(infile, line);
  _size = std::stoi(line);
  bool expect_capacity = false;
  if (_size == 0) {
    // Probably reading a file in format for 1810.02711
    getline(infile, line);
    _size = std::stoi(line);
    expect_capacity = true;
  }
  getline(infile, line);
  int second_size = std::stoi(line);

  for(int lineno = 0; lineno < _size; ++lineno) {
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
      if ((token.front() == '[') || (token.front() == '('))  {
        token.erase(0, 1); // Remove [
        in_tie = true;
      }
      if ((token.back() == ']')  || (token.back() == ')')) {
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
    _ones.emplace(id, Agent(id, preferences));
  }
  for(int lineno = 0; lineno < second_size; ++lineno) {
    int id;
    getline(infile, line);
    std::stringstream prefstream(line);
    prefstream >> id;
    std::string token;
    std::vector<std::vector<int>> preferences;
    std::vector<int> tie;
    bool in_tie = false;
    bool need_capacity = expect_capacity;
    while (prefstream >> token) {
      if (token == ":") {
        continue;
      }
      if (need_capacity) {
        // We just pulled in the capacity, which is hopefully 1, but we ignore
        // it.
        need_capacity = false;
        continue;
      }
      if ((token.front() == '[') || (token.front() == '(')) {
        token.erase(0, 1); // Remove [
        in_tie = true;
      }
      if ((token.back() == ']') || (token.back() == ')')) {
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
    _twos.emplace(id, Agent(id, preferences));
  }
}

SMTI::SMTI(const SMTI & old) {
  _size = old._size;
  for(auto [id, left]: old.agents_left()) {
    _ones.emplace(id, Agent(id, left.preferences()));
  }
  for(auto [id, right]: old.agents_right()) {
    _twos.emplace(id, Agent(id, right.preferences()));
  }
}

SMTI::SMTI(const std::vector<std::vector<std::vector<int>>> & ones, const std::vector<std::vector<std::vector<int>>> & twos) :
  _ones(), _twos() {
  _size = ones.size();
  for (unsigned int id = 0; id < ones.size(); ++id) {
    _ones.emplace(id, Agent(id, ones.at(id)));
  }
  for (unsigned int id = 0; id < twos.size(); ++id) {
    _twos.emplace(id, Agent(id, twos[id]));
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
    _ones.emplace(_size + i, Agent(_size + i, prefs, true));
    _twos.emplace(_size + i, Agent(_size + i, prefs, true));
  }

  // Add the dummies as compatible to all existing agents.
  for(auto & [key, one]: _ones) {
    one.add_dummy_pref_up_to(_size + 1, _size + num_dummy);
  }
  for(auto & [key, two]: _twos) {
    two.add_dummy_pref_up_to(_size + 1, _size + num_dummy);
  }
  _size += num_dummy;
  _num_dummies += num_dummy;
}

void SMTI::remove_dummy(int num_dummy) {
  // Remove the dummies.
  for(int i = 0; i < num_dummy; ++i) {
    _ones.erase(_size - i);
    _twos.erase(_size - i);
  }
  // Remove them as preference options.
  for(auto & [key, one]: _ones) {
    one.remove_dummies(num_dummy);
  }
  for(auto & [key, two]: _twos) {
    two.remove_dummies(num_dummy);
  }
  _size -= num_dummy;
  _num_dummies -= num_dummy;
}

void SMTI::remove_pair(int left, int right) {
  _ones.at(left).remove_preference(right);
  _twos.at(right).remove_preference(left);
}



std::string SMTI::to_string(std::string id_sep, std::string bracket_start, std::string bracket_end) const {
  std::stringstream ss;
  ss << _ones.size() << std::endl;
  ss << _twos.size() << std::endl;
  for(auto & [key, one]: _ones) {
    ss << one.pref_list_string(id_sep, bracket_start, bracket_end) << std::endl;
  }
  for(auto & [key, two]: _twos) {
    ss << two.pref_list_string(id_sep, bracket_start, bracket_end) << std::endl;
  }
  return ss.str();
}
