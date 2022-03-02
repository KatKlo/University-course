#ifndef __TREASURE_HUNT_H
#define __TREASURE_HUNT_H

#include "member.h"
#include <concepts>

namespace {
template <typename T>
concept TreasureType = requires(T x) {
    { Treasure{x} } -> std::same_as<T>;
};

template <typename V, bool B>
Treasure<V, B> getTreasure() {
    return Treasure<V, B>{V{}};
}

template <typename T>
concept MemberType = requires(T x) {
    typename T::strength_t;
    { [] () constexpr { return T::isArmed; }() } -> std::convertible_to<bool>;
    { x.pay() } -> TreasureValueType;
    x.loot(getTreasure<decltype(x.pay()), false>());
    x.loot(getTreasure<decltype(x.pay()), true>());
};

template <MemberType A, MemberType B>
constexpr void transfer(A& a, B& b) {
    Treasure t = SafeTreasure{a.pay()};
    b.loot(std::move(t));
}
} // namespace

template <typename T>
concept EncounterSide = TreasureType<T> || MemberType<T>;

template <EncounterSide sideA, EncounterSide sideB>
using Encounter = std::pair<sideA&, sideB&>;

template <MemberType A, MemberType B>
constexpr void run(Encounter<A, B> encounter) {
    const auto& [a, b] = encounter;
    if constexpr(a.isArmed && b.isArmed) {
        if (a.getStrength() < b.getStrength())
            transfer(a, b);
        else if (a.getStrength() > b.getStrength())
            transfer(b, a);
    } else if constexpr(a.isArmed) {
        transfer(b, a);
    } else if constexpr(b.isArmed) {
        transfer(a, b);
    }
}

template <MemberType A, TreasureType B>
constexpr void run(Encounter<A, B> encounter) {
    encounter.first.loot(std::move(encounter.second));
}

template <TreasureType A, MemberType B>
constexpr void run(Encounter<A, B> encounter) {
    run(Encounter(encounter.second, encounter.first));
}

constexpr void expedition() {}
template <typename A, typename B, typename... T>
constexpr void expedition(Encounter<A, B> encounter, const T... args) {
    run(encounter);
    expedition(args...);
}

#endif
