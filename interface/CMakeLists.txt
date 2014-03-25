FOREACH (name   test_prototype_interface
    )
    add_executable(${name} ${name}.cpp)
    add_dependencies(${name} ${SRC}/conf.hpp.in)
    target_link_libraries(${name} ${LIB})
    add_default_test(${name})
ENDFOREACH(name)
