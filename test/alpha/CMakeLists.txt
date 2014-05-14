FOREACH (name   test_baseline_greschd
                test_baseline_mskoenz
                test_interface
                test_rng_baseline_greschd
                baseline_count_ops
                #~ test_validation # replaced by validation in ../
                test_v1
                test_int2t_vec
                zorder
    )
    add_executable(${name} ${name}.cpp)
    add_dependencies(${name} ${SRC}/conf.hpp)
    target_link_libraries(${name} ${LIB})
    add_default_test(${name})
ENDFOREACH(name)
