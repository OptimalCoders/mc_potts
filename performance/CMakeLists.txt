add_custom_target(
            perf_search 
            COMMAND python3 ${SRC}/../performance/performance_search.py
            )
add_custom_target(
            perf_screen 
            COMMAND python3 ${SRC}/../performance/performance_screen.py
            )
add_executable(perf EXCLUDE_FROM_ALL perf.cpp)
add_executable(screen EXCLUDE_FROM_ALL screen.cpp)
