#ifndef MATCHING_H
#define MATCHING_H

#include <list>


class Matching : public std::list<std::pair<int, int>> {
  public:
    inline bool has(const std::pair<int, int> & match) const;

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


#endif /* MATCHING_H */
