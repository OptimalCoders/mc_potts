FOREACH (name   performance_example
                rng_performance_example
                sim_performance_example
                version_comparison
    )
    add_executable(${name} ${name}.cpp)
    add_dependencies(${name} ${SRC}/conf.hpp)
    target_link_libraries(${name} ${LIB})
    add_default_test(${name})
ENDFOREACH(name)
