# set-cover-algorithms
A repository containing C++ implementations of a range of approximation algorithms for the **Set Cover** problem:

  - Disk-based DFG [[1]](#1)
  - RAM-based DFG [[1]](#1)
  - Naive Progressive Greedy [[2]](#2)
  - Progressive Greedy [[2]](#2)
  - Emek-Rosen Semi-Streaming Set Cover [[3]](#3)
  - Multi-pass Semi-Streaming Set Cover

## Usage
Each implementation is contained its own directory. Usage instructions are given in corresponding `README.md` files.

### Data sets
These implementations operate on a common data format: a text file with one set per line, where each set is a space-delimited list of integers. See `input-example.dat` for an example.

All algorithms have the option of outputting a cover. This cover is a text file containing a set index on each line.

## License

[MIT license](./LICENSE)

## References
<a id="1">[1]</a>
Cormode, G., Karloff, H., & Wirth, A. (2010, October).
Set cover algorithms for very large datasets.
In Proceedings of the 19th ACM international conference on Information and knowledge management (pp. 479-488).

<a id="2">[2]</a>
Chakrabarti, A., & Wirth, A. (2016).
Incidence geometries and the pass complexity of semi-streaming set cover.
In Proceedings of the twenty-seventh annual ACM-SIAM symposium on Discrete algorithms (pp. 1365-1373).
Society for Industrial and Applied Mathematics.

<a id="3">[3]</a>
Emek, Y., & Rosén, A. (2016).
Semi-streaming set cover.
ACM Transactions on Algorithms (TALG), 13(1), 1-22.