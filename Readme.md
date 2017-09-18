suanPan
=======

[![License: GPL v3](https://img.shields.io/badge/License-GPL%20v3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)
[![Codacy Badge](https://api.codacy.com/project/badge/Grade/7cb47e58d7dc4c1680c2205c4ba02e72)](https://www.codacy.com/app/TLCFEM/suanPan?utm_source=github.com&utm_medium=referral&utm_content=TLCFEM/suanPan&utm_campaign=Badge_Grade)
[![Build status](https://ci.appveyor.com/api/projects/status/mi0hh9inkb30yewj?svg=true)](https://ci.appveyor.com/project/TLCFEM/suanpan)
[![Build Status](https://travis-ci.org/TLCFEM/suanPan.svg?branch=master)](https://travis-ci.org/TLCFEM/suanPan)

Intro
-----

**suanPan** is a platform for FEM simulation aimed to applications in solid mechanics. The name Suan Pan (算盤) is literally [Chinese abacus](https://en.wikipedia.org/wiki/Suanpan). suanPan is written in C++ incorporating with several high performance libraries.

Features
--------

The highlights of **suanPan** are

-   **suanPan** is fast. Please check the benchmark section to see the performance.
-   **suanPan** is open-source and expandable.
-   **suanPan** separates the FEM computation framework from linear algebra manipulations, which significantly simplify the procedure of development.
-   **suanPan** utilizes the new language features introduced in the latest standards (C++11 and C++14), such as the new STL containers, smart pointers and many other features.

How to Compile
--------------

**suanPan** uses `CMake` to manage builds. Simply use

``` bash
cmake . && make
```

to compile the program. The package is tested with `Intel C++` and `MSVC` under Windows and `GCC` under both Windows and Ubuntu. As **suanPan** uses new features introduced in C++11 and C++14, please use `Visual Studio 2015` and/or later version, `GCC 5.0` and/or later version.

Dependency
----------

Additional libraries are

-   [Armadillo](http://arma.sourceforge.net/)
-   [MKL](https://software.intel.com/en-us/mkl)
-   [SuperLU](http://crd-legacy.lbl.gov/~xiaoye/SuperLU/)
-   [OpenBLAS](http://www.openblas.net/)
-   [HDF5](https://www.hdfgroup.org/)

Those libraries may depend on other libraries such as

-   [BLAS](http://www.netlib.org/blas/)
-   [LAPACK](http://www.netlib.org/lapack/)
-   [ARPACK](http://www.caam.rice.edu/software/ARPACK/)

