Solver
======

A solver in suanPan is in charge of predicting the next trial status according to current status.

In detail, denoting current (pseudo) time as $T_n$, the corresponding status is $U_n$, $U_n$ may represent any quantity required to describe the status of the system, such as current displacement, velocity, acceleration, resistance and extern load level. Now give an time increment $\Delta{}T$, a solver needs to find an estimation of the status $\hat{U}_{n+1}$ at new time point $T_{n+1}=T_n+\Delta{}T$.

Please note that, it is stated as *to find an estimation of the status $\hat{U}_{n+1}$* instead of to find a ***converged*** trial status. As the algorithm has to stop according to some given criterion, the converged status may not be found for limited steps. Hence, accordingly, the error estimation should also be provided be the solver.
