target_sources(${PROJECT_NAME} PRIVATE
        "Element/Element.cpp"
        "Element/Section.cpp"
        "Element/ElementTemplate.cpp")

add_subdirectory(Truss)
add_subdirectory(Membrane)