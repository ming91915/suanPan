### Matrix Storage Schemes

Currently, **suanPan** supports 4 different storage schemes, for different applications. They are

-   Full Matrix Storage
-   Band Matrix Storage
-   Symmetric Band Matrix Storage
-   Symmetric Matrix in Packed Format Storage

For asymmetric storage schemes, the [LU factorization](https://en.wikipedia.org/wiki/LU_decomposition) is used for solving the system. For symmetric ones, the [Cholesky factorization](https://en.wikipedia.org/wiki/Cholesky_decomposition) is used. To successfully conduct the Cholesky factorization, the matrix has to be **positive definite**. The positive definite property cannot be ensured for the stiffness matrices of some geometrically nonlinear problems, material softening problems and/or other relative applications. Please be aware of the problem type and choose proper storage schemes.

The [LAPACK](http://www.netlib.org/lapack/explore-html/) library is used as the base driver. The corresponding solvers used are

-   Full Matrix Storage --&gt; [\_gesv](http://www.netlib.org/lapack/explore-html/d7/d3b/group__double_g_esolve_ga5ee879032a8365897c3ba91e3dc8d512.html#ga5ee879032a8365897c3ba91e3dc8d512)
-   Band Matrix Storage --&gt; [\_gbsv](http://www.netlib.org/lapack/explore-html/d3/d49/group__double_g_bsolve_gafa35ce1d7865b80563bbed6317050ad7.html#gafa35ce1d7865b80563bbed6317050ad7)
-   Symmetric Band Matrix Storage --&gt; [\_pbsv](http://www.netlib.org/lapack/explore-html/de/d49/group__double_o_t_h_e_rsolve_ga9c26c8344bc125d78d6a33a22459169c.html#ga9c26c8344bc125d78d6a33a22459169c)
-   Symmetric Matrix in Packed Format Storage --&gt; [\_ppsv](http://www.netlib.org/lapack/explore-html/de/d49/group__double_o_t_h_e_rsolve_ga59a299a18cb0af017ee2c972ae7defb5.html#ga59a299a18cb0af017ee2c972ae7defb5)

Please visit the LAPACK website to check the details of those subroutines.

### Which Scheme to Use?

-   A safe yet efficient option is to use **Band Matrix Storage**. It requires relatively small storage cost while the solving speed is decent if the bandwidth is small.
-   If the matrix is 100% positive definite (symmetric as well), use **Symmetric Band Matrix Storage** to speed up further.
-   If the problem size is small, for example only a few elements are modeled, the global matrix may not be banded, or the bandwidth is not significantly smaller than the size of the matrix. Use **Full Matrix Storage** to avoid unnecessary abuse of memory.
-   **Symmetric Matrix in Packed Format Storage** should be used only if the matrix is not banded and the problem size is not large. If the problem is large, above several hundreds or several thousands. The solving speed is super slow, way slower than Full Matrix Storage.

### Is Sparse Necessary?

Currently there is no sparse support and won't be in the near future. Nevertheless, it may be added once I find a fast implementation.

It can be seen that for some cases, the storage cost of sparse matrix and symmetric band matrix can be similar, though it still depends on different problems and different numbering schemes. Assume both schemes requires similar amount of memory to store the given matrix, the solving speed of sparse matrix will normally be slower, as the indexing is more complicated (needs to iterate and loop up indices).

Meanwhile, for FEM application, it is necessary to clear and reassemble the matrix. In sparse scheme, inserting new element requires sorting of indices and loop-up and iteration. Assembling the matrix is very slow when the number of non zero elements is large.

If interested, readers can investigate the performance different between dense and sparse schemes. Armadillo provides a relatively decent sparse container, which may be used for benchmarking.
