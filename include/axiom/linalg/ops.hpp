#ifndef AXIOM_OPS_HPP
#define AXIOM_OPS_HPP

#include "axiom/core/core.hpp"
#include "axiom/core/assert.hpp"
#include "axiom/linalg/vec.hpp"
#include "axiom/linalg/mat.hpp"

namespace axiom::linalg {
/*
 * vector ops:
 * Completed:
 * - dot product, isOrthogonal
 * -operator overloading addition / subtraction
 * -scalar multiplication / division
 * -in place ops +=, -=, *=, /=
 * -unary negation -v
 * -vector norms (L1, L2, infty)
 * -length / lengthSquared
 *
 * TODO:
 * -vector projection
 * -normalize / normalized
 * -cross product (3 dim, 7 dim?)
 * -isApprox(v, w, eps) / isApprox(v, w, eps)
 * -distance(a,b) / distanceSquared(a,b)
 * -clampLength(maxLen) / SetLength(len) vs clampedLength
 * -reflect (v,n) off a normal vector
 * -lerp (a,b,t)
 * - Component-wise min/max: min(a,b), max(a,b)
 * - abs(), clamp(), floor/ceil()
 * -sum(), minCoeff(), maxCoeff(), argMin/argMax
 */

    template <typename T>
    T dot(const Vec<T>& a, const Vec<T>& b) {
        std::size_t n = a.size();
        if (b.size() != n) {
            throw core::Error(core::ErrorCode::kShapeMismatch, "dot(): vectors must be of same size");
        }
        T sum{};
        for (std::size_t i = 0; i < n; ++i) sum += a[i] * b[i];
        return sum;
    }

    template <typename T>
    bool is_orthogonal(const Vec<T>& a, const Vec<T>& b) {
        return dot(a, b) == 0;
    }

    template <typename T>
    [[nodiscard]] double norm(const Vec<T>& v, const std::size_t order = 1) {
        // 0 is infinity norm, 1 is L1 norm, 2 is L2 norm
        switch (order) {
            case 0: return static_cast<double>(v.infty_norm());
            case 1: return static_cast<double>(v.l1_norm());
            case 2: return static_cast<double>(v.l2_norm());
            default:
                throw core::Error(core::ErrorCode::kInvalidArgument,
                "norm(vec, order): order must be between 0, 1, or 2");
        }
    }

    template <typename T>
    double len(const Vec<T>& v) {
        return norm(v,2);
    }

    template <typename T>
    double len_squared(const Vec<T>& v) {
        long double sum = 0.0L;
        for (const auto& x : v) {
            const auto xx = static_cast<long double>(x);
            sum += xx * xx;
        }
        return sum;
    }


}
#endif //AXIOM_OPS_HPP