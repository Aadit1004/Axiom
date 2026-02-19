#include <catch2/catch_test_macros.hpp>
#include "axiom/core/core.hpp"
#include "catch2/catch_approx.hpp"
#include <type_traits>
#include <limits>

#include "catch2/catch_template_test_macros.hpp"

template <typename T>
static void require_equal(T actual, T expected) {
    if constexpr (std::is_floating_point_v<T>)
        REQUIRE(actual == Catch::Approx(expected));
    else
        REQUIRE(actual == expected);
}

TEMPLATE_TEST_CASE("core::sq squares values", "[core][sq]", int, float, double) {
    using T = TestType;
    require_equal(axiom::core::sq(T{4}), T{16});
    require_equal(axiom::core::sq(T{-3}), T{9});
    require_equal(axiom::core::sq(T{0}), T{0});
}

TEMPLATE_TEST_CASE("core::clamp clamps into range", "[core][clamp]", int, float, double) {
    using T = TestType;
    require_equal(axiom::core::clamp(T{0}, T{5}, T{3}),  T{3});
    require_equal(axiom::core::clamp(T{0}, T{5}, T{-3}), T{0});
    require_equal(axiom::core::clamp(T{0}, T{5}, T{10}), T{5});
}

TEST_CASE("nearly_equal: basic behavior", "[core][nearly_equal]") {
    using std::numeric_limits;

    SECTION("exact equality") {
        REQUIRE(axiom::core::nearly_equal(1.0, 1.0));
    }

    SECTION("within epsilon is true") {
        double eps = numeric_limits<double>::epsilon();
        REQUIRE(axiom::core::nearly_equal(1.0, 1.0 + eps * 0.5));
    }

    SECTION("at epsilon boundary is false") {
        double eps = numeric_limits<double>::epsilon();
        REQUIRE_FALSE(axiom::core::nearly_equal(1.0, 1.0 + eps));
    }

    SECTION("outside epsilon is false") {
        double eps = numeric_limits<double>::epsilon();
        REQUIRE_FALSE(axiom::core::nearly_equal(1.0, 1.0 + eps * 2.0));
    }

    SECTION("custom epsilon") {
        REQUIRE(axiom::core::nearly_equal(1.0, 1.0 + 1e-6, 1e-5));
        REQUIRE_FALSE(axiom::core::nearly_equal(1.0, 1.0 + 1e-6, 1e-7));
    }
}