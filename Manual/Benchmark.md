Benchmark
---------

Note: Performance of all programs involved is machine dependent.

### Example Alpha --- Elastic Loading of A Panel

The dimension of the panel is $1\times1$. The thickness of the panel is $1$. the top and bottom edges are free, the left edge is fully fixed and the right edge i subjected to a uniformly distributed shear load pointing towards the positive direction of $y$ axis.

The material model is simply elastic, with a modulus of $1$ and a Poisson's ratio of $1/3$.

The seed size is set to $0.02$ so that for each edge, there are $50$ elements.Thus there are in total $2601$ nodes and $50\times50=2500$ elements. The element used is CPS4.

Since each node has $2$ DoFs, there are $5202$ DoFs. Correspondingly the dimension of the global matrix is $5202\times5202$.

The step length is set to default $1$, choose a fixed stepping scheme with sub-step size of $0.01$. That is to say, there are $100$ sub-steps and the global matrix will be reformed $100$ times.

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

| Platform | Wallclock Time | Scaled Time | Memory Usage |
|:--------:|:--------------:|:-----------:|:------------:|
|  ABAQUS  |      21.2      |     2.26    |      60      |
|  suanPan |       9.4      |     1.00    |      53      |
| OpenSees |      10.3      |     1.10    |      30      |


