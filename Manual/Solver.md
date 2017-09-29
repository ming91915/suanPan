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

Arc-Length Method
-----------------

Arc-length method is extremely useful when both snap-back and snap-through occur in the problem. Unlike load/displacement control method, the arc-length method solves load and displacement simultaneously. Variants may include Wempner's version, Riks's version, Ramm's version and Crisfield's version. The Crisfield's version is most popular one, but it solves a quadratic equation in each iteration to determine the loading direction, which is a bit unnecessary. The Ramm's version is a more concise one and it only needs to calculate the sign of the stiffness matrix at the first iteration of each sub-step. If the stiffness is already factorized, computing its sign almost costs no additional resources. A modified version based on Ramm's method is also adopted by ABAQUS.

Searching on either a plane or a sphere is feasible and assuming the increment is small, the difference is negligible. But the plane version is simpler regarding implementation. **suanPan** by default uses $0.0001$ as the initial arc length $\Delta{}l$ and automatically adjusts it in subsequent steps. The iteration limit $m_{max}$ defined in the Converger used also acts as the target iteration in arc-length step. **suanPan** uses
$\begin{gather}\Delta{}l_{n+1}=\Delta{}l_n\sqrt{\dfrac{m_{max}}{m_n}}\end{gather}$
to scale the arc length according to current iteration number $m_n$ for each converged sub-step. If any sub-step fails to converge within $m_{max}$, the arc length is simply halved. This is approximately equal to assume the solver may converge within $4m_{max}$ iterations.

Compared to other solving schemes, arc-length method is not very stable, as it depends on not only displacement but also load and they are further related via stiffness. Ideally, the reference load should be scaled so that the solution space is smooth enough. But since we do not know how complicated the solution path is and we can predict neither the maximum displacement nor the maximum load, **suanPan** takes whatever reference load defined in `step` command. Users are sugguested to choose the reference load carefully as the algorithm may fail/succeed by simply tuning the magnitude of reference load.
