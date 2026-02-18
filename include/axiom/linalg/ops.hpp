#ifndef AXIOM_OPS_HPP
#define AXIOM_OPS_HPP

#include <limits>
#include <algorithm>
#include <numeric>
#include <concepts>

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
 * -vector projection
 * -isApprox(v, w, eps)
 * -normalize
 * -distance(a,b) / distanceSquared(a,b)
 * -cross product (3 dim)
 * -reflect (v,n) off a normal vector
 * -Component-wise min/max: min(a,b), max(a,b)
 * - abs(), clamp(), floor/ceil()
 * -sum(), minCoeff(), maxCoeff(), argMin/argMax
 *
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
        return core::nearly_equal(dot(a, b), T{});
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

    template <typename T>
    Vec<T> proj(const Vec<T>& u, const Vec<T>& v) {
        // projects u onto v
        T denom = dot(v, v);
        if (denom == T{}) {
            throw core::Error(core::ErrorCode::kDivideByZero,
                              "proj(u,v): cannot project onto zero vector");
        }
        T scalar = dot(u, v) / denom;
        return scalar * v;
    }

    template <typename T>
    bool is_approx(const Vec<T>& v, const Vec<T>&w, T epsilon = std::numeric_limits<T>::epsilon()) {
       const std::size_t n = v.size();
        if (n != w.size()) {
            throw core::Error(core::ErrorCode::kShapeMismatch,
                "is_approx(): vectors should be of same length");
        }
        for (std::size_t i = 0; i < n; ++i) {
            if (!core::nearly_equal(v[i], w[i], epsilon)) {
                return false;
            }
        }
        return true;
    }

    template <typename T>
    Vec<T> normalize(Vec<T> v) {
        double len = len(v);
        if (len == 0.0) throw core::Error(core::ErrorCode::kDivideByZero, "normalize: zero vector");
        return v / static_cast<T>(len);
    }

    template <typename T>
    T distanceSquared(const Vec<T>& a, const Vec<T>& b) {
        const std::size_t n = a.size();
        if (n != b.size()) {
            throw core::Error(core::ErrorCode::kShapeMismatch,
                "distance(): vectors should be of same length");
        }
        T sum{};
        for (std::size_t i = 0; i < n; ++i) {
            T diff = a[i] - b[i];
            sum += (diff * diff);
        }
        return sum;
    }

    template <typename T>
    double distance(const Vec<T>& a, const Vec<T>& b) {
        return std::sqrt(static_cast<double>(distanceSquared(a, b)));
    }

    template <typename T>
    Vec<T> cross(const Vec<T>& u, const Vec<T>& v) {
        if (u.size() != 3 || v.size() != 3) {
            throw core::Error(core::ErrorCode::kShapeMismatch,
                "cross(): vectors must be of size 3");
        }
        Vec<T> out(3);
        out[0] = (u[1] * v[2]) - (u[2] * v[1]);
        out[1] = -((u[0] * v[2]) - (u[2] * v[0]));
        out[2] = (u[0] * v[1]) - (u[1] * v[0]);
        return out;
    }

    template <typename T>
    Vec<T> reflect(const Vec<T>& v, const Vec<T>& n) {
        return v - 2 * dot(v, n) * n;
    }

    // component-wise min and max
    template <typename T, typename Op>
    Vec<T> cwise_binary(Vec<T> a, const Vec<T>& b, Op op) {
            const std::size_t n = a.size();
            if (n != b.size()) {
                throw core::Error(core::ErrorCode::kShapeMismatch,
                                  "cwise_binary(): vectors should be of same length");
            }
            for (std::size_t i = 0; i < n; ++i) a[i] = op(a[i], b[i]);
            return a;
    }

    template <typename T>
    Vec<T> min(Vec<T> a, const Vec<T>& b) {
        return cwise_binary(std::move(a), b, [](const T& x, const T& y) {
            return std::min(x, y);
        });
    }

    template <typename T>
    Vec<T> max(Vec<T> a, const Vec<T>& b) {
        return cwise_binary(std::move(a), b, [](const T& x, const T& y) {
            return std::max(x, y);
        });
    }

    template <typename T>
    Vec<T> abs(Vec<T> v) {
        std::transform(v.begin(), v.end(), v.begin(), [](T x) { return std::abs(x); });
        return v;
    }

    template <std::floating_point T>
    Vec<T> floor(Vec<T> v) {
        std::transform(v.begin(), v.end(), v.begin(), [](T x) { return std::floor(x); });
        return v;
    }

    template <std::floating_point T>
    Vec<T> ceil(Vec<T> v) {
        std::transform(v.begin(), v.end(), v.begin(), [](T x) { return std::ceil(x); });
        return v;
    }

    template <typename T>
    Vec<T> clamp(Vec<T> v, const T& low, const T& high) {
        std::transform(v.begin(), v.end(), v.begin(), [&](T x) {
            return std::max(low, std::min(high, x));
        });
        return v;
    }

    template <typename T>
    T sum(const Vec<T>& v) {
        T sum = std::accumulate(v.begin(), v.end(), T{});
        return sum;
    }

    template <typename T>
    T minCoeff(const Vec<T>& v) {
        T min = std::accumulate(v.begin(), v.end(), std::numeric_limits<T>::max(), [](T x, T y) {
            return std::min(x, y);
        });
        return min;
    }

    template <typename T>
    T maxCoeff(const Vec<T>& v) {
        T max = std::accumulate(v.begin(), v.end(), std::numeric_limits<T>::lowest(), [](T x, T y) {
            return std::max(x, y);
        });
        return max;
    }

    // argmin and argmax returns first min element event if not unique
    template <typename T>
    core::index argMin(const Vec<T>& v) {
        core::index idx = 0;
        T min = std::numeric_limits<T>::max();
        for (std::size_t i = 0; i < v.size(); ++i) {
            if (v[i] < min) {
                min = v[i];
                idx = i;
            }
        }
        return idx;
    }

    template <typename T>
    core::index argMax(const Vec<T>& v) {
        core::index idx = 0;
        T max = std::numeric_limits<T>::lowest();
        for (std::size_t i = 0; i < v.size(); ++i) {
            if (v[i] > max) {
                max = v[i];
                idx = i;
            }
        }
        return idx;
    }


}
#endif //AXIOM_OPS_HPP