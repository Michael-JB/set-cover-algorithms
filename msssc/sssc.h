#include <vector>
#include <unordered_map>
#include <unordered_set>

class SSSC {
  public:
    SSSC(double effectivityBase) {
      if (effectivityBase > 1) {
        this->effectivityBase = effectivityBase;
      }
    }

    // Describes the current cover status of a set element
    struct VertexData {
      int eid = -1;
      int eff = -1;
    };

    // Processes a set from the input stream
    void offerSet(const std::vector<int>& set, const int index);

    // Extracts the 1-cover set from the element data table
    std::vector<int> extractCover();

    std::unordered_set<int> extractCoverSet();

    // Extracts the 1-cover set from the element data table
    std::unordered_map<int,int> extractCountCover();

    void restrictUniverse(const std::unordered_set<int>& set);

    std::unordered_map<int,VertexData> getCoverCertificate();

  private:
    // Stores the data for each vertex in a hashtable with <element,data> pairs
    std::unordered_map<int,VertexData> vertices;

    std::unordered_set<int> restrictedUniverse; // For multi-pass.

    // The logarithmic base for the calculation of effectivity
    double effectivityBase = 2;

    // Computes the level associated with an input set size
    int computeLevel(const int setSize);

    // Finds an effective subset of maximal size
    std::vector<int> computeEffectiveSubsetSort(std::vector<std::pair<int,VertexData>>& vs);

    // Finds an effective subset of maximal size
    std::vector<int> computeEffectiveSubsetCount(std::vector<std::pair<int,VertexData>>& vs);

    // Retrieves the VertexData (if any) associated with each element in the input set
    std::vector<std::pair<int,VertexData>> fetchVertexData(const std::vector<int>& set);
};