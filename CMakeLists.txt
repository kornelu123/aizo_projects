cmake_minimum_required(VERSION 3.24)

project(sort)

set(CMAKE_CXX_STANDARD_17)

add_compile_options(-Wall -Wextra -fstack-protector-all -fsanitize=address)

add_link_options(-fsanitize=address)

include_directories(
  ./inc
)

file(GLOB ALL_SRC
  ./src/main.c
  ./src/sorts.c
)

add_executable(sort ${ALL_SRC})
