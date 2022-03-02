#ifndef __TREASURE_H
#define __TREASURE_H

#include <concepts>

template <std::integral ValueType, bool trapped>
class Treasure {
private:
    ValueType value;

public:
    constexpr static bool isTrapped = trapped;
    constexpr ValueType evaluate() const { return value; }
    constexpr ValueType getLoot() {
        ValueType res = value;
        value = 0;
        return res;
    }
    constexpr Treasure(ValueType value)
    : value(value) {}
    constexpr Treasure(const Treasure& t) = default;
};

template <class ValueType>
using SafeTreasure = Treasure<ValueType, false>;

template <class ValueType>
using TrappedTreasure = Treasure<ValueType, true>;

#endif
