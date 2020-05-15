#include <vector>
#include <unordered_map>
#include <unordered_set>

class DFG {
  private:
    std::unordered_map<int,std::vector<int>> sets; // Map used instead of vector of vectors for improved extraction time
    std::unordered_map<int,std::vector<int>> subcoll; // <k,indices>
    std::unordered_set<int> coveredElements;
    double p = 1.05;
    int maxK = 0;

    std::unordered_map<int,int> labelledCover; // <k,indices>

    // Computes the k value associated with the inequality p^k <= n < p^(k+1)
    int getSubCollForN(int n);

    // Returns a \ b
    std::vector<int> setDifference(const std::vector<int>& a, const std::unordered_set<int>& b);

    // Adds the set to the appropriate S_(k), updating maxK if necessary
    void addToSubCollection(const std::vector<int>& set, const int index);

  public:
    DFG(double p) {
      if (p > 1) {
        this->p = p;
      }
    }

    // Processes a set from the input stream
    void offerSet(const std::vector<int>& set, const int index);

    // Extracts a 1-cover from the subcoll and sets structures
    std::vector<int> extractCover();
};