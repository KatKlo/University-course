#ifndef PROJEKT_7_TRI_LIST_H
#define PROJEKT_7_TRI_LIST_H

#include "tri_list_concepts.h"
#include <vector>
#include <functional>
#include <ranges>


template<typename T, typename T1, typename T2, typename T3>
concept tri_list_valid_type =
        (std::same_as<T, T1> || std::same_as<T, T2> || std::same_as<T, T3>)
        && !(std::same_as<T, T1> && std::same_as<T, T2>)
        && !(std::same_as<T, T1> && std::same_as<T, T3>)
        && !(std::same_as<T, T2> && std::same_as<T, T3>);

template<typename T>
auto identity = [](T x) { return x; };

template<typename T>
std::function<T(T)> compose(modifier<T> auto f, modifier<T> auto g) {
    return [f_ = std::move(f), g_ = std::move(g)](T x) mutable { return f_(g_(x)); };
}

template<typename T1, typename T2, typename T3>
class tri_list {
private:
    using value_t = std::variant<T1, T2, T3>;
    using modifier_t = std::function<value_t(value_t)>;

    std::vector<value_t> elements{};
    modifier_t T1_modifier{identity<value_t>};
    modifier_t T2_modifier{identity<value_t>};
    modifier_t T3_modifier{identity<value_t>};

    const modifier_t all_modifier = [&](value_t x) {
        if (x.index() == 0)
            return T1_modifier(x);
        else if (x.index() == 1)
            return T2_modifier(x);
        else
            return T3_modifier(x);
    };

    using view_t = decltype(elements | std::views::transform(all_modifier));
    const view_t iterator_view = elements | std::views::transform(all_modifier);

    template<tri_list_valid_type<T1, T2, T3> T>
    constexpr modifier_t &get_right_modifier() {
        if constexpr(std::same_as<T, T1>)
            return T1_modifier;
        else if constexpr(std::same_as<T, T2>)
            return T2_modifier;
        else
            return T3_modifier;
    }

public:
    constexpr tri_list() = default;
    constexpr tri_list(std::initializer_list<value_t> t) : elements{t} {}

    template<tri_list_valid_type<T1, T2, T3> T>
    void push_back(const T &t) {
        elements.emplace_back(t);
    }

    template<tri_list_valid_type<T1, T2, T3> T, modifier<T> F>
    void modify_only(F m = F{}) {
        get_right_modifier<T>() = compose<value_t>(
                [&](value_t x) { return value_t(m(std::get<T>(x))); },
                std::move(get_right_modifier<T>())
        );
    }

    template<tri_list_valid_type<T1, T2, T3> T>
    void reset() {
        get_right_modifier<T>() = identity<value_t>;
    }

    template<tri_list_valid_type<T1, T2, T3> T>
    auto range_over() const {
        return iterator_view
               | std::views::filter([](value_t x) { return std::holds_alternative<T>(x); })
               | std::views::transform([](value_t x) { return std::get<T>(x); });
    }

    auto begin() const {
        return iterator_view.begin();
    }

    auto end() const {
        return iterator_view.end();
    }

};

#endif //PROJEKT_7_TRI_LIST_H
