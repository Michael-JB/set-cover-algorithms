# Naive Progressive Greedy
An implementation of Naive Progressive Greedy [[1]](#1), a multi-pass semi-streaming algorithm for set cover.

## Usage
A `Makefile` is included, which facilitates the build process:
  - `$ make` to build the executable
  - `$ make clean` to remove the executable

Once built, run the algorithm:
```
./main p SETFILE [COVERFILE]
```

  - `p`: the number of passes to make over the input
  - `SETFILE`: the input data file, e.g. `../input-example.dat`
  - `COVERFILE`: the output cover file (optional), e.g. `cover.dat`

## References
<a id="1">[1]</a>
Chakrabarti, A., & Wirth, A. (2016).
Incidence geometries and the pass complexity of semi-streaming set cover.
In Proceedings of the twenty-seventh annual ACM-SIAM symposium on Discrete algorithms (pp. 1365-1373).
Society for Industrial and Applied Mathematics.
