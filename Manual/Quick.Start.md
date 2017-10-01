Quick Start
===========

**suanPan** has several wrappers. One of those resembles the syntax of OpenSees. Users who are familiar with OpenSees modeling can quickly adapt existing models to run in **suanPan**.

Run Program
-----------

The program can be run from terminal (cmd). Simply run `./suanPan` or `suanPan.exe`, the program will start in command line interface and wait for inputs.

``` text
+--------------------------------------------------+
|   __        __        suanPan is an open source  |
|  /  \      |  \          FEM framework (64-bit)  |
|  \__       |__/  __   __          Acrux (0.1.0)  |
|     \ |  | |    |  \ |  |                        |
|  \__/ |__| |    |__X |  |     maintained by tlc  |
|                             all rights reserved  |
+--------------------------------------------------+

suanPan --> _
```

In **suanPan**, no additional configuration is required so users can directly define the model.

Uniaxial Tension of Truss Bar
-----------------------------

Let's start with a very simple example --- the uniaxial tension of a truss bar.

First we define a uniaxial elastic material with tag 1, assume the elastic modulus is $100$.

``` text
material Elastic1D 1 100
```

Two nodes, which are assumed to be located at $(0,0)$ and $(1,0)$, are needed to define a truss member.

``` text
node 1 0 0
node 2 1 0
```

Now we can define the truss element connecting those two nodes and using material 1 we just defined. Assume the cross section area is $10$.

``` text
element Truss2D 1 1 2 1 10
```

Assume the left node is pinned and right node is roller supported, apply a concentrated load pointing towards the positive direction of $x$ axis with a magnitude of $25$ at right node.

``` text
# define a fix BC with tag 1, restrain DoF 1 of node 1
fix 1 1 1
# define a fix BC with tag 2, restrain DoF 2 of node 1 and node 2
fix 2 2 1 2
# define a cload with tag 1, with a magnitude of 25, acting on node 2 dof 1
cload 1 25 1 2
```

To create an analysis step, use `step` command.

``` text
step static 1
```

Now we can run the analysis and peek the displacement of node 2.

``` text
analyze

peek node 2
```

The displacement at node 2 can be easily computed.

$\begin{gather} \Delta{}U=\dfrac{P}{EA}\cdot{}l=\dfrac{25}{100\cdot10}\cdot{}1=0.0250\end{gather}$

The output of `peek` command is directly printed on the screen.

``` text
...
suanPan --> peek node 2
Node 2:
   1.0000        0
Displacement:
   0.0250        0

suanPan -->
```

Screenshot
----------

The complete model is shown as follows. The execution time is printed when the program is terminated.

``` text
+--------------------------------------------------+
|   __        __        suanPan is an open source  |
|  /  \      |  \          FEM framework (64-bit)  |
|  \__       |__/  __   __          Acrux (0.1.0)  |
|     \ |  | |    |  \ |  |                        |
|  \__/ |__| |    |__X |  |     maintained by tlc  |
|                             all rights reserved  |
+--------------------------------------------------+

suanPan --> material Elastic1D 1 100
suanPan --> node 1 0 0
suanPan --> node 2 1 0
suanPan --> element Truss2D 1 1 2 1 10
suanPan --> fix 1 1 1
suanPan --> fix 2 2 1 2
suanPan --> cload 1 25 1 2
suanPan --> step static 1
suanPan --> analyze
suanPan --> peek node 2
Node 2:
   1.0000        0
Displacement:
   0.0250        0

suanPan --> exit
Finished in 6.611 seconds.
```
