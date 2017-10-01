Multiple Steps in One Problem
=============================

As mentioned before, users can define multiple steps in one problem, here is a simple example.

``` text
material Elastic1D 1 100
node 1 0 0
node 2 1 0
element Truss2D 1 1 2 1 10

fix 1 1 1
fix 2 2 1 2

step static 1
cload 1 25 1 2

step static 2
cload 2 25 1 2

analyze

peek node 2

exit
```

There are two steps and two loads are defined in those steps. The over all load is $50$ hence the displacement should be doubled. **Please be aware of that each object should have a unique tag.**

The output should be

``` text
Node 2:
   1.0000        0
Displacement:
   0.0500        0
```
