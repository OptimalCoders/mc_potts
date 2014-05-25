add_custom_target(
            perf_search 
            COMMAND python3 ${SRC}/../performance/performance_search.py
            )
add_custom_target(
            perf_screen 
            COMMAND python3 ${SRC}/../performance/performance_screen.py
            )
add_custom_target(
            compile_all
            COMMAND python3 ${SRC}/../performance/compile_all.py
            )
add_custom_target(
            mem_traffic
            COMMAND python3 ${SRC}/../performance/mem_traffic.py
            )
add_custom_target(
            runtime_opt
            COMMAND python3 ${SRC}/../performance/runtime_opt.py
            )
add_executable(perf EXCLUDE_FROM_ALL perf.cpp)
add_executable(screen EXCLUDE_FROM_ALL screen.cpp)
add_executable(perf_raw EXCLUDE_FROM_ALL perf_raw.cpp)
add_executable(perf_pcm_mem EXCLUDE_FROM_ALL perf_pcm_mem.cpp)
target_link_libraries(perf_pcm_mem MeasuringCore)
add_executable(perf_pcm_mem_opt EXCLUDE_FROM_ALL perf_pcm_mem_opt.cpp)
target_link_libraries(perf_pcm_mem_opt MeasuringCore)
add_executable(perf_pcm_mem_opt_full EXCLUDE_FROM_ALL perf_pcm_mem_opt_full.cpp)
target_link_libraries(perf_pcm_mem_opt_full MeasuringCore)
add_executable(perf_opt EXCLUDE_FROM_ALL perf_opt.cpp)
add_executable(perf_opt_full EXCLUDE_FROM_ALL perf_opt_full.cpp)
