target_sources(${PROJECT_NAME} PRIVATE
        "Solver/Solver.cpp"
        "Solver/BFGS.cpp"
        "Solver/CentralDifference.cpp"
        "Solver/Newmark.cpp"
        "Solver/Newton.cpp"
        "Solver/ODE.cpp"
        "Solver/Ramm.cpp"
        "Solver/User_Function.cpp"
        "Solver/ODE_INSTANCE.cpp")

add_subdirectory(ODE_Solver)