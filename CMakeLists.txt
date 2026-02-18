cmake_minimum_required(VERSION 4.0)
project(Axiom LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

add_library(axiom STATIC src/axiom/axiom.cpp)

target_compile_definitions(axiom PUBLIC AXIOM_ENABLE_ASSERTS=1)

# Public include directory for consumers of the library
target_include_directories(axiom PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_sources(axiom PRIVATE
        include/axiom/core/core.hpp
        include/axiom/core/assert.hpp
        include/axiom/io/print.hpp
        include/axiom/linalg/vec.hpp
        include/axiom/linalg/mat.hpp
        include/axiom/linalg/ops.hpp
        include/axiom/linalg/decomposition.hpp
        include/axiom/opt/gd.hpp
        include/axiom/opt/linsearch.hpp
)

add_executable(Axiom main.cpp)
target_link_libraries(Axiom PRIVATE axiom)