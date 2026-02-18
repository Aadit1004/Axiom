#ifndef AXIOM_MAT_HPP
#define AXIOM_MAT_HPP

#include <vector>
#include <algorithm>
#include <utility>

#include "axiom/core/assert.hpp"
#include "axiom/core/core.hpp"

namespace axiom::linalg {
    template <typename T>
    class Mat {
        // row-major layout w/ indexing by data_[r * cols + c]
        std::vector<T> data_;
        core::index cols_;

        [[nodiscard]] core::index idx(const core::index row_index, const core::index col_index) const {
            return row_index * cols_ + col_index;
        }

        static void validate_dims(const core::index rows, const core::index cols) {
            if (rows == 0 || cols == 0) throw core::Error(core::ErrorCode::kInvalidArgument,
                "Mat(rows, cols): rows and cols must be >= 1");
        }

        static void validate_square(const core::index n) {
            if (!n) throw core::Error(core::ErrorCode::kInvalidArgument,
                "Mat(n): rows and cols must be >= 1");
        }

        static std::vector<T> check_data(std::vector<T>&& data, core::index cols) {
            if (cols == 0) {
                throw core::Error(core::ErrorCode::kInvalidArgument,
                                  "Mat(data, cols): cols must be >= 1");
            }
            if (data.empty()) {
                throw core::Error(core::ErrorCode::kInvalidArgument,
                                  "Mat(data, cols): data must be non-empty");
            }
            if (data.size() % cols != 0) {
                throw core::Error(core::ErrorCode::kShapeMismatch,
                                  "Mat(data, cols): data.size() must be a multiple of cols");
            }
            return std::move(data);
        }

        static std::vector<T> make_data(const core::index r, const core::index c, T val = T{}) {
            validate_dims(r, c);
            return std::vector<T>(r * c, val);
        }

        void check_idx_out_of_range(const core::index row, const core::index col) const {
            if (idx(row, col) >= size()) {
                throw core::Error(core::ErrorCode::kOutOfBounds, "Mat index out of bounds");
            }
        }

    public:
        // Constructors
        explicit Mat(std::vector<T>&& data, const core::index cols)
            : data_(check_data(std::move(data), cols)), cols_(cols) {}

        explicit Mat(const core::index rows, const core::index cols)
            : data_(make_data(rows, cols)), cols_(cols) {}

        explicit Mat(const core::index n) : Mat(n,n) {}

        static Mat identity(core::index n) {
            validate_square(n);
            std::vector<T> data(n*n);
            for (core::index i = 0; i < n; i++) {
                core::index idx = i * n + i;
                data[idx] = T{1};
            }
            return Mat(std::move(data), n);
        }

        static Mat zeros(const core::index n) {
            return Mat(n);
        }

        static Mat zeros(const core::index rows, const core::index cols) {
            return Mat(rows, cols);
        }

        static Mat ones(const core::index n) {
            return Mat(std::move(make_data(n, n, T{1})), n);
        }

        static Mat ones(const core::index rows, const core::index cols) {
            return Mat(std::move(make_data(rows, cols, T{1})), cols);
        }

        // Getters
        [[nodiscard]] std::size_t size() const noexcept { return data_.size(); }
        T* data() noexcept { return data_.data(); }
        const T* data() const noexcept { return data_.data(); }
        [[nodiscard]] core::index rows() const {
            AXIOM_ASSERT(cols_ != 0, "cols_ must be non-zero");
            return data_.size() / cols_;
        }
        [[nodiscard]] core::index cols() const { return cols_; }

        // Overload operations
        T& operator()(const core::index row, const core::index col) {
            return data_[idx(row, col)];
        }

        const T& operator()(const core::index row, const core::index col) const {
            return data_[idx(row, col)];
        }

        T& at(const core::index row, const core::index col) {
            check_idx_out_of_range(row, col);
            return data_[idx(row, col)];
        }

        const T& at(const core::index row, const core::index col) const {
            check_idx_out_of_range(row, col);
            return data_[idx(row, col)];
        }

        // Ops
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

#endif //AXIOM_MAT_HPP