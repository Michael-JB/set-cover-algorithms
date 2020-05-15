#include <vector>
#include <unordered_map>
#include <unordered_set>

class SSSC {
  public:
    // Processes a set from the input stream
    void offerSet(const std::vector<int>& set, const int index);

    // Extracts a 1-cover vector from the element data table
    std::vector<int> extractCover();

    // Extracts a 1-cover set from the element data table
    std::unordered_set<int> extractCoverSet();

  private:
    // Describes the current cover status of a set element
    struct VertexData {
      int eid = -1;
      int eff = -1;
    };

    // Stores the data for each vertex in a hashtable with <element,data> pairs
    std::unordered_map<int,VertexData> vertices;

    // Computes the level associated with an input set size
    int computeLevel(const int setSize);

    // Finds an effective subset of maximal size
    std::vector<int> computeEffectiveSubsetSort(std::vector<std::pair<int,VertexData>>& vs);

    // Finds an effective subset of maximal size
    std::vector<int> computeEffectiveSubsetCount(std::vector<std::pair<int,VertexData>>& vs);

    // Retrieves the VertexData (if any) associated with each element in the input set
    std::vector<std::pair<int,VertexData>> fetchVertexData(const std::vector<int>& set);
};