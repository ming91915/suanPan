suanPan
=======

[![License: GPL v3](https://img.shields.io/badge/License-GPL%20v3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)
[![Codacy Badge](https://api.codacy.com/project/badge/Grade/7cb47e58d7dc4c1680c2205c4ba02e72)](https://www.codacy.com/app/TLCFEM/suanPan?utm_source=github.com&utm_medium=referral&utm_content=TLCFEM/suanPan&utm_campaign=Badge_Grade)
[![Build status](https://ci.appveyor.com/api/projects/status/mi0hh9inkb30yewj?svg=true)](https://ci.appveyor.com/project/TLCFEM/suanpan)
[![Build Status](https://travis-ci.org/TLCFEM/suanPan.svg?branch=master)](https://travis-ci.org/TLCFEM/suanPan)
[![codecov](https://codecov.io/gh/TLCFEM/suanPan/branch/master/graph/badge.svg)](https://codecov.io/gh/TLCFEM/suanPan)

Intro
-----

**suanPan** is an FEM simulation platform for applications in solid mechanics, civil/structural/seismic engineering. The name **suanPan** (in some certain places **suPan**) comes from *Suan Pan* (算盤), which is literally [Chinese abacus](https://en.wikipedia.org/wiki/Suanpan). **suanPan** is written in *C++* and is targeted to provide an efficient, concise and reliable FEM simulation platform.

**suanPan** is partially influenced by some popular (non-)commercial FEA packages, such as [ABAQUS UNIFIED FEA](https://www.3ds.com/products-services/simulia/products/abaqus/), [ANSYS](http://www.ansys.com/) and [OpenSees](http://opensees.berkeley.edu/).

Features
--------

The highlights of **suanPan** are

-   **suanPan** is fast. Please check the benchmark section to see the performance.
-   **suanPan** is open source and easy to be expanded to incorporate with user defined elements, materials, etc.
-   **suanPan** separates the FEM computation flow from linear algebra computation, which significantly reduce the complexity of development.
-   **suanPan** utilizes the new language features shipped with the latest standards (*C++11* and *C++14*), such as new STL containers, smart pointers and many other features.

How to Compile
--------------

As **suanPan** uses new language features, please use

-   **MSVC++ 14** (corresponds to *Visual Studio 2015*) and/or later version,
-   **GNU GCC 5.0** and/or later version.

The program is deliberately designed to disable the backward compatibility.

**suanPan** uses [CMake](https://cmake.org/) to manage builds.

### Windows

The package is tested under Windows with `MSVC++`, `GNU GCC`, `Intel C++ Compiler` and `Clang`. The libraries (only 64-bit):

-   BLAS and LAPACK
-   OpenBLAS
-   ARPACK
-   SuperLU
-   HDF5

are shipped with the package.

#### Visual Studio

The default VS solution uses reference BLAS and LAPACK. Simply `Build (F7)` the solution. You can change the BLAS and LAPACK to OpenBLAS or Intel MKL, if you have those libraries available. Since OpenBLAS is quite platform dependent, you are recommended to compile your own version instead of using the shipped one, which may probably not work for your machine.

#### GNU GCC

Use CMake to generate Makefiles, assume current folder is the root of the package and your are using `MinGW`, then the commands should look like this.

``` bash
mkdir cmake-build
cd cmake-build
cmake -G "MinGW Makefiles" ..
make
```

To switch to OpenBLAS, you can use following option.

``` cpp
cmake -G "MinGW Makefiles" -DUSE_OPENBLAS=ON ..
```

### Ubuntu

If you want to use HDF5, you have to compile and install it first. A useful link is [HDF5 SOURCE CODE](https://support.hdfgroup.org/HDF5/release/obtainsrc.html).

Again, the shipped OpenBLAS may not work on your machine, please compile your own library if you want to use OpenBLAS.

A default configuration is enough for most cases. Simply

``` cpp
cmake . && make
```

Dependency
----------

Additional libraries that may be used in **suanPan** are

-   [Armadillo](http://arma.sourceforge.net/)
-   [MKL](https://software.intel.com/en-us/mkl)
-   [SuperLU](http://crd-legacy.lbl.gov/~xiaoye/SuperLU/)
-   [OpenBLAS](http://www.openblas.net/)
-   [HDF5](https://www.hdfgroup.org/)

Those libraries may depend on other libraries such as

-   [BLAS](http://www.netlib.org/blas/)
-   [LAPACK](http://www.netlib.org/lapack/)
-   [ARPACK](http://www.caam.rice.edu/software/ARPACK/)

