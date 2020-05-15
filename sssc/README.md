# Semi-Streaming Set Cover
An implementation of Semi-Streaming Set Cover [[1]](#1), a one-pass semi-streaming algorithm for set cover.

# Usage
A `Makefile` is included, which facilitates the build process:
  - `$ make` to build the executable
  - `$ make clean` to remove the executable

Once built, run the algorithm:
```
./main SETFILE [COVERFILE]
```

  - `SETFILE`: the input data file, e.g. `../input-example.dat`
  - `COVERFILE`: the output cover file (optional), e.g. `cover.dat`

## References
<a id="1">[1]</a>
Emek, Y., & Rosén, A. (2016).
Semi-streaming set cover.
ACM Transactions on Algorithms (TALG), 13(1), 1-22.