cmake_minimum_required(VERSION 3.10)
project(polygon_operations)
SET(CMAKE_CXX_STANDARD 17)
SET(DCMAKE_BUILD_TYPE Release)
include_directories(/usr/local/include)

add_executable(compute
        src/polygon_operations/compute.cpp
        src/polygon_operations/conversion.hpp
        src/polygon_operations/conversion.cpp
)

install(TARGETS compute DESTINATION $PROJECT_SOURCE_DIR/bin)
