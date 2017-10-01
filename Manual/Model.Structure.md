Model Components
================

If you are an OpenSees user, I bet after reviewing example models, you are ready to write your own models with the help of command manuals.

Normally users do not like to work in CLI mode and type in commands line by line. Instead, using an input file to establish the model and running the analysis automatically should be a better, more efficient option. It is supported in **suanPan**.

Input File
----------

The input files used in **suanPan** is plain text files. It is not compulsary though, the extension `.supan` is used to indicate the file is a **suanPan** input file.

The command parser is written from scratch and uses no external library. Hence it is less robust compared to other word/file processors.

Any line starts with the sharp symbol `#` indicates a comment line and will be skipped. Free free to use `#` to comment the model.

Run with Input File
-------------------

There are two ways to execute an input file. You can

-   Run the program with `-f` option to directly process the file. For example,

``` text
suanPan.exe -f MY_MODEL.supan
```

-   Load the file in CLI mode using `file` command. For example,

``` text
+--------------------------------------------------+
|   __        __        suanPan is an open source  |
|  /  \      |  \          FEM framework (64-bit)  |
|  \__       |__/  __   __          Acrux (0.1.0)  |
|     \ |  | |    |  \ |  |                        |
|  \__/ |__| |    |__X |  |     maintained by tlc  |
|                             all rights reserved  |
+--------------------------------------------------+

suanPan --> file MY_MODEL.supan
```

Basic Structure
---------------

The input file can be split into three parts. Take the model from previous section as example.

``` text
             --> | material Elastic1D 1 100
             |   | 
             |   | node 1 0 0
SETUP    ----|   | node 2 1 0
             |   | ...
             |   | 
             --> | element Truss2D 1 1 2 1 10
-------- --------|-------------------------------
             --> | step static 1
             |   | 
             |   | fix 1 1 1
ANALYSIS ----|   | fix 2 2 1 2
             |   | cload 1 25 1 2
             |   | 
             --> | analyze
-------- --------|-------------------------------
             --> | 
OUTPUT   ----|   | peek node 2
             --> | 
-------- --------|-------------------------------
                 | exit
```

### Model Setup

In the first part, users are required to define nodes, elements, section, materials, recorders, etc. or load files, external modules and/or other things. The order of commands does not really matter as everything will only be created but not initialized in this part. So it is legal to write something like follows, although element 1 depends on node 1 and node 2 and material 1.

``` text
element Truss2D 1 1 2 1 10
node 2 1 0
material Elastic1D 1 100
node 1 0 0
```

### Analysis

In the second part, users are asked to create analysis steps, similar to the analysis flow in ABAQUS, by default, a valid analysis flow starts with step 0. Users shall define multiple steps in a strictly increasing order. **Anything defined in between two steps belongs to the previous one and will persist in subsequent steps by default.** If this is not the behavior users want, it can be turned off manually.

``` text
           --> | ...
           |   |
STEP 0 ----|   |
           |   |
           --> | ...
---------------|-------------------------------
           --> | step static 1
           |   | ...
STEP 1 ----|   |
           |   |
           --> | ...
---------------|-------------------------------
           --> | step static 2
STEP 2 ----|   | ...
           --> | ...
---------------|-------------------------------
               | ...
```

So step 0 overlaps the first part. The step-specific configurations should be defined in corresponding step regions. This part ends with the `analyze` command, which triggers the analysis. Similar to ABAQUS, some prescribed conditions can be either defined in step 0 or step 1, so it is alright to write

``` text
fix 1 1 1
fix 2 2 1 2
cload 1 25 1 2
step static 1
analyze
```

### Output

The final part is output section. Users can check the value of specified object, or save the recorded data, or save the model for post-processing. Users can also continue analysis by defining more steps, or erase this model and create another one, or keep the existing model and switch to another model. Those functions will not be covered here.

A Modified Model
----------------

The order of commands is changed, check the difference and make sure you understand the flexibility of writing a model.

``` text
element Truss2D 1 1 2 1 10
node 2 1 0
material Elastic1D 1 100
node 1 0 0
fix 2 2 1 2
cload 1 25 1 2
fix 1 1 1
step static 1
analyze
peek node 2
exit
```
