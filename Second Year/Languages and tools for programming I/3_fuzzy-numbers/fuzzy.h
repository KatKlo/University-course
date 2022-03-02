#ifndef __FUZZY_H__
#define __FUZZY_H__

#include <algorithm>
#include <cmath>
#include <iostream>
#include <set>

using real_t = double;

namespace {
    // Do zaokrąglania małych liczb
    const real_t nearlyZero = 1e-15;
}

class TriFuzzyNum {
  private:
    real_t l;
    real_t m;
    real_t u;

    constexpr std::tuple<real_t, real_t, real_t> rank() const {
        real_t z = (u - l) + std::sqrt(1 + (u - m) * (u - m)) +
                   std::sqrt(1 + (m - l) * (m - l));
        real_t x = ((u - l) * m + sqrt(1 + (u - m) * (u - m)) * l +
                    sqrt(1 + (m - l) * (m - l)) * u) /
                   z;
        real_t y = (u - l) / z;
        return {x - y / 2, 1 - y, m};
    }

  public:
    constexpr TriFuzzyNum(real_t a, real_t b, real_t c) {
        l = std::min({a, b, c});
        u = std::max({a, b, c});
        m = a + b + c - l - u;
    }
    TriFuzzyNum() = delete;
    TriFuzzyNum(const TriFuzzyNum &) = default;
    TriFuzzyNum(TriFuzzyNum &&) noexcept = default;

    constexpr real_t lower_value() const { return l; }
    constexpr real_t modal_value() const { return m; }
    constexpr real_t upper_value() const { return u; }

    TriFuzzyNum &operator=(const TriFuzzyNum &) = default;
    TriFuzzyNum &operator=(TriFuzzyNum &&) noexcept = default;

    TriFuzzyNum &operator+=(const TriFuzzyNum &rhs) {
        l += rhs.l;
        m += rhs.m;
        u += rhs.u;
        return *this = TriFuzzyNum(l, m, u);
    }
    TriFuzzyNum &operator-=(const TriFuzzyNum &rhs) {
        l -= rhs.u;
        m -= rhs.m;
        u -= rhs.l;
        return *this = TriFuzzyNum(l, m, u);
    }
    TriFuzzyNum &operator*=(const TriFuzzyNum &rhs) {
        l *= rhs.l;
        m *= rhs.m;
        u *= rhs.u;
        return *this = TriFuzzyNum(l, m, u);
    }

    TriFuzzyNum operator+(const TriFuzzyNum &rhs) const {
        return TriFuzzyNum(*this) += rhs;
    }
    TriFuzzyNum operator-(const TriFuzzyNum &rhs) const {
        return TriFuzzyNum(*this) -= rhs;
    }
    TriFuzzyNum operator*(const TriFuzzyNum &rhs) const {
        return TriFuzzyNum(*this) *= rhs;
    }

    inline friend std::ostream &operator<<(std::ostream &o,
                                           const TriFuzzyNum &tri) {
        auto checkZero = [](real_t a) { return std::abs(a) < nearlyZero ? 0 : a; };

        return o << "(" << checkZero(tri.l) << ", " << checkZero(tri.m)
                 << ", " << checkZero(tri.u) << ")";
    }

    std::partial_ordering operator<=>(const TriFuzzyNum &rhs) const {
        return rank() <=> rhs.rank();
    }

    constexpr bool operator==(const TriFuzzyNum &rhs) const {
        return l == rhs.l && m == rhs.m && u == rhs.u;
    }

    constexpr bool operator!=(const TriFuzzyNum &rhs) const {
        return !(*this == rhs);
    }
};

class TriFuzzyNumSet {
  private:
    std::multiset<TriFuzzyNum> elements;
    real_t sum_l, sum_m, sum_u;

  public:
    TriFuzzyNumSet() : elements(), sum_l(0), sum_m(0), sum_u(0) {}
    TriFuzzyNumSet(std::initializer_list<TriFuzzyNum> list) :
    elements(), sum_l(0), sum_m(0), sum_u(0) {
        for (const TriFuzzyNum &e : list) {
            insert(e);
        }
    }

    TriFuzzyNumSet(const TriFuzzyNumSet &) = default;
    TriFuzzyNumSet(TriFuzzyNumSet && other) noexcept :
    elements(std::move(other.elements)),
    sum_l(other.sum_l),
    sum_m(other.sum_m),
    sum_u(other.sum_u) {}

    ~TriFuzzyNumSet() = default;

    TriFuzzyNumSet &operator=(const TriFuzzyNumSet &) = default;
    TriFuzzyNumSet &operator=(TriFuzzyNumSet && other) noexcept {
            elements = std::move(other.elements);
            sum_l = other.sum_l;
            sum_m = other.sum_m;
            sum_u = other.sum_u;

            return *this;
    }

    void insert(const TriFuzzyNum &to_insert) {
        sum_l += to_insert.lower_value();
        sum_m += to_insert.modal_value();
        sum_u += to_insert.upper_value();
        elements.insert(to_insert);
    }

    void insert(TriFuzzyNum &&to_insert) {
        sum_l += to_insert.lower_value();
        sum_m += to_insert.modal_value();
        sum_u += to_insert.upper_value();
        elements.insert(to_insert);
    }

    void remove(const TriFuzzyNum &to_remove) {
        auto it = elements.find(to_remove);

        if (it != elements.end()) {
            sum_l -= to_remove.lower_value();
            sum_m -= to_remove.modal_value();
            sum_u -= to_remove.upper_value();
            elements.erase(it);
        }
    }

    TriFuzzyNum arithmetic_mean() {
        size_t size = elements.size();

        if (size > 0) {
            return TriFuzzyNum(sum_l / size, sum_m / size, sum_u / size);
        } else {
            throw std::length_error(
                "TriFuzzyNumSet::arithmetic_mean - the set is empty.");
        }
    }
};

consteval TriFuzzyNum crisp_number(real_t v) { return TriFuzzyNum(v, v, v); }

constexpr TriFuzzyNum crisp_zero(0, 0, 0);

#endif /* __FUZZY_H__ */