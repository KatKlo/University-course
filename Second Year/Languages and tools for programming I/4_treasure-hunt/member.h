#ifndef __MEMBER_H
#define __MEMBER_H

#include "treasure.h"
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <array>
#include <tuple>

namespace {
template <typename T>
concept TreasureValueType = requires(T x) {
    typename Treasure<T, false>;
    typename Treasure<T, true>;
};

template <typename ValueType>
constexpr ValueType implementPay(ValueType& ownCount) {
    ValueType res = ownCount;
    ownCount = 0;
    return res;
}
} // namespace

template <TreasureValueType ValueType, bool armed>
class Adventurer {
public:
    using strength_t = uint32_t;
    constexpr static bool isArmed = armed;

    constexpr Adventurer() requires(!armed) {}
    constexpr Adventurer(strength_t strength) requires(armed)
    : strength(strength) {}

    template <bool trapped>
    constexpr void loot(Treasure<ValueType, trapped>&& treasure) {
        if constexpr(trapped) {
            if (strength == 0)
                return;
            strength /= 2;
        }

        ownCount += treasure.getLoot();
    }

    constexpr ValueType pay() { return implementPay(ownCount); }
    constexpr ValueType getStrength() const requires(armed) { return strength; }

private:
    ValueType ownCount = 0;
    strength_t strength = 0;
};

template <typename ValueType>
using Explorer = Adventurer<ValueType, false>;

template <TreasureValueType ValueType, std::size_t CompletedExpeditions>
class Veteran {
public:
    using strength_t = uint32_t;
    constexpr static bool isArmed = true;

    constexpr Veteran() requires(CompletedExpeditions < 25)
    : strength(fib(CompletedExpeditions)) {}

    template <bool trapped>
    constexpr void loot(Treasure<ValueType, trapped>&& treasure) {
        if constexpr (trapped)
            if (strength == 0)
                return;

        ownCount += treasure.getLoot();
    }

    constexpr ValueType pay() { return implementPay(ownCount); }
    constexpr ValueType getStrength() const  { return strength; }

private:
    ValueType ownCount = 0;
    strength_t strength;

    constexpr strength_t fib(std::size_t n) {
        std::array<strength_t, 2> f = {0, 1};
        for(; n >= 2; n--)
            std::tie(f[0], f[1]) = std::tuple {f[1], f[0] + f[1]};
        return f[n];
    }
};

#endif
