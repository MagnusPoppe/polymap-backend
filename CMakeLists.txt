cmake_minimum_required(VERSION 3.12)
project(polygon_operations)
SET(CMAKE_CXX_STANDARD 17)
SET(DCMAKE_BUILD_TYPE Release)
include_directories(/usr/local/include)

add_executable(UNION
        src/polygon_operations/union.cpp
        src/polygon_operations/structs.hpp
        src/polygon_operations/conversion.hpp
        src/polygon_operations/conversion.cpp
)

add_executable(INTERSECTION
        src/polygon_operations/intersection.cpp
        src/polygon_operations/structs.hpp
        src/polygon_operations/conversion.hpp
        src/polygon_operations/conversion.cpp
)