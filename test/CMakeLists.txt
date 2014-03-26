FOREACH (name   test_baseline_greschd
                test_baseline_mskoenz
                test_prototype_interface
                test_rng_baseline_greschd
                test_validation
    )
    add_executable(${name} ${name}.cpp)
    add_dependencies(${name} ${SRC}/conf.hpp)
    target_link_libraries(${name} ${LIB})
    add_default_test(${name})
ENDFOREACH(name)
