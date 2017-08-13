target_sources(${PROJECT_NAME} PRIVATE
        "Element/Element.cpp"
        "Element/ElementTemplate.cpp")

add_subdirectory(Truss)
add_subdirectory(Membrane)
add_subdirectory(Section)
add_subdirectory(Cube)