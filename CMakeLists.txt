cmake_minimum_required(VERSION 3.10)
project(prac2)

set(MAIN_SOURCE src/main.cpp)

include_directories(includes)
add_subdirectory(includes)
add_subdirectory(project_tests)
add_executable(earley_alg ${MAIN_SOURCE})

target_link_libraries(earley_alg Algo)