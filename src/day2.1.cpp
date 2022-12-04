#include <GSL/assert>

#include <fstream>
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

namespace tests {

static_assert(outcome({1, 0}) == Outcome::lose);
static_assert(total_score({{0, 1}, {1, 0}, {2, 2}}) == 15);

} // namespace tests

auto main() -> int {
    std::ifstream ifs("input");
    Expects(ifs);

    std::vector<std::pair<int, int>> strategy;
    {
        char a, x;
        while (ifs >> a >> x)
            strategy.emplace_back(a - 'A', x - 'X');
    }

    std::cout << total_score(strategy) << '\n';
}
