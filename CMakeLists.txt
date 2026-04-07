cmake_minimum_required(VERSION 3.30)
project(c_cli_lib C)

find_package(PkgConfig REQUIRED)
pkg_check_modules(READLINE REQUIRED readline)

set(CMAKE_C_STANDARD 23)


add_library(c_cli_lib STATIC src/parser.c
        src/unlikely.h
        include/cli/types.h
        include/cli/types.h
        include/cli/cli.h
        include/cli/cli.h
        include/cli/cli.h
        include/cli/types.h
        src/internal.h
        src/internal.c
        src/app.c
        examples/calculator/main.c)

target_link_libraries(c_cli_lib PUBLIC ${READLINE_LIBRARIES})
add_subdirectory(examples)
target_include_directories(c_cli_lib PUBLIC include)
target_include_directories(c_cli_lib PRIVATE src)
