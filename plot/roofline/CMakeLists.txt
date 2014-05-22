add_custom_target(
            roofline_plot
            COMMAND python3 ${SRC}/../plot/roofline/rooflinePlot.py
            )
