add_subdirectory("alpha")

FOREACH (name   msk_v1_test
                baseline_greschd_test
                int2t_greschd_test
                validation
    )
    add_executable(${name} ${name}.cpp)
    add_dependencies(${name} ${SRC}/conf.hpp)
    target_link_libraries(${name} ${LIB})
    add_default_test(${name})
ENDFOREACH(name)

FOREACH (name   screen_performance
    )
    add_executable(${name} ${name}.cpp)
    add_dependencies(${name} ${SRC}/conf.hpp)
    target_link_libraries(${name} ${LIB})
    #add_default_test(${name})
ENDFOREACH(name)
