add_custom_target(
            validate_all
            COMMAND python3 ${SRC}/../validation/validate_all.py
            )
add_executable(validate EXCLUDE_FROM_ALL validate.cpp)
