#ifndef MATCHING_H
#define MATCHING_H

#include <list>
#include <string>
#include <sstream>

class Matching : public std::list<std::pair<int, int>> {
  public:
    inline bool has(const std::pair<int, int> & match) const;
    inline std::string toString() const;
  private:

};


inline bool Matching::has(const std::pair<int, int> & match) const {
  for(const auto [left, right]: *this) {
    if ((left == match.first) && (right == match.second)) {
      return true;
    }
  }
  return false;
}

inline bool operator==(const Matching & lhs, const Matching & rhs) {
  if (lhs.size() != rhs.size()) {
    return false;
  }
  for(const auto & left: lhs) {
    if (rhs.has(left)) {
      return false;
    }
  }
  return true;
}

inline std::string Matching::toString() const {
  std::stringstream ss;
  bool first = true;
  for(const auto & [left, right]: *this) {
    if (! first) {
      ss << ", ";
    }
    ss << "(" << left << ", " << right << ")";
    first = false;
  }
  return ss.str();
}


#endif /* MATCHING_H */
