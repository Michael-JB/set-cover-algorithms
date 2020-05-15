#include <stdlib.h>
#include <fstream>
#include <iostream>
#include "sssc.h"

#include <sys/time.h>
#include <sys/resource.h>

// Parses a string of delimited integers
std::vector<int> parseLine(char* line, const char delimiter) {
  std::vector<int> data;
  while (*line) {
    const int val = strtol(line, &line, 10);
    data.push_back(val);
    while (*line == delimiter) {
      line++;
    }
  }
  return data;
}

// Computes the critical threshold for universe restriction for the jth pass of p passes
float computeThreshold(float j, float p, float delta) {
  // return (((p-j+1)*log2(delta))+(3*j)-p-1)/(p+1); // Theoretical threshold
  return (p-j)*(ceil(log2(delta)))/p;
}

int main(int argc, char** argv) {
  std::string setFileName, coverFileName;
  int p = 2;

  // Parse command line arguments
  if (argc < 3) {
    std::cerr << "Usage: " << argv[0] << " p SETFILE [COVERFILE]" << std::endl;
    return 1;
  } else {
    p = std::stoi(argv[1]);
    if (p < 1) p = 1;
    setFileName = argv[2];
    if (argc >= 4) coverFileName = argv[3];
  }

  // Open file stream
  std::ifstream setFile(setFileName);
  if (setFile.is_open()) {
    SSSC sssc(2);
    int delta = 0;

    std::unordered_set<int> cover;

    // Make p passes over the stream
    const auto t1 = std::chrono::high_resolution_clock::now();
    for (int j = 1; j <= p; j++) {
      int index = 0;
      std::string line;
      while (getline(setFile, line)) {
        const std::vector<int> data = parseLine(const_cast<char*>(line.c_str()), ' ');
        if (data.size() > delta) delta = data.size(); // Calculate delta during first pass
        sssc.offerSet(data, index++);
      }

      const float threshold = computeThreshold(j, p, delta);
      std::unordered_set<int> restrictedUniverse;

      // Populate cover and restrict universe for next pass
      for (const std::pair<const int,SSSC::VertexData>& v : sssc.getCoverCertificate()) {
        if (j < p && v.second.eff <= threshold) {
          restrictedUniverse.insert(v.first);
        } else {
          cover.insert(v.second.eid);
        }
      }

      // Create new SSSC instance with restricted universe
      sssc = SSSC(2);
      sssc.restrictUniverse(restrictedUniverse);

      // Return to beginning of file
      setFile.clear();
      setFile.seekg(0, std::ios::beg);
    }
    const auto t2 = std::chrono::high_resolution_clock::now();

    setFile.close();
    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);
    const std::chrono::duration<double, std::milli> totalTime = t2 - t1;

    std::cout << "File: " << setFileName << std::endl
              << "Algorithm: MSSSC (p=" << p << ")" << std::endl
              << "Cover size: " << cover.size() << std::endl
              << "Total time: " << totalTime.count() << "ms" << std::endl
              << "Peak RAM usage: " << usage.ru_maxrss * 0.000001 << "MB" << std::endl;

    // Write cover to file if necessary
    if (!coverFileName.empty()) {
      std::ofstream coverFile(coverFileName);
      if (coverFile.is_open()) {
        for (const int &element : cover) {
          coverFile << element << std::endl;
        }
        std::cout << "Cover written to " << coverFileName << std::endl;
        coverFile.close();
      } else {
        std::cout << "Unable to open " << coverFileName << std::endl;
      }
    }
  } else {
    std::cout << "Unable to open " << setFileName << std::endl;
  }

  return 0;
}