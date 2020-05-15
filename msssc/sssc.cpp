#include <math.h>
#include "sssc.h"

// Computes the level associated with an input set size
int SSSC::computeLevel(const int setSize) {
  return ceil(log2(setSize)/log2(effectivityBase));
}

// Finds an effective subset of maximal size
std::vector<int> SSSC::computeEffectiveSubsetSort(std::vector<std::pair<int,SSSC::VertexData>>& vs) {
  // Sort vertices by descending eff(v)
  std::sort(vs.begin(), vs.end(), [](const std::pair<int,SSSC::VertexData>& i, const std::pair<int,SSSC::VertexData>& j) {
    return i.second.eff > j.second.eff;
  });
  for (int i = 0; i < vs.size(); ++i) {
    const std::pair<int,SSSC::VertexData> v = vs.at(i);
    if (computeLevel(vs.size() - i) > v.second.eff) {
      std::vector<std::pair<int,SSSC::VertexData>> effSubset = std::vector<std::pair<int,SSSC::VertexData>>(vs.begin() + i, vs.end());
      std::vector<int> result;

      std::transform(effSubset.begin(), effSubset.end(), std::back_inserter(result), [](const std::pair<int,SSSC::VertexData>& v) {
        return v.first;
      });
      return result;
    }
  }
  return {};
}

// Finds an effective subset of maximal size
std::vector<int> SSSC::computeEffectiveSubsetCount(std::vector<std::pair<int,SSSC::VertexData>>& vs) {
  // Calculate the upper bound for the effectivity in this set
  int capacity = computeLevel(vs.size());

  // Populate effectivity counter table (+1 added to account for eff=-1)
  std::vector<int> effCounter(capacity + 1, 0);
  for (const std::pair<int,SSSC::VertexData>& v : vs) {
    if (v.second.eff < capacity) {
      effCounter[v.second.eff + 1] += 1;
    }
  }

  // Iterate through effectivity counter, finding the critical effectivity
  int subsetSize = 0, criticalEff = -1;
  for (int i = 0; i <= capacity; ++i) {
    int eff = i - 1;
    subsetSize += effCounter[i];
    if (subsetSize > 0 && computeLevel(subsetSize) > eff) {
      criticalEff = eff;
    }
  }

  // Add all elements below this critical value to the solution set
  std::vector<int> result;
  for (const std::pair<int,SSSC::VertexData>& v : vs) {
    if (v.second.eff <= criticalEff) {
      result.push_back(v.first);
    }
  }

  return result;
}

// Retrieves the SSSC::VertexData (if any) associated with each element in the input set
std::vector<std::pair<int,SSSC::VertexData>> SSSC::fetchVertexData(const std::vector<int>& set) {
  std::vector<std::pair<int,SSSC::VertexData>> vs;

  for (int i = 0; i < set.size(); ++i) {
    const int element = set.at(i);
    if (restrictedUniverse.empty() || restrictedUniverse.find(element) != restrictedUniverse.end()) {
      std::unordered_map<int,SSSC::VertexData>::const_iterator v = vertices.find(element);
      if (v == vertices.end()) {
        const std::pair<int,SSSC::VertexData> newVertex = {element, {}};
        vertices.insert(newVertex);
        vs.push_back(newVertex);
      } else {
        vs.push_back(*v);
      }
    }
  }

  return vs;
}

// Processes a set from the input stream
void SSSC::offerSet(const std::vector<int>& set, const int index) {
  std::vector<std::pair<int,SSSC::VertexData>> vs = fetchVertexData(set);
  if (!vs.empty()) {
    const std::vector<int> subset = computeEffectiveSubsetCount(vs);

    if (!subset.empty()) {
      const int level = computeLevel(subset.size());
      for (int i = 0; i < subset.size(); ++i) {
        std::unordered_map<int,SSSC::VertexData>::iterator v = vertices.find(subset.at(i));
        v->second.eid = index;
        v->second.eff = level;
      }
    }
  }
}

// Extracts the 1-cover set from the element data table
std::vector<int> SSSC::extractCover() {
  std::unordered_set<int> cover = extractCoverSet();
  std::vector<int> result(cover.begin(), cover.end());
  return result;
}

// Extracts the 1-cover set from the element data table
std::unordered_set<int> SSSC::extractCoverSet() {
  std::unordered_set<int> cover;
  for (const std::pair<const int,SSSC::VertexData>& v : vertices) {
    cover.insert(v.second.eid);
  }
  return cover;
}

// Extracts the 1-cover set from the element data table
std::unordered_map<int,SSSC::VertexData> SSSC::getCoverCertificate() {
  return vertices;
}

std::unordered_map<int,int> SSSC::extractCountCover() {
  std::unordered_map<int,int> setCounts;
  for (const std::pair<const int,SSSC::VertexData>& v : vertices) {
    std::unordered_map<int,int>::iterator existingSet = setCounts.find(v.second.eid);
    if (existingSet == setCounts.end()) {
      const std::pair<int,int> newSetCount = {v.second.eid, 1};
      setCounts.insert(newSetCount);
    } else {
      existingSet->second++;
    }
  }

  return setCounts;
}

void SSSC::restrictUniverse(const std::unordered_set<int>& set) {
  restrictedUniverse = set;
}