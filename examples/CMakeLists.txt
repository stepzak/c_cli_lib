include_directories(../include)
add_executable(example_calc calculator/main.c)
target_link_libraries(example_calc c_cli_lib)