#include <gsl/narrow>

#include <algorithm>
#include <array>
#include <iostream>
#include <span>
#include <vector>

constexpr auto _abs(int x) -> int { return x >= 0 ? x : -x; }

constexpr auto sgn(int x) -> int { return (x > 0) - (x < 0); }

constexpr auto move_tail(const std::pair<int, int>& head, const std::pair<int, int>& tail) -> std::pair<int, int> {
    if (std::max(_abs(head.first - tail.first), _abs(head.second - tail.second)) <= 1)
        return tail;

    auto new_tail = tail;
    new_tail.first += sgn(head.first - tail.first);
    new_tail.second += sgn(head.second - tail.second);
    return new_tail;
}

constexpr auto add(const std::pair<int, int>& a, const std::pair<int, int>& b) -> std::pair<int, int> {
    return {a.first + b.first, a.second + b.second};
}

template<class T>
constexpr auto contains(const std::vector<T>& v, const T& x) -> bool {
    return std::ranges::find(v, x) != std::ranges::end(v);
}

constexpr auto simulate(
    std::span<const int> steps, const std::vector<std::pair<int, int>>& knots,
    std::vector<std::pair<int, int>>&& visited
) -> std::vector<std::pair<int, int>> {
    if (steps.empty())
        return visited;

    constexpr std::array<std::pair<int, int>, 4> dirs{{{-1, 0}, {0, 1}, {1, 0}, {0, -1}}};
    const auto& step = steps.front();
    std::vector<std::pair<int, int>> new_knots(10);
    new_knots.at(0) = add(knots.at(0), dirs.at(step));
    for (int i = 1; i < new_knots.size(); ++i)
        new_knots.at(i) = move_tail(new_knots.at(i - 1), new_knots.at(i));
    if (!contains(visited, new_knots.back()))
        visited.emplace_back(new_knots.back());
    auto other_steps = steps.subspan(1);
    return simulate(other_steps, new_knots, std::move(visited));
}

constexpr auto covered(const std::vector<int>& steps) -> int {
    return gsl::narrow_cast<int>(simulate(std::span{steps}, std::vector<std::pair<int, int>>(10), {{0, 0}}).size());
}

constexpr auto tests() -> void {
    static_assert(sgn(3) == 1);
    static_assert(sgn(0) == 0);
    static_assert(sgn(-1) == -1);
    static_assert(move_tail({1, 2}, {1, 1}) == std::make_pair(1, 1));
    static_assert(move_tail({1, 2}, {2, 1}) == std::make_pair(2, 1));
    static_assert(move_tail({1, 1}, {1, 1}) == std::make_pair(1, 1));
    static_assert(move_tail({1, 3}, {1, 1}) == std::make_pair(1, 2));
    static_assert(move_tail({1, 1}, {3, 1}) == std::make_pair(2, 1));
    static_assert(move_tail({3, 2}, {1, 1}) == std::make_pair(2, 2));
    static_assert(add({1, 1}, {-2, 3}) == std::make_pair(-1, 4));
    static_assert(contains({1, 2, 3}, 3));
    static_assert(!contains({1, 2, 3}, 4));
    static_assert(covered(std::vector<int>(4, 1)) == 1);
}

auto main() -> int {
    std::vector<int> steps;
    {
        static constexpr std::array<char, 4> dirs{'U', 'R', 'D', 'L'};
        char dir;
        int cnt;
        while (std::cin >> dir >> cnt) {
            int di = std::ranges::distance(std::ranges::begin(dirs), std::ranges::find(dirs, dir));
            for (int i = 0; i < cnt; ++i)
                steps.push_back(di);
        }
    }
    std::cout << covered(steps) << '\n';
}