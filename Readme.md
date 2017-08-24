# suanPan

suanPan is a platform for FEM simulation aimed to applications in solid mechanics. The name Suan Pan (算盤) is literally [Chinese abacus](https://en.wikipedia.org/wiki/Suanpan). suanPan is written in C++ incorporating with several high performance libraries.

## Features

The highlights of suanPan are

- suanPan is fast. Please check the benchmark section to see the performance.
- suanPan is open-source and expandable.
- suanPan separates the FEM computation framework from linear algebra manipulations, which significantly simplify the procedure of development.
- suanPan utilizes the new language features introduced in the latest standards (C++11 and C++14), such as the new STL containers, smart pointers and many other features.

## Dependency

Additional libraries are

- [Armadillo](http://arma.sourceforge.net/)
- [MKL](https://software.intel.com/en-us/mkl)
- [SuperLU](http://crd-legacy.lbl.gov/~xiaoye/SuperLU/)
- [OpenBLAS](http://www.openblas.net/)
- [HDF5](https://www.hdfgroup.org/)

Those libraries may depend on other libraries such as

- [BLAS](http://www.netlib.org/blas/)
- [LAPACK](http://www.netlib.org/lapack/)
- [ARPACK](http://www.caam.rice.edu/software/ARPACK/)
