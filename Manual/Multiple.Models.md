Multiple Problems in One File
=============================

Commands Will Be Mentioned
--------------------------

``` text
domain # to switch/create domain
remove # to remove objects
delete # to remove objects
```

Main Content
------------

By default, **suanPan** creates a domain with tag 1 for use. Users can use `remove` or `delete` to delete certain domain or use `domain` to create new domains and/or switch from one to anther. For example,

``` text
suanPan --> domain 1
create_new_domain() switches to Domain 1.
suanPan --> domain 2
create_new_domain() successfully creates Domain 2.
suanPan --> domain 3
create_new_domain() successfully creates Domain 3.
suanPan --> domain 2
create_new_domain() switches to Domain 2.
suanPan --> remove domain 1
erase_domain() switches to Domain 2.
suanPan --> remove domain 2 3
erase_domain() switches to Domain 3.
erase_domain() removes the last domain and switches to default Domain 1.
suanPan --> exit
```

Just like what is available in ABAQUS, users can define multiple models in one file by switching from one domain to another. Furthermore, it is not necessary to `analyze` each domain separately before switching to other domains. After `analyze`, users can switch between domains to check the result. Following is a simple example.

Example
-------

``` text
# by default in domain 1
material Elastic1D 1 100
node 1 0 0
node 2 1 0
element Truss2D 1 1 2 1 10

fix 1 1 1
cload 1 25 1 2
fix 2 2 1 2

step static 1

# switch to domain 2
domain 2

# change parameters
material Elastic1D 1 200
node 1 0 0
node 2 6 0
element Truss2D 1 1 2 1 15

fix 1 1 1
cload 1 25 1 2
fix 2 2 1 2

step static 1

# analyze both domains
analyze

# now switch back to domain 1 and check displacement
domain 1
peek node 2

# check displacement in problem 2
domain 2
peek node 2

exit
```

Output
------

The output will be

``` text
create_new_domain() successfully creates Domain 2.
create_new_domain() switches to Domain 1.
Node 2:
   1.0000        0
Displacement:
   0.0250        0

create_new_domain() switches to Domain 2.
Node 2:
   6.0000        0
Displacement:
   0.0500        0

Finished in 0.035 seconds.
```

Multiple Steps in One Problem
=============================

As mentioned before, users can define multiple steps in one problem, here is a simple example.

``` text
material Elastic1D 1 100
node 1 0 0
node 2 1 0
element Truss2D 1 1 2 1 10

recorder 1 Node U 2

fix 1 1 1
fix 2 2 1 2

step static 1
cload 1 40 1 2

step static 2
cload 2 10 1 2

analyze

peek node 2

save recorder 1

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

The time against displacement plot can be seen as follows, which is a bilinear curve as expected.
![Time-Displacement](https://image.ibb.co/bvk5bG/001.png)
