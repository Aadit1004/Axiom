#ifndef AXIOM_VEC_HPP
#define AXIOM_VEC_HPP

#include <vector>
#include <algorithm>
#include <utility>
#include <cmath>
#include "axiom/core/core.hpp"

namespace axiom::linalg {
    template <typename T>
    class Vec {
        // vector data is represented by nx1
        std::vector<T> data_;

        static void validate_dims(const core::index n) {
            if (n == 0) throw core::Error(core::ErrorCode::kInvalidArgument,
               "Vec(n): vector size must be >= 1");
        }

        static std::vector<T> make_vec(const core::index n, T value = T{}) {
            validate_dims(n);
            return std::vector<T>(n, value);
        }

        void check_idx_out_of_range(const core::index idx) const {
            if (idx >= size()) {
                throw core::Error(core::ErrorCode::kOutOfBounds, "Vec::at: index out of bounds");
            }
        }

        static std::vector<T> check_data(std::vector<T>&& data) {
            if (data.empty()) {
                throw core::Error(core::ErrorCode::kInvalidArgument,
                                  "Vec(data): data must be non-empty");
            }
            return std::move(data);
        }

    public:
        // Constructors
        explicit Vec(std::vector<T>&& data) : data_(check_data(std::move(data))) {}
        explicit Vec(const core::index n) : data_(make_vec(n)) {}
        static Vec ones(const core::index n) { return Vec(make_vec(n, T{1})); }
        static Vec zeros(const core::index n) { return Vec(make_vec(n)); }

        // Getters
        [[nodiscard]] std::size_t size() const noexcept { return data_.size(); }
        T* data() noexcept { return data_.data(); }
        const T* data() const noexcept { return data_.data(); }

        // Ops
        void resize(core::index n) { data_.resize(n); }
        void resize(core::index n, T val) { data_.resize(n, val); }
        void fill(const T& val) { std::fill(data_.begin(), data_.end(), val); }

        T l1_norm() const {
            T res{};
            for (auto x : data_) res += std::abs(x);
            return res;
        }

        [[nodiscard]] double l2_norm() const {
            long double sum = 0.0;
            for (const auto x : data_) sum += static_cast<long double>(x) * static_cast<long double>(x);
            return std::sqrt(static_cast<double>(sum));
        }

        T infty_norm() const {
            T res{};
            for (auto x : data_) res = std::max(res, std::abs(x));
            return res;
        }

        // Iterators
        using iterator = std::vector<T>::iterator;
        using const_iterator = std::vector<T>::const_iterator;
        iterator begin() noexcept { return data_.begin(); }
        iterator end() noexcept { return data_.end(); }
        const_iterator begin() const noexcept { return data_.begin(); }
        const_iterator end() const noexcept { return data_.end(); }
        const_iterator cbegin() const noexcept { return data_.cbegin(); }
        const_iterator cend() const noexcept { return data_.cend(); }

        // Overload Operations
        T& operator[](core::index i) noexcept {return data_[i]; }
        const T& operator[](core::index i) const noexcept { return data_[i]; }

        T& at(core::index i) {
            check_idx_out_of_range(i);
            return data_[i];
        }
        const T& at(core::index i) const {
            check_idx_out_of_range(i);
            return data_[i];
        }

        // vector addition
        Vec& operator+=(const Vec& rhs) {
            if (size() != rhs.size()) {
                throw core::Error(core::ErrorCode::kShapeMismatch,
                    "vec operator +: vectors must be of same size");
            }
            for (core::index i = 0; i < size(); ++i) data_[i] += rhs[i];
            return *this;
        }

        friend Vec operator+(Vec lhs, const Vec& rhs) {
            lhs += rhs;
            return lhs;
        }

        // vector subtraction
        Vec& operator-=(const Vec& rhs) {
            if (size() != rhs.size()) {
                throw core::Error(core::ErrorCode::kShapeMismatch,
                    "vec operator -: vectors must be of same size");
            }
            for (core::index i = 0; i < size(); ++i) data_[i] -= rhs[i];
            return *this;
        }

        friend Vec operator-(Vec lhs, const Vec& rhs) {
            lhs -= rhs;
            return lhs;
        }

        // scalar multiplication
        Vec& operator*=(const T& val) {
            for (auto& x : data_) x *= val;
            return *this;
        }

        friend Vec operator*(Vec lhs, const T& val) {
            lhs *= val;
            return lhs;
        }

        friend Vec operator*(const T& val, Vec rhs) {
            rhs *= val;
            return rhs;
        }

        // scalar division
        Vec& operator/=(const T& val) {
            if (val == 0) throw core::Error(core::ErrorCode::kDivideByZero,
                "vec operator /: cannot divide by 0");
            for (auto& x : data_) x /= val;
            return *this;
        }

        friend Vec operator/(Vec lhs, const T& val) {
            lhs /= val;
            return lhs;
        }

        // unary negation for -v
        friend Vec operator-(Vec v) {
            for (auto& x : v.data_) x = -x;
            return v;
        }

    };
}

#endif //AXIOM_VEC_HPP