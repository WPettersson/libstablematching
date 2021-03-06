#ifndef AGENT_H
#define AGENT_H

#include <algorithm>
#include <list>
#include <map>
#include <random>
#include <string>
#include <vector>

class Agent {
  public:

    static std::vector<int> preference_options;
    /**
     * Constructor that passes in the actual list of lists of preferences.
     *
     * param id The ID of this agent
     * param preferences a vector of tie-groups of actual preferences
     * param is_fummy true iff this agent is to just be a dummy
     */
    Agent(int id, const std::vector<std::vector<int>> & preferences, bool is_dummy = false);

    /**
     * Constructor using random number generator
     */
    Agent(int id, int pref_length, float tie_density, std::mt19937 & generator);

    /**
     * Constructor using random number generator, but instead of giving the
     * number of preferences to choose, it explicitly gives the exact
     * preferences to use.
     */
    Agent(int id, const std::vector<int> & partners, float tie_density, std::mt19937 & generator);

    /**
     * The ID of this agent.
     */
    int id() const {return this->_id; }

    /**
     * The number of preferences this agent has.
     */
    int num_prefs() const {return this->_preferencesInOrder.size(); }

    /**
     * Return the rank of the given agent according to this agent.
     */
    int rank_of(const Agent & agent) const;
    int rank_of(int id) const;

    /**
     * Adds all IDs between the given values (inclusive) as dummy agents. All
     * dummies are given equal-last preference.
     */
    void add_dummy_pref_up_to(int start, int end);

    /**
     * Removes all IDs after the given ID (not inclusive). These are assumed to
     * be dummy IDs.
     */
    void remove_dummies(int id);

    /**
     * Is the given agent compatible with this agent?
     */
    bool is_compatible(const Agent & agent) const;

    /**
     * Return the position of the given agent. Note that this is _not_ the rank
     * of the agent. Note that the first agent is at position 1, not 0, since
     * DIMACS.
     * return -1 if the agent is not found.
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
     * Returns the preferences as groups, where agents in each group are tied.
     */
    const std::vector<std::vector<signed int>> & preferences() const;

    /**
     * Returns a given tied group of preferences, as a list of Agents.
     */
    const std::vector<signed int> preference_group(int rank) const;

    /**
     * Returns the list of agents which are at least as good as the given
     * agent, according to this agent.
     */
    std::vector<int> as_good_as(int id) const;
    std::vector<int> as_good_as(const Agent & agent) const;

    /**
     * Remove any preferences after the given rank (not including the given
     * rank). Return the list of agent IDs of those agents that were removed.
     */
    std::list<signed int> remove_after(unsigned int rank);

    /**
     * Removes a given option from the preference list.
     */
    void remove_preference(int ident);
    void remove_preference(const Agent & other);

    /**
     * Return a human-readable representation of this agent.
     */
    std::string toString() const { return pref_list_string(); };

    /**
     * Returns a string showing this agents preferences.
     */
    std::string pref_list_string(std::string id_sep = ":", std::string bracket_start = "[", std::string bracket_end = "]") const;

  private:
    int _id;
    unsigned int _max_rank;

    signed int _dummy_rank;
    std::vector<std::vector<signed int>> _preferences;
    std::vector<signed int> _preferencesInOrder;
    std::map<int, int> _ranks;

};

#endif /* AGENT_H */
