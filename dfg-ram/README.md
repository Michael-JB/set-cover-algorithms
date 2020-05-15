# (RAM-based) Disk Friendly Greedy
An implementation of DFG [[1]](#1) suitable for data sets which can fully reside in RAM.

## Usage
A `Makefile` is included, which facilitates the build process:
  - `$ make` to build the executable
  - `$ make clean` to remove the executable

Once built, run the algorithm:
```
./main p SETFILE [COVERFILE]
```

  - `p`: input parameter which governs the approximation factor and running time. 1.05 is default. See [[1]](#1) for more information.
  - `SETFILE`: the input data file, e.g. `../input-example.dat`
  - `COVERFILE`: the output cover file (optional), e.g. `cover.dat`

## References
<a id="1">[1]</a>
Cormode, G., Karloff, H., & Wirth, A. (2010, October).
Set cover algorithms for very large datasets.
In Proceedings of the 19th ACM international conference on Information and knowledge management (pp. 479-488).
