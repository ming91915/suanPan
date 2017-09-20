Benchmark
---------

**Note: Performance of all programs involved is machine dependent. Though, the result obtained can be reproduced on reader's machine and the difference will be relatively the same.**

### Purpose

This post is targeted to show the single threaded performance comparison between **suanPan** and other FEA platforms.

### Configuration

The specifications of hardwares used in following tests are:

-   Platform: Sandy Bridge (i7-2600)
-   Base Frequency: 3.4 GHz No Turbo
-   Memory: Duel Channel DDR3 1333

### Model Summary

The dimension of the panel is $1\times1$. The thickness of the panel is $1$. the top and bottom edges are free, the left edge is fully fixed and the right edge is subjected to a uniformly distributed shear load pointing towards the positive direction of $y$ axis.

The material model is simply elastic, with a modulus of $1$ and a Poisson's ratio of $1/3$.

The seed size is set to $0.02$ so that for each edge, there are $50$ elements. Thus there are in total $2601$ nodes and $50\times50=2500$ elements. The element used is CPS4.

Since each node has $2$ DoFs, there are $5202$ DoFs. Correspondingly the dimension of the global matrix is $5202\times5202$. If the nodes are properly labeled, a minimum half bandwidth can be found to be $105$. RCM algorithm gives an approximation of $205$, which is produced by both the RCM implementations in OpenSees and suanPan.

The step length is set to default $1$, choose a fixed stepping scheme with sub-step size of $0.01$. That is to say, there are $100$ sub-steps. The convergence tester is chosen to be the unbalanced displacement increment, i.e., $\Big|\Big|\Delta{}U_{n+1}\Big|\Big|<\epsilon$ in which $\Delta{}U_{n+1}=K^{-1}_nR_n$. So for each sub-step, there are at least two iterations, the first is to calculate the initial displacement increment, which is not normally converged as the unbalanced load is introduced in this iteration. The second increment should be very small (converged) as this is an elastic case. So in total there are $200$ solving calls, the global stiffness matrix will be formed for $200$ times.

### Several Notes on ABAQUS

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

The gross memory used by `standard.exe` is around $60$ MB. One double-precision floating number occupies $8$ bytes, $1$ KB stores $128$ doubles. So for storing the stiffness alone, at least $206.4576$ MB is required if a full storage scheme is used. Considering if a symmteric scheme is applied, the minimum cost is $103$ MB. Apparently, ABAQUS used band matrix stroage or skyline/profile storage scheme.

### Some notes on OpenSees

The element used is the build-in `quad` element (check [this link](http://opensees.berkeley.edu/wiki/index.php/Quad_Element)), this element is optimized so that the computation cost is minimized, as a result, the performance is very good. The speed boost compared to the conventional formulation is roughly $20\%$ to $30\%$.

The band symmetric matrix corresponds to the *BandSPD* system.

``` text
system BandSPD
```

The convergence tester used is [Norm Displacement Increment Test](http://opensees.berkeley.edu/wiki/index.php/Norm_Displacement_Increment_Test)

``` text
test NormDispIncr $tol $iter
```

For the purpose of analysis, other testers will work as well. But to guarantee the solver is called twice for each sub-step, the norm displacement increment is the right one to use.

### Results

|      Platform      | Wallclock Time | Scaled Time | Memory Usage | Max GFLOPS |
|:------------------:|:--------------:|:-----------:|:------------:|:----------:|
|       ABAQUS       |      20.9      |     2.26    |      60      |            |
|      OpenSees      |      16.6      |     1.10    |      30      |            |
|       suanPan      |       9.4      |     1.00    |      53      |            |
| suanPan (OpenBlas) |       6.0      |     0.72    |      45      |            |
|    suanPan (MKL)   |       5.6      |     0.72    |      55      |   22.493   |

In fact, the analysis flow should be similar for all platforms, the performance of the solver used should be the major factor that affects the overall speed.
