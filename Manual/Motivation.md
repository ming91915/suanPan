OpenSees
--------

1.  **OpenSees is buggy.** Users can never predict when and where the problem will crash. Overflow and memory leak are easy to be spotted in the source codes.
2.  **OpenSees is slow regarding to analyses.** The analysis flow is unnecessarily complicated and may contribute up to half of the running time in a single-threaded case.
3.  **OpenSees is slow regarding to linear algebra manipulations.** OpenSees implements its own matrix and vector classes. The functionalities of those classes are very limited and most of them are slow implementations. For example, OpenSees implements its own matrix--matrix multiplication instead of using optimized `_gemm` subroutines. The performance is very limited in all cases compared to `_gemm` subroutines.
4.  **OpenSees does not support multi-threading and is not thread-safe.** OpenSees does support distributed computation via MPI so users can run it in cloud/cluster or distributed computation network. But it does not support parallel computation. Although one can always `mpirun` the problem locally, the additional communication overhead is unnecessary. Furthermore, it does not work if one simply tries to use OpenMP or other libraries to parallelize it. As there are a lot of classes (elements, material, etc.) using class-wise static variables. That is to say, all objects of the class use the same memory as the temporary workspace. This will cause racing conditions if the program is parallelized.
5.  **OpenSees does not support hybrid/mixed modeling.** In OpenSees, every model starts with the `model` command, in which a fixed global DoF number per node is assigned. This means OpenSees cannot model problems like truss/beam systems in which different nodes may have different DoF numbers. For example, once there is one node only connects truss elements, the rotational DoF of this particular node will be counted thus the global stiffness matrix generated by OpenSees will always be singular as it assumes a fixed DoF number for all nodes. To fix this deficiency, a large number of classes need to be redesigned.
6.  **OpenSees has incompletely implemented visualizations.** OpenSees tries to use native OpenGL to render some basic elements. It is slow and buggy and does not work on both NVIDIA and AMD cards. The `display` command is almost unusable.
7.  **OpenSees is very weak regarding to post-processing.** OpenSees implements unstructured serialization. It is okay if users only needs to plot some simple curves, but there is no easy solution when it comes to more complicated visualization.
8.  **OpenSees has limited auto-stepping control.** Users has to control the step-size, iteration scheme, convergence criterion manually. Some examples even tries different solvers and step-length in one model. When the iteration does not converge, it is more likely the problem itself is not in a solvable status rather than the solver fails to solve the system due to algorithm issues. So it is quite irrational to leave it for users to decide how to proceed. A more reasonable way to let the problem to decide whether to continue iteration or simply quit solving, as what most commercial platforms do.
