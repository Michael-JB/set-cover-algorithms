#include <math.h>
#include "dfg.h"

// Computes the k value associated with the inequality p^k <= n < p^(k+1)
int DFG::getSubCollForN(int n) {
  return floor(log(n) / log(p));
}

// Returns a \ b
std::vector<int> DFG::setDifference(const std::vector<int>& a, const std::unordered_set<int>& b) {
  std::vector<int> result;
  for (const int element : a) {
    std::unordered_set<int>::const_iterator elementInB = b.find(element);
    if (elementInB == b.end()) {
      result.push_back(element);
    }
  }
  return result;
}

// Adds the set to the appropriate S_(k), updating maxK if necessary
void DFG::addToSubCollection(const std::vector<int>& set, const int index) {
  if (set.size() > 0) {
    const int k = getSubCollForN(set.size());
    std::unordered_map<int,std::vector<int>>::iterator v = subcoll.find(k);
    if (v == subcoll.end()) {
      subcoll.insert({k, {index}});
    } else {
      v->second.push_back(index);
    }
    if (k > maxK) {
      maxK = k;
    }
  }
}

// Processes a set from the input stream
void DFG::offerSet(const std::vector<int>& set, const int index) {
  sets.insert({index, set});
  addToSubCollection(set, index);
}

// Extracts a 1-cover from the subcoll and sets structures
std::vector<int> DFG::extractCover() {
  std::vector<int> cover;

  for (int k = maxK; k >= 0; --k) {
    std::unordered_map<int,std::vector<int>>::iterator sk = subcoll.find(k);
    if (sk != subcoll.end()) {
      for (int i = 0; i < sk->second.size(); ++i) {
        const int setIndex = sk->second.at(i);
        std::unordered_map<int,std::vector<int>>::iterator set = sets.find(setIndex);
        const std::vector<int> diff = setDifference(set->second, coveredElements);
        if ((k == 0 && diff.size() == 1) || diff.size() >= pow(p, k)) {
          cover.push_back(setIndex);
          coveredElements.insert(diff.begin(), diff.end());
        } else {
          if (k != 0) {
            set->second = diff;
            addToSubCollection(diff, setIndex);
          }
        }
      }
    }
  }

  return cover;
}