cmake_minimum_required(VERSION 3.22)
project(cplot LANGUAGES CXX VERSION 1.1 DESCRIPTION "Plot complex-valued functions to bitmaps")

set(CXX_STANDARD 20)

include_directories({PROJECT_SOURCE_DIR}/include/**)

add_library(cplot SHARED 
    src/libcplot.cpp 
    src/toojpeg.cpp 
)

set_target_properties(cplot PROPERTIES PUBLIC_HEADER libcplot.hpp)

target_include_directories(cplot PRIVATE include)

include(GNUInstallDirs)