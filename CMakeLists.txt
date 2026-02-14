cmake_minimum_required(VERSION 4.0)
project(Axiom)

set(CMAKE_CXX_STANDARD 20)

add_executable(Axiom main.cpp include/axiom/core/core.hpp
        include/axiom/linalg/vec.hpp
        include/axiom/linalg/mat.hpp
        include/axiom/linalg/ops.hpp
        include/axiom/linalg/decomposition.hpp
        include/axiom/opt/gd.hpp
        include/axiom/opt/linsearch.hpp
        include/axiom/core/assert.hpp
        include/axiom/io/print.hpp)
