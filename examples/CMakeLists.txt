add_subdirectory("alpha")

FOREACH (name   micro_bench_example
    )
    add_executable(${name} ${name}.cpp)
    add_dependencies(${name} ${SRC}/conf.hpp)
    target_link_libraries(${name} ${LIB})
    add_default_test(${name})
ENDFOREACH(name)
