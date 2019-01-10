#include <algorithm>
#include <sstream>

#include "Agent.h"

std::vector<int> Agent::preference_options = std::vector<int>();

Agent::Agent(int id, int pref_length, float tie_density, std::mt19937 & generator) : _id(id), _dummy_rank(-1) {
  std::sample(Agent::preference_options.begin(), Agent::preference_options.end(),
              std::back_inserter(this->_preferencesInOrder),
               pref_length, generator);
  std::vector<int> tie = std::vector<int>();
  _ranks = std::map<int, int>();
  _first_at_this_rank = std::map<int, int>();
  _max_rank = 1;

  // Our distribution
  std::uniform_real_distribution<float> distribution(0, 1);

  for(int i: this->_preferencesInOrder) {
    if (!tie.empty() && distribution(generator) >= tie_density) {
      _preferences.push_back(std::move(tie));
      tie = std::vector<int>();
      _max_rank += 1;
    }
    _ranks[i] = _max_rank;
    auto found = _first_at_this_rank.find(_max_rank);
    if (found == _first_at_this_rank.end()) {
      _first_at_this_rank[_max_rank] = i;
    }
    tie.push_back(i);
  }
  // Last tie group
  _preferences.push_back(std::move(tie));
}

Agent::Agent(int id, std::vector<std::vector<int>> preferences, bool is_dummy) : _id(id), _dummy_rank(-1) {
  _ranks = std::map<int, int>();
  _first_at_this_rank = std::map<int, int>();
  _max_rank = 0;
  for(std::vector<int> group: preferences) {
    _max_rank += 1;
    std::vector<int> new_group = group;
    _preferences.push_back(new_group);
    _first_at_this_rank[_max_rank] = group.at(0);
    for(auto pref: group) {
      _preferencesInOrder.push_back(pref);
      _ranks[pref] = _max_rank;
    }
  }
  if (is_dummy) {
    _dummy_rank = 0;
  }
}

bool Agent::is_compatible(const Agent & agent) const {
  return _ranks.find(agent.id()) != _ranks.end();
}

int Agent::rank_of(const Agent & agent) const {
  return _ranks.at(agent.id());
}

int Agent::position_of(const Agent & agent) const {
  return this->position_of(agent.id());
}

int Agent::position_of(int id) const {
  int count = 1;
  for(auto i: _preferencesInOrder) {
    if (i == id) {
      return count;
    }
    count+=1;
  }
  return -1;
}

void Agent::add_dummy_pref_up_to(int id) {
  if (_dummy_rank == -1) {
    _preferences.emplace_back(std::vector<int>());
    _dummy_rank = _preferences.size() - 1;
    _max_rank += 1;
  }
  for(int i = _preferencesInOrder.size() + 1; i <= id; ++i) {
    _preferences[_dummy_rank].push_back(i);
    _preferencesInOrder.push_back(i);
  }
}

void Agent::remove_dummies(int id) {
  for(int i = 0; i < id; ++i) {
    _preferencesInOrder.pop_back();
    _preferences[_dummy_rank].pop_back();
  }
  if (_preferences[_dummy_rank].empty()) {
    _preferences.pop_back();
    _dummy_rank = -1;
    _max_rank -= 1;
  }
}

signed int Agent::position_of_next_worst(const Agent & agent) const {
  int next_rank = 1 + this->rank_of(agent);
  if (next_rank > this->_max_rank) {
    return _preferencesInOrder.size() + 1;
  }
  return this->position_of(_first_at_this_rank.at(next_rank));
}

const std::vector<int> Agent::prefs() const {
  return this->_preferencesInOrder;
}

std::string Agent::pref_list_string() const {
  std::stringstream ss;
  ss << this->id() << ":";
  for(auto pref_group: this->_preferences) {
    if (pref_group.size() == 1) {
      ss << " " << pref_group.at(0);
    } else {
      ss << " " << "[";
      bool first = true;
      for (auto pref: pref_group) {
        if (!first) {
          ss << " ";
        }
        first = false;
        ss << pref;
      }
      ss << "]";
    }
  }
  return ss.str();
}
