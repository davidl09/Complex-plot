include_directories(${cmplx-plot-1.0_SOURCE_DIR}/main/include)

set(EXECUTABLE_OUTPUT_PATH ${cmplx-plot-1.0_SOURCE_DIR}/bin)


add_executable(cplot main.cpp toojpeg.cpp)