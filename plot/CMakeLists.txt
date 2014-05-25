add_subdirectory("mc_movie")
add_subdirectory("roofline")
add_subdirectory("micro_bench_v0")
add_custom_target(
    matrix_plot
    COMMAND python3 ${SRC}/../plot/matrix_plot.py
    )
