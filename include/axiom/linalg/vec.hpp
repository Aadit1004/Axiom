#ifndef AXIOM_VEC_HPP
#define AXIOM_VEC_HPP

#include <vector>
#include <algorithm>
#include <utility>
#include "axiom/core/core.hpp"

namespace axiom::linalg {
    template <typename T>
    class Vec {
        std::vector<T> data_;

    public:
        // Constructors
        explicit Vec(std::vector<T>&& data) : data_(std::move(data)) {}
        explicit Vec(core::index n) : data_(n) {}
        static Vec ones(core::index n) { return Vec(std::vector<T>(n, T{1})); }
        static Vec zeros(core::index n) { return Vec(std::vector<T>(n, T{})); }

        // Getters
        [[nodiscard]] std::size_t size() const noexcept { return data_.size(); }
        T* data() noexcept { return data_.data(); }
        const T* data() const noexcept { return data_.data(); }

        // Overload Operations
        T& operator[](core::index i) noexcept {return data_[i]; }
        const T& operator[](core::index i) const noexcept { return data_[i]; }

        // Ops
        void resize(core::index n) { data_.resize(n); }
        void resize(core::index n, T val) { data_.resize(n, val); }
        void fill(const T& val) { std::fill(data_.begin(), data_.end(), val); }

        // Iterators
        using iterator = std::vector<T>::iterator;
        using const_iterator = std::vector<T>::const_iterator;
        iterator begin() noexcept { return data_.begin(); }
        iterator end() noexcept { return data_.end(); }
        const_iterator begin() const noexcept { return data_.begin(); }
        const_iterator end() const noexcept { return data_.end(); }
        const_iterator cbegin() const noexcept { return data_.cbegin(); }
        const_iterator cend() const noexcept { return data_.cend(); }

    };
}

#endif //AXIOM_VEC_HPP