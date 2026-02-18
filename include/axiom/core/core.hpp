#ifndef AXIOM_CORE_HPP
#define AXIOM_CORE_HPP

#include <limits>
#include <string>

namespace axiom::core {

    enum class ErrorCode { kInvalidArgument, kShapeMismatch, kOutOfBounds, kDivideByZero };

    class Error final : public std::runtime_error {
    public:
        Error(const ErrorCode code, const std::string& msg)
            : std::runtime_error("axiom: " + msg), code_(code) {}

        [[nodiscard]] ErrorCode code() const noexcept { return code_; }

    private:
        ErrorCode code_;
    };

    using real = double;
    using index = std::size_t;

    template <typename T>
    constexpr T sq(T x) {
        return x * x;
    }

    template <typename T>
    constexpr T clamp(T lo, T hi, T v) {
        return std::max(lo, std::min(hi, v));
    }

    template <typename T>
    constexpr bool nearly_equal(T a, T b, T epsilon = std::numeric_limits<T>::epsilon()) {
        return std::abs(a - b) < epsilon;
    }

}

#endif //AXIOM_CORE_HPP