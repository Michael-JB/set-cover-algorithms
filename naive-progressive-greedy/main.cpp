#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <unordered_set>
#include <vector>

#include <sys/time.h>
#include <sys/resource.h>

std::unordered_set<int> coveredElements;
std::vector<int> cover;

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

// Returns a \ b
std::vector<int> setDifference(const std::vector<int>& a, const std::unordered_set<int>& b) {
  std::vector<int> result;
  for (const int element : a) {
    std::unordered_set<int>::const_iterator elementInB = b.find(element);
    if (elementInB == b.end()) {
      result.push_back(element);
    }
  }
  return result;
}

// Processes a set
void offerSet(const std::vector<int>& set, const int index, const float threshold) {
  // Add the set to the solution if the uncovered element count exceeds the threshold
  const std::vector<int> diff = setDifference(set, coveredElements);
  if (diff.size() >= threshold) {
    cover.push_back(index);
    coveredElements.insert(diff.begin(), diff.end());
  }
}

// Calculates the uncovered element threshold for a given n (base), p (passes total), and j (passes made)
float calculateThreshold(const int n, const int p, const int j) {
  return pow((float) n, (1 - ((float) j / (float) p)));
}

int main(int argc, char** argv) {
  std::string setFileName, coverFileName;
  int p = 1;

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
    std::string line;

    // Calculate delta in a pre-processing pass
    int delta = 0;
    while (getline(setFile, line)) {
      const std::vector<int> data = parseLine(const_cast<char*>(line.c_str()), ' ');
      if (data.size() > delta) delta = data.size();
    }

    // Make p passes over the data set
    const auto t1 = std::chrono::high_resolution_clock::now();
    for (int j = 1; j <= p; j++) {
      // Return to beginning of file
      setFile.clear();
      setFile.seekg(0, std::ios::beg);

      // Do a pass of the stream
      const float threshold = calculateThreshold(delta, p, j);
      int index = 0;
      while (getline(setFile, line)) {
        const std::vector<int> data = parseLine(const_cast<char*>(line.c_str()), ' ');
        offerSet(data, index++, threshold);
      }
    }
    const auto t2 = std::chrono::high_resolution_clock::now();
    const std::chrono::duration<double, std::milli> totalTime = t2 - t1;

    setFile.close();
    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);

    std::cout << "File: " << setFileName << std::endl
              << "Algorithm: Naive Progressive Greedy (p=" << p << ")" << std::endl
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