target_sources(${PROJECT_NAME} PRIVATE
        "Constraint/Constraint.cpp"
        "Constraint/MPC.cpp"
        "Constraint/Tie.cpp"
        )

add_subdirectory(BC)
add_subdirectory(Criterion)