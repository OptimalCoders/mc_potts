add_custom_target(
            perf_search 
            COMMAND python3 ${SRC}/../performance/performance_search.py
            )
add_executable(perf EXCLUDE_FROM_ALL perf.cpp)
