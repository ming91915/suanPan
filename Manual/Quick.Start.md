Quick Start
===========

**suanPan** has several wrappers. One of these resembles the OpenSees syntax. Users who are familiar with OpenSees modeling can quickly switch/adopt existing model to **suanPan** model. The input file is plain text file, the extension `.supan` is chosen to distinguish **suanPan** model from other files. But it is not compulsory.

Basic Structure
---------------

The input file can be split into three parts. They are: model information, analysis configuration and data recording.

``` text
# a typical model has three parts
model information <--- order does not matter
analysis configuration <--- order matters between steps but does not matter within one step
data recording <--- order does not matter
```

By default, **suanPan** creates a `domain` with tag 1 and uses it as the working domain. Analysts do not need to explicitly create a `domain` before defining other stuff. If needed, it is possible to switch from one domain to another or create a new domain, using `domain` command.

``` text
suanPan --> domain 1
create_new_domain() switches to Domain 1.
suanPan --> domain 2
create_new_domain() successfully creates Domain 2.
suanPan --> domain 3
create_new_domain() successfully creates Domain 3.
suanPan --> domain 2
create_new_domain() switches to Domain 2.
suanPan --> remove domain 2 3 1
erase_domain() is about to delete Domain 2, are you sure? [Y/n]
erase_domain() switches to Domain 1.
erase_domain() is about to delete Domain 3, are you sure? [Y/n]
erase_domain() switches to Domain 1.
erase_domain() is about to delete Domain 1, are you sure? [Y/n]
erase_domain() removes the last domain and switches to default Domain 1.
suanPan --> exit
```

### Model Information

In the first part, analysts are required to define the basic model information, such as nodes, elements and materials in this part. The order of those commands does not matter, **suanPan** does not initialize objects right after creation. So it is alright to define something like

``` text
node 1 0 0
node 2 4 0
node 3 0 -3

material Elastic2D 1 1000 .2

element CP3 1 1 3 2 1
```

Or in a reverse order

``` text
# define a CP3 element with tag 1 connects nodes 1 node 3 and node 2 and use material 1
element CP3 1 1 3 2 1
material Elastic2D 1 1000 .2
node 3 0 -3
node 2 4 0
node 1 0 0
```

Although the first command `element` depends on the remaining commands.

### Analysis Configuration

The analysis configuration in **suanPan** resembles the one in ABAQUS. It is `step` oriented. By default, the analysis starts from `step 0`, users are allowed to create multi steps in a sequent order.

``` text
# starts with step 0
# configuration for step 0

fix 1 P 1 3 # boundary conditions
cload 1 200 2 2 # add concentrated load

# now starts a new analysis static step with tag 1
step static 1

# some steo confugurations
set ini_step_size .1
set fixed_step_size true

# can further define more steps
step static 2

# use analyze to start analysis
analyze
```

Everything defined under current step, before a new step uses current step by default. It is very similar to the ABAQUS work flow.

### Data Recording

If any recorder is defined before `analyze` command, users can save it to the disk. Users can also simply `peek` the information of current model component, for example

``` text
peek node 2
peek element 1
```

To quit the problem, use `exit`.

A Simple Example
----------------

``` text
element CP3 1 1 3 2 1

material Elastic2D 1 1000 .2

node 3 0 -3
node 2 4 0
node 1 0 0

fix 1 P 1 3

cload 1 200 2 2

step static 1
set ini_step_size .1
set fixed_step_size true

analyze

peek node 2

peek element 1

exit
```
