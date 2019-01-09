#ifndef AGENT_H
#define AGENT_H

#include <algorithm>
#include <random>
#include <string>
#include <map>
#include <vector>

class Agent {
  public:

    static std::vector<int> preference_options;
    //Agent(int id, std::vector<std::vector<unsigned int>> preferences);

    Agent(int id, int pref_length, float tie_density, std::mt19937 & generator);

    /**
     * The ID of this agent.
     */
    int id() const {return this->_id; }

    /**
     * Return a human-readable representation of this agent.
     */
    std::string toString();

    /**
     * Return the rank of the given agent according to this agent.
     */
    int rank_of(const Agent & agent) const;

    /**
     * Return the position of the given agent. Note that this is _not_ the rank
     * of the agent. Note that the first agent is at position 1, not 0, since
     * DIMACS.
     * Returns -1 if the agent is not found.
     */
    int position_of(const Agent & agent) const;
    int position_of(int) const;

    /**
     * Return the position (not rank) of the next agent in the preference list
     * with a strictly worse rank.
     */
    signed int position_of_next_worst(const Agent & agent) const;

    /**
     * Returns the preferences in order, such that prefs()[i] is not less
     * preferred than prefs()[i+1]
     */
    const std::vector<int> prefs() const;

    /**
     * Returns a string showing this agents preferences.
     */
    std::string pref_list_string() const;

  private:
    int _id;
    int _max_rank;
    std::vector<std::vector<signed int>> _preferences;
    std::vector<signed int> _preferencesInOrder;
    std::map<int, int> _ranks;
    std::map<int, int> _first_at_this_rank;

};

#endif /* AGENT_H */
