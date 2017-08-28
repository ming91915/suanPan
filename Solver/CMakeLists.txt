target_sources(${PROJECT_NAME} PRIVATE
        "Solver/Solver.cpp"
        "Solver/BFGS.cpp"
        "Solver/Newton.cpp"
        "Solver/ODE.cpp"
        "Solver/Ramm.cpp"
        "Solver/User_Function.cpp"
        "Solver/lapack_wrapper.cpp"
        "Solver/arpack_wrapper.cpp"
        )

add_subdirectory(ODE_Solver)
add_subdirectory(Integrator)