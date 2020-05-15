#include <math.h>
#include <cstdint>
#include <unordered_map>

#include "dfg.h"

// Computes the k value associated with the inequality p^k <= n < p^(k+1)
int DFG::getSubCollForN(int n) {
  return floor(log(n) / log(p));
}

// Returns a \ b
std::vector<uint64_t> DFG::setDifference(const std::vector<uint64_t>& a, const std::unordered_set<uint64_t>& b) {
  std::vector<uint64_t> result;
  for (const uint64_t element : a) {
    std::unordered_set<uint64_t>::const_iterator elementInB = b.find(element);
    if (elementInB == b.end()) {
      result.push_back(element);
    }
  }
  return result;
}

// Adds the set to the appropriate S_(k), updating maxK if necessary
void DFG::addToSubCollection(const std::vector<uint64_t>& set, const uint64_t index) {
  if (set.size() > 0) {
    const int k = getSubCollForN(set.size());
    std::ofstream* outfile;

    std::unordered_map<int,std::ofstream*>::const_iterator outstream = outfiles.find(k);
    if (outstream == outfiles.end()) {
      std::string outfileName = getSubcollectionFileName(k);
      outfile = new std::ofstream(outfileName, std::ios::out | std::ios::binary);
      outfiles.insert({k, outfile});
    } else {
      outfile = outstream->second;
    }

    const uint64_t sentinel = UINT64_MAX;

    if (outfile->is_open()) {
      // Prepend index
      outfile->write((const char*)&index, sizeof(index));
      // Write set
      for (const uint64_t element : set) {
        outfile->write((const char*)&element, sizeof(element));
      }
      // Append sentinel
      outfile->write((const char*)&sentinel, sizeof(sentinel));
      bytesWritten += 8 * (set.size() + 2);
    } else {
      std::cerr << "Unable to open file for subcollection " << k << std::endl;
    }

    // Determine if this subcollection is the new maximum
    if (k > maxK) {
      maxK = k;
    }
  }
}

// Processes a set from the input stream
void DFG::offerSet(const std::vector<uint64_t>& set, const uint64_t index) {
  addToSubCollection(set, index);
}

// Returns the file name associated with a subcollecion id
std::string DFG::getSubcollectionFileName(int k) {
  return "s-" + std::to_string(k) + ".bin";
}

// Extracts a 1-cover from the subcoll and sets structures
std::vector<uint64_t> DFG::extractCover() {
  std::vector<uint64_t> cover;

  for (int k = maxK; k >= 0; --k) {
    std::unordered_map<int,std::ofstream*>::const_iterator outstream = outfiles.find(k);
    if (outstream != outfiles.end()) {

      // Close (obselete) writing stream for this subcollection
      outstream->second->close();
      delete outstream->second;
      outfiles.erase(outstream);

      // Open reading stream for this subcollection
      std::string infileName = getSubcollectionFileName(k);
      std::ifstream infile(infileName, std::ios::in | std::ios::binary);
      if (infile.is_open()) {
        while (infile) {
          uint64_t val, setIndex;
          std::vector<uint64_t> data;

          if (infile.read((char*)&setIndex, sizeof(setIndex))) {
            infile.read((char*)&val, sizeof(val));

            // Read 8-byte chunks from file until sentinel value is reached
            while(val != UINT64_MAX) {
              data.push_back(val);
              infile.read((char*)&val, sizeof(val));
            }
            bytesRead += 8 * (data.size() + 2);

            // Compute uncovered element count and add to solution or demote as appropriate
            const std::vector<uint64_t> diff = setDifference(data, coveredElements);
            if ((k == 0 && diff.size() == 1) || diff.size() >= pow(p, k)) {
              cover.push_back(setIndex);
              coveredElements.insert(diff.begin(), diff.end());
            } else {
              if (k != 0) {
                addToSubCollection(diff, setIndex);
              }
            }
          }
        }
        infile.close();

        // Now that subcollection is processed, it can safely be deleted
        std::remove(infileName.c_str());
      } else {
        std::cerr << "Unable to open " << infileName << std::endl;
      }
    }
  }

  return cover;
}

long long DFG::getBytesWritten() {
  return bytesWritten;
}

long long DFG::getBytesRead() {
  return bytesRead;
}