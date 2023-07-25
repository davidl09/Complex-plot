cmake_minimum_required(VERSION 3.22)
project(cmplx-plot-1.0)

set(CXX_STANDARD 20)

add_compile_options(-g -O0 -pthread -std=c++20 -Wall -Wpedantic)
add_subdirectory(main)