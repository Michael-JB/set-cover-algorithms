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

int main(int argc, char** argv) {
  std::string setFileName, coverFileName;

  // Parse command line arguments
  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " SETFILE [COVERFILE]" << std::endl;
    return 1;
  } else {
    setFileName = argv[1];
    if (argc >= 3) {
      coverFileName = argv[2];
    }
  }

  // Open file stream
  std::ifstream setFile(setFileName);
  if (setFile.is_open()) {
    SSSC sssc;
    int index = 0;
    std::string line;

    // Process data set
    const auto t1 = std::chrono::high_resolution_clock::now();
    while (getline(setFile, line)) {
      const std::vector<int> data = parseLine(const_cast<char*>(line.c_str()), ' ');
      sssc.offerSet(data, index++);
    }
    const auto t2 = std::chrono::high_resolution_clock::now();
    const std::unordered_set<int> cover = sssc.extractCoverSet();
    const auto t3 = std::chrono::high_resolution_clock::now();

    setFile.close();
    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);

    const std::chrono::duration<double, std::milli> offerTime = t2 - t1;
    const std::chrono::duration<double, std::milli> extractionTime = t3 - t2;
    const std::chrono::duration<double, std::milli> totalTime = t3 - t1;

    std::cout << "File: " << setFileName << std::endl
              << "Algorithm: SSSC" << std::endl
              << "Cover size: " << cover.size() << std::endl
              << "Offer time: " << offerTime.count() << "ms" << std::endl
              << "Extraction time: " << extractionTime.count() << "ms" << std::endl
              << "Total time: " << totalTime.count() << "ms" << std::endl
              << "Peak RAM usage: " << usage.ru_maxrss * 0.000001 << "MB" << std::endl;

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