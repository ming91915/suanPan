Solver
======

A solver in suanPan is in charge of predicting the next trial status according to current status.

In detail, denoting current (pseudo) time as $T_n$, the corresponding status is $U_n$, $U_n$ may represent any quantity required to describe the status of the system, such as current displacement, velocity, acceleration, resistance and extern load level. Now give an time increment $\Delta{}T$, a solver needs to find an estimation of the status $\hat{U}_{n+1}$ at new time point $T_{n+1}=T_n+\Delta{}T$.

Please note that, it is stated as *to find an estimation of the status $\hat{U}_{n+1}$* instead of to find a ***converged*** trial status. As the algorithm has to stop according to some given criterion, the converged status may not be found for limited steps. Hence, accordingly, the error estimation should also be provided be the solver.

Besides, since in most cases, the prediction/approximation of the trial status cannot be obtained within one step update, iterations are often needed. In this sense, to some degree,

Newton Method
-------------

When we say Newton's method, we often mean the Newton--Raphson method.

Quasi-Newton Method
-------------------

The most well known rank one quasi-Newton update should be Broyden family update. A rank two popular update is BGFS method.

Since BGFS is positive-definite update and is proved to fail for non-convex problems. For softening and buckling problems, it is not suitable to use BFGS solver.
