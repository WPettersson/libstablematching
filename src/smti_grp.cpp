#include "smti.h"


SMTI SMTI::create_from_GRP(std::string filename, int threshold) {
  std::ifstream infile(filename);
  std::string line;
  getline(infile, line);
  int nrows = std::stoi(line);

  getline(infile, line);
  int ncols = std::stoi(line);

  std::vector<std::vector<std::pair<int, float>>> row_agents(nrows);
  std::vector<std::vector<std::pair<int, float>>> col_agents(ncols);

  for(int row = 0; row < nrows; ++row) {
    // For each row
    getline(infile, line);
    std::stringstream prefstream(line);
    int col = 0;
    std::string token;
    while (prefstream >> token) {
      if (token.length() == 0) {
        break;
      }
      float score = std::stof(token);
      row_agents[row].push_back(std::make_pair(col, score));
      col_agents[col].push_back(std::make_pair(row, score));
      col += 1;
    }
  }

  // rows are _ones
  std::vector<std::vector<std::vector<int>>> ones;
  for(int row = 0; row < nrows; ++row) {
    std::sort(row_agents[row].begin(), row_agents[row].end(),
              [](const std::pair<int, float> & a, const std::pair<int, float> & b) { return a.second > b.second; });
    std::vector<int> indices;
    std::vector<std::vector<int>> preferences;
    std::vector<int> this_pref;
    float last_score = row_agents[row].front().second;
    for(auto & pair: row_agents[row]) {
      if (pair.second < threshold) {
        break;
      }
      if (last_score != pair.second) {
        preferences.push_back(std::move(this_pref));
        this_pref = std::vector<int>();
      }
      this_pref.push_back(pair.first);
      last_score = pair.second;
    }
    if (! this_pref.empty()) {
        preferences.push_back(std::move(this_pref));
    }
    ones.push_back(preferences);
  }

  // cols are _twos
  std::vector<std::vector<std::vector<int>>> twos;
  for(int col = 0; col < ncols; ++col) {
    std::sort(col_agents[col].begin(), col_agents[col].end(),
              [](const std::pair<int, float> & a, const std::pair<int, float> & b) { return a.second > b.second; });
    std::vector<int> indices;
    std::vector<std::vector<int>> preferences;
    std::vector<int> this_pref;
    float last_score = col_agents[col].front().second;
    for(auto & pair: col_agents[col]) {
      if (pair.second < threshold) {
        break;
      }
      if (last_score != pair.second) {
        preferences.push_back(std::move(this_pref));
        this_pref = std::vector<int>();
      }
      this_pref.push_back(pair.first);
      last_score = pair.second;
    }
    if (! this_pref.empty()) {
        preferences.push_back(std::move(this_pref));
    }
    twos.push_back(preferences);
  }
  return SMTI(ones, twos);
}
