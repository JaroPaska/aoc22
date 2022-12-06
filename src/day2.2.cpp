#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>

enum class Outcome : int { lose = 0, draw = 3, win = 6 };

constexpr auto outcome(const std::pair<int, int>& round) -> Outcome {
    if (round.first == round.second)
        return Outcome::draw;
    return round.first == (round.second + 1) % 3 ? Outcome::lose : Outcome::win;
}

constexpr auto total_score(const std::vector<std::pair<int, int>>& strategy) -> int {
    return std::transform_reduce(
        strategy.begin(), strategy.end(), 0, std::plus(),
        [](const std::pair<int, int>& round) { return round.second + 1 + static_cast<int>(outcome(round)); });
};

constexpr auto deduce(const std::vector<std::pair<int, int>>& strategy) -> std::vector<std::pair<int, int>> {
    std::vector<std::pair<int, int>> v;
    std::ranges::transform(strategy, std::back_inserter(v), [](const std::pair<int, int>& round) {
        return std::make_pair(round.first, (round.first + round.second + 2) % 3);
    });
    return v;
}

namespace tests {

static_assert(outcome({1, 0}) == Outcome::lose);
static_assert(total_score({{0, 1}, {1, 0}, {2, 2}}) == 15);
static_assert(deduce({{0, 1}, {1, 0}, {2, 2}}) == std::vector<std::pair<int, int>>{{0, 0}, {1, 0}, {2, 0}});

} // namespace tests

auto main() -> int {
    std::vector<std::pair<int, int>> strategy;
    {
        char a, x;
        while (std::cin >> a >> x)
            strategy.emplace_back(a - 'A', x - 'X');
    }
    std::cout << total_score(deduce(strategy)) << '\n';
}
