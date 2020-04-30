#include <sstream>

#include "Agent.h"

std::vector<int> Agent::preference_options = std::vector<int>();

Agent::Agent(int id, int pref_length, float tie_density,
    std::mt19937 & generator) : _id(id), _dummy_rank(-1) {
  std::sample(Agent::preference_options.begin(), Agent::preference_options.end(),
              std::back_inserter(this->_preferencesInOrder),
              pref_length, generator);
  std::shuffle(_preferencesInOrder.begin(), _preferencesInOrder.end(), generator);
  std::vector<int> tie = std::vector<int>();
  _ranks = std::map<int, int>();
  _max_rank = 0;

  // Our distribution
  std::uniform_real_distribution<float> distribution(0, 1);

  for(int i: this->_preferencesInOrder) {
    if (!tie.empty() && distribution(generator) >= tie_density) {
      _preferences.push_back(std::move(tie));
      tie = std::vector<int>();
      _max_rank += 1;
    }
    _ranks[i] = _max_rank;
    tie.push_back(i);
  }
  // Last tie group
  _preferences.push_back(std::move(tie));
}

Agent::Agent(int id, const std::vector<int> & partners, float tie_density, std::mt19937 & generator) :
  _id(id), _dummy_rank(-1) {
  _ranks = std::map<int, int>();
  _max_rank = 0;

  if (partners.empty()) {
    return;
  }

  std::sample(partners.begin(), partners.end(),
              std::back_inserter(this->_preferencesInOrder),
              partners.size(), generator);
  std::vector<int> tie = std::vector<int>();
  // Our distribution
  std::uniform_real_distribution<float> distribution(0, 1);

  for(int i: this->_preferencesInOrder) {
    if (!tie.empty() && distribution(generator) >= tie_density) {
      _preferences.push_back(std::move(tie));
      tie = std::vector<int>();
      _max_rank += 1;
    }
    _ranks[i] = _max_rank;
    tie.push_back(i);
  }
  // Last tie group
  _preferences.push_back(std::move(tie));
}

Agent::Agent(int id, const std::vector<std::vector<int>> & preferences, bool is_dummy) : _id(id), _dummy_rank(-1) {
  _ranks = std::map<int, int>();
  _max_rank = 0;
  for(std::vector<int> group: preferences) {
    std::vector<int> new_group(group);
    _preferences.push_back(new_group);
    for(auto pref: group) {
      _preferencesInOrder.push_back(pref);
      _ranks[pref] = _max_rank;
    }
    _max_rank += 1;
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

int Agent::rank_of(int id) const {
  return _ranks.at(id);
}

std::vector<int> Agent::as_good_as(const Agent & agent) const {
  return as_good_as(agent.id());
}

std::vector<int> Agent::as_good_as(int id) const {
  std::vector<int> res;
  for (const auto & group: _preferences) {
    bool found_here = false;
    for (auto pref: group) {
      res.push_back(pref);
      if (pref == id) {
        found_here = true;
      }
    }
    if (found_here) {
      return res;
    }
  }
  return res;
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

void Agent::add_dummy_pref_up_to(int start, int end) {
  if (_dummy_rank == -1) {
    _preferences.emplace_back(std::vector<int>());
    _dummy_rank = _preferences.size() - 1;
    _max_rank += 1;
  }
  for(int i = start; i <= end; ++i) {
    _preferences[_dummy_rank].push_back(i);
    _preferencesInOrder.push_back(i);
    _ranks[i] = _max_rank;
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
  unsigned int next_rank = 1 + this->rank_of(agent);
  if (next_rank > this->_max_rank) {
    return _preferencesInOrder.size() + 1;
  }
  while (next_rank < _preferences.size()) {
    if (_preferences[next_rank].size() == 0)
      continue;
    return this->position_of(_preferences[next_rank][0]);
  }
  return _preferencesInOrder.size() + 1;
}

const std::vector<int> Agent::prefs() const {
  return this->_preferencesInOrder;
}

const std::vector<std::vector<signed int>> & Agent::preferences() const {
  return this->_preferences;
}

const std::vector<signed int> Agent::preference_group(int rank) const {
  return this->_preferences[rank];
}

std::list<signed int> Agent::remove_after(unsigned int rank) {
  std::list<signed int> removed;
  // if rank >= (max_rank - 1), do nothing
  if ((rank + 1) >= _max_rank) {
    return removed;
  }
  // Remove in _preferencesInOrder, if there is something to remove
  unsigned int next_rank = rank + 1;
  // Find next to remove
  while (next_rank < this->_preferences.size()) {
    // If the next rank is empty, look further
    if (this->_preferences[next_rank].size() == 0) {
      next_rank += 1;
      continue;
    }
    auto pos = std::find(_preferencesInOrder.begin(), _preferencesInOrder.end(),
        this->_preferences[next_rank][0]);
    if (pos != _preferencesInOrder.end()) {
      _preferencesInOrder.erase(pos, _preferencesInOrder.end());
    }
    break;
  }
  // Clear out _ranks
  for(unsigned int now = rank + 1; now < this->_preferences.size(); ++now) {
    for(auto ident: _preferences[now]) {
      _ranks.erase(ident);
      removed.push_back(ident);
    }
  }
  this->_preferences.erase(this->_preferences.begin() + rank + 1,
                           this->_preferences.end());
  this->_max_rank = rank;
  return removed;
}

void Agent::remove_preference(int ident) {
  for(auto & tie_group: _preferences) {
    auto pos = std::find(tie_group.begin(), tie_group.end(), ident);
    if (pos != tie_group.end()) {
      tie_group.erase(pos);
    }
  }
  auto pos = std::find(_preferencesInOrder.begin(), _preferencesInOrder.end(), ident);
  if (pos != _preferencesInOrder.end()) {
    _preferencesInOrder.erase(pos);
  }
  _ranks.erase(ident);
}

void Agent::remove_preference(const Agent & other) {
  remove_preference(other.id());
}

std::string Agent::pref_list_string(std::string id_sep, std::string bracket_start, std::string bracket_end) const {
  std::stringstream ss;
  ss << (this->id() + 1) << id_sep;
  for(auto pref_group: this->_preferences) {
    if (pref_group.size() == 0) {
      continue;
    }
    if (pref_group.size() == 1) {
      ss << " " << (pref_group.at(0) + 1);
    } else {
      ss << " " << bracket_start;
      bool first = true;
      for (auto pref: pref_group) {
        if (!first) {
          ss << " ";
        }
        first = false;
        ss << (pref + 1);
      }
      ss << bracket_end;
    }
  }
  return ss.str();
}
