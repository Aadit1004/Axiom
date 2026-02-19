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

# Catch2 linking
Include(FetchContent)
FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG        v3.8.1
)
FetchContent_MakeAvailable(Catch2)

add_executable(AxiomApp main.cpp)
target_link_libraries(AxiomApp PRIVATE axiom)

file(GLOB_RECURSE AXIOM_TEST_SOURCES CONFIGURE_DEPENDS
        ${CMAKE_CURRENT_SOURCE_DIR}/tests/axiom/*.cpp
)
add_executable(AxiomTests ${AXIOM_TEST_SOURCES})
target_link_libraries(AxiomTests PRIVATE axiom Catch2::Catch2WithMain)

include(CTest)
enable_testing()

include(Catch)
catch_discover_tests(AxiomTests)