# suanPan
## Some highlights of suanPan
- suanPan utilizes the new language features introduced in the latest standards (C++11 and C++14), such as the new STL containers `unordered_set` and `unordered_map`, smart pointers `shared_ptr`, `weak_ptr` and `unique_ptr` and many other features.
- suanPan does not implement any linear algebra calculations. Hence code writers do not need to worry about the `real' computation part.

## So What Are Current Drawbacks in OpenSees?
There are a couple of reasons make me to write a more versatile platform on top of OpenSees.
### Complicated Configuration
Although OpenSees allows users to configure every tiny details, such as matrix storage, solver algorithm and so on, it is NOT a common practice. Check how ANSYS, Abaqus or even ANSYS Workbench set up the model configuration. The program should take care of most configurations, some of which should be hidden from users. 
### Incorrect Modeling
OpenSees does not support the modeling of certain types of hybrid structure. Consider a beam frame, to set one foot to pinned support, the user can simple use `fix` command to release the constraint at the third DoF. But what about we need an additional truss bracing/support? As each node has three DoFs defined to accommodate the rotational DoF of beam elements, for the node only connecting truss elements, the stiffness at the third DoF will always be zero hence the stiffness matrix will be singular. Users may need additional configurations to make it work.
### Manually Controlled Iteration
Just like what is adopted in commercial softwares, a more natural analysis scheme should support auto--stepping. OpenSees uses a manually controlled scheme that makes the nonlinear analysis difficult to carry out. Personally, I think Abaqus shows a good example of stepping strategy.
### Ugly Linear Algebra Support
OpenSees uses common linear algebra libraries such as BLAS and LAPACK. Meanwhile, it defined its own `Vector` and `Matrix` classes. However, the functionality of those classes are incomplete. The operators are not fully overloaded so that some basic operations lead to undefined behavior. Furthermore, What if users want to do a spectral factorization? They shall write the corresponding method to call LAPACK subroutine from scratch. A uniform layer that deals with all linear algebra stuff should be there.
### Poor Sparse Matrix Support
The build-in matrix class is a dense matrix. Although there are some sparse solvers existing in the source code, according to the official document, only one system is available. Apart from equation solving, no other function is supported.
### Poor Parallel Support
Distributed computation is supported by OpenSees but parallel computation is not very powerful. There are parallel version using MPICH available for downloading. A better choice is Open MPI for uniformity, portability and performance.
### GUI Support
Pre--process is not a very severe problem in OpenSees, as one can always use other softwares to generate the model can convert it to tcl scripts. Meanwhile there are other tools developed for pre--processing. But in deed post-processing is a big issue. As the `recorder` is not a very efficient command for recording data.
### More to Discover
...
## Design Structure
The linear algebra manipulations are isolated from the modeling functionality. The main data type used are `vec`, `mat` and `sp_mat` in [Armadillo](http://arma.sourceforge.net/) which is a template C++ linear algebra library that uses BLAS, LAPCK, ARPACK and SuperLU libraries as base layer.
## So Why Armadillo?
There are a couple of different choices for basic linear algebra manipulations, such as Blitz++, Eigen3, Armadillo and so on. Blitz++ is not under active development currently and it does not support sparse matrix. Eigen3 is good but personally I do not like the naming convention it adopts. Besides, the syntax is slightly complicated. More importantly, from the perspective of performance, Eigen3 is not remarkably faster than Armadillo. There are heaps of benchmarks on the Internet, if interested, users are recommended to investigate the comparisons between those libraries.