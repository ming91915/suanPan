target_sources(${PROJECT_NAME} PRIVATE
        "Domain/Domain.cpp"
        "Domain/DomainBase.cpp"
        "Domain/ExternalModule.cpp"
        "Domain/Factory.hpp"
        "Domain/Node.cpp"
        "Domain/Storage.hpp"
        "Domain/Tag.cpp"
        )

add_subdirectory(MetaMat)