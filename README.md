# varint: Inverted Index Compression Library

[![Build Status](https://secure.travis-ci.org/stephanepoirier/varint.png?branch=master)](https://travis-ci.org/stephanepoirier/varint)
[![Coverage Status](https://coveralls.io/repos/github/stephanepoirier/varint/badge.svg?branch=master)](https://coveralls.io/github/stephanepoirier/varint?branch=coverage)


## Description

varint uses SIMD instructions and CPU cache to efficiently compress sorted integer arrays and performs highly efficient operations on compressed arrays. varint originates from the [zsearch](https://github.com/victorparmar/zsearch) search engine.

Fast operation examples:
- Sequential traversal
- Random lookup
- Finding intersections (lookup in combination of several bitmaps indices)

Compression needs to be data-type and data-distribution dependent.


## Installation

### Requirements

- CMake (>= 3.2)
- GCC (>= 4.8)


### Library setup
Get the code:
```Bash
mkdir varint && cd varint
git clone https://github.com/stephanepoirier/varint.git .
```

Build the library and run tests:
```Bash
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make all test
```

Install the library:
```Bash
make install
```

Next, [integrate](#integration) varint into your project.


### Integration

Integrate varint into your project following any of the following methods:
- [Using CMake](#using-cmake)
- [Manually](#manually)


#### Using CMake

After installing varint, add the following to your CMake project:
```cmake
find_package("varint" REQUIRED)
```

...and link your target against varint:
```cmake
target_link_libraries(my_target varint)
```

This will make varint's include directories and library available to your target.


#### Manually

varint's include directory and library file are located at the root of your installation directory.


## Documentation

Generate the API documentation using `make doc` (requires [Doxygen](www.doxygen.org/)).

## Contact

General:
- Maxime (maximecaron@gmail.com)
- Victor (victorparmar@gmail.com)

Build/integration:
- Stephane (stephane.poirier01@gmail.com)


## Credits

- [Daniel Lemire](https://github.com/lemire)'s SIMD-related libraries:
  - [simdcomp](https://github.com/lemire/simdcomp)
  - [SIMDCompressionAndIntersection](https://github.com/lemire/SIMDCompressionAndIntersection)
- [Lars Bilke](https://github.com/bilke):  [CodeCoverage](https://github.com/bilke/cmake-modules/blob/master/CodeCoverage.cmake) CMake module


## References
* Daniel Lemire, Leonid Boytsov, Nathan Kurz, SIMD Compression and the Intersection of Sorted Integers, arXiv: 1401.6399, 2014
http://arxiv.org/abs/1401.6399
