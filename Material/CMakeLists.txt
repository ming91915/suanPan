target_sources(${PROJECT_NAME} PRIVATE
        "Material/Material.cpp"
        "Material/MaterialTemplate.cpp")

add_subdirectory(1D)
add_subdirectory(2D)
add_subdirectory(3D)