Benchmark
---------

**Note: Performance of all programs involved is machine dependent. Though, the result obtained can be reproduced on reader's machine and the difference will be relatively the same.**

### Purpose

This post is targeted to show the single threaded performance comparison between **suanPan** and other FEA platforms.

The model file can be found under `/Example/CPS4N50.supan`, use following command to run the model.

``` bash
./suanPan -f CPS4N50.supan
```

### Configuration

The specifications of hardwares used in following tests are:

-   Platform: Sandy Bridge (i7-2600)
-   Base Frequency: 3.4 GHz No Turbo
-   Memory: Duel Channel DDR3 1333

Other programs used:

-   ABAQUS
-   [OpenSees 2.5.0](http://opensees.berkeley.edu/)
-   [LAPACK 3.4.2](http://www.netlib.org/lapack/)
-   [OpenBLAS 0.2.20](http://www.openblas.net/)
-   [Intel MKL 2018](https://software.intel.com/en-us/mkl)

### Model Summary

The dimension of the panel is $1\times1$. The thickness of the panel is $1$. the top and bottom edges are free, the left edge is fully fixed and the right edge is subjected to a uniformly distributed shear load pointing towards the positive direction of $y$ axis.

The material model is simply elastic, with a modulus of $1$ and a Poisson's ratio of $1/3$.

The seed size is set to $0.02$ so that for each edge, there are $50$ elements. Thus there are in total $2601$ nodes and $50\times50=2500$ elements. The element used is CPS4.

Since each node has $2$ DoFs, there are $5202$ DoFs. Correspondingly the dimension of the global matrix is $5202\times5202$. If the nodes are properly labeled, a minimum half bandwidth can be found to be $105$. RCM algorithm gives an approximation of $205$, which is produced by both the RCM implementations in OpenSees and suanPan.

The step length is set to default $1$, choose a fixed stepping scheme with sub-step size of $0.01$. That is to say, there are $100$ sub-steps. The convergence tester is chosen to be the unbalanced displacement increment, i.e., $\Big|\Big|\Delta{}U_{n+1}\Big|\Big|<\epsilon$ in which $\Delta{}U_{n+1}=K^{-1}_nR_n$. So for each sub-step, there are at least two iterations, the first is to calculate the initial displacement increment, which is not normally converged as the unbalanced load is introduced in this iteration. The second increment should be very small (converged) as this is an elastic case. So in total there are $200$ solving calls, the global stiffness matrix will be formed for $200$ times.

### Several Notes on ABAQUS Model

Force ABAQUS to use symmetric matrix storage scheme, so the analysis step is

``` text
*Step, name=TrialStep, inc=100, unsymm=NO
*Static, direct
0.01, 1., 
```

For single threaded analysis, use default configuration to submit the job.

``` bash
abaqus job=ElasticPanel
```

The job summary from ABAQUS is

``` tex
     JOB TIME SUMMARY
       USER TIME (SEC)      =   20.400    
       SYSTEM TIME (SEC)    =  0.50000    
       TOTAL CPU TIME (SEC) =   20.900    
       WALLCLOCK TIME (SEC) =         21
```

The gross memory used by `standard.exe` is around $60$ MB. One double-precision floating number occupies $8$ bytes, $1$ KB stores $128$ doubles. So for storing the stiffness alone, at least $206.4576$ MB is required if a full storage scheme is used. Considering if a symmteric scheme is applied, the minimum cost is around $103$ MB. Apparently, ABAQUS uses band matrix stroage or skyline/profile storage scheme.

### Some notes on OpenSees Model

The element used is the build-in `quad` element (check [this link](http://opensees.berkeley.edu/wiki/index.php/Quad_Element)), this element is optimized so that the computation cost is minimized, as a result, the performance is superior. The speed boost brought by this element $20\%$ to $30\%$ compared to the conventional formulation is roughly.

The band symmetric matrix storage corresponds to the *BandSPD* system.

``` text
system BandSPD
```

The convergence tester used is [Norm Displacement Increment Test](http://opensees.berkeley.edu/wiki/index.php/Norm_Displacement_Increment_Test)

``` text
test NormDispIncr $tol $iter
```

For the purpose of analysis, other testers will work as well. But to guarantee the solver is called twice for each sub-step, the norm displacement increment is the right one to use.

### Results

Model Name: CPS4NL50.supan

| Platform           | Wallclock Time | Scaled Time | Memory Usage |    Max GFLOPS   |
|:-------------------|:--------------:|:-----------:|:------------:|:---------------:|
| ABAQUS             |      20.9      |     2.22    |      60      |        -        |
| OpenSees           |      16.6      |     1.77    |      30      | 10.921 (\_trsm) |
| suanPan (Ref.)     |       9.4      |     1.00    |      53      | 10.562 (\_gemv) |
| suanPan (OpenBlas) |       6.0      |     0.64    |      45      |        -        |
| suanPan (MKL)      |       5.6      |     0.60    |      55      |      22.493     |

Some explanations:

-   ABAQUS model is run in CLI instead of CAE.
-   OpenSees uses reference LAPACK (not sure about the version).
-   suanPan (Ref.) is linked to reference LAPACK 3.4.2.
-   suanPan (OpenBLAS) is linked to OpenBLAS 0.2.20 in single threaded mode.
-   suanPan (MKL) is linked to Intel MKL 2018 in sequential mode.
-   GFLOPS is obtained from Intel Adviser 2018. The \_trsm and \_gemv are the fastest LAPACK (BLAS) subroutines measured.
-   For problems of this scale (around several thousands or even greater), enabling multi-threading does not help to speed up the analysis. The spin time and overhead are way greater than the execution time of the most model operations.

### Some Discussions

-   ABAQUS uses Intel MKL as well thus should be fast. But in this special case it is slow.
-   Both OpenSees and suanPan (Ref.) use reference BLAS and LAPACK from [netlib](http://www.netlib.org/lapack/), the matrix solving procedure uses the same subroutine \_pbsv to solve the global system. It takes similar time to run for both programs as the maximum GFLOPS is close. The second fastest subroutine in suanPan (Ref.) is in fact \_trsm.
-   The difference between OpenSees and suanPan (Ref.) is $77\%$. It ought to come from the FE analysis flow. The difference between element level computation should be negligible as the formulation is simple and similar.
-   The performance of OpenBLAS and MKL is similar, which can be checked from OpenBLAS official site. Though, MKL is slightly faster in this case.
-   The running time of OpenSees almost triples the one of suanPan (MKL).

### Another Example Model

Following is the same model but with a dense mesh, the mesh grid is refined to $100\times100$ so there are $10000$ elements.

Model Name: CPS4NL100.supan

| Platform           | Wallclock Time | Scaled Time | Memory Usage |
|:-------------------|:--------------:|:-----------:|:------------:|
| ABAQUS             |       8.4      |     0.73    |      94      |
| OpenSees           |      21.2      |     1.84    |      104     |
| suanPan (Ref.)     |      11.5      |     1.00    |      229     |
| suanPan (OpenBlas) |       6.2      |     0.54    |      203     |
| suanPan (MKL)      |       5.9      |     0.51    |      231     |

With the increased problem size, in this case ABAQUS is fast. OpenSees is significantly slower than other platforms.
