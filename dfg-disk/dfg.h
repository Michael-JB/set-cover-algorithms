#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <fstream>
#include <iostream>

class DFG {
  private:
    std::unordered_map<int, std::ofstream*> outfiles; // <k, file stream>
    std::unordered_set<uint64_t> coveredElements;
    double p = 1.05;
    int maxK = 0;
    long long bytesWritten = 0, bytesRead = 0;

    // Computes the k value associated with the inequality p^k <= n < p^(k+1)
    int getSubCollForN(int n);

    // Gets the file name associated with the subcollection k
    std::string getSubcollectionFileName(int k);

    // Returns a \ b
    std::vector<uint64_t> setDifference(const std::vector<uint64_t>& a, const std::unordered_set<uint64_t>& b);

    // Adds the set to the appropriate S_(k), updating maxK if necessary
    void addToSubCollection(const std::vector<uint64_t>& set, const uint64_t index);


  public:
    DFG(double p) {
      if (p > 1) {
        this->p = p;
      }
    }

    // Processes a set from the input stream
    void offerSet(const std::vector<uint64_t>& set, const uint64_t index);

    // Extracts a 1-cover from the subcoll and sets structures
    std::vector<uint64_t> extractCover();

    long long getBytesWritten();

    long long getBytesRead();

};