#include <GSL/assert>

#include <iostream>
#include <fstream>
#include <numeric>
#include <vector>

enum class Score : int { lose = 0, draw = 3, win = 6 };

constexpr auto outcome(const std::pair<int, int>& round) -> Score {
    if (round.first == round.second)
        return Score::draw;
    return round.first == (round.second + 1) % 3 ? Score::lose : Score::win;
}

constexpr auto total_score(const std::vector<std::pair<int, int>>& strategy) -> int {
    return std::transform_reduce(strategy.begin(), strategy.end(), 0, std::plus<>(),
                                 [](const std::pair<int, int>& round) -> int {
                                     return round.second + 1 + static_cast<int>(outcome(round));
                                 });
};

auto main() -> int {
    std::ifstream ifs("input");
    Expects(ifs);

    std::vector<std::pair<int, int>> strategy;
    {
        char a = 0, x = 0;
        while (ifs >> a >> x)
            strategy.emplace_back(a - 'A', x - 'X');
    }

    std::cout << total_score(strategy) << '\n';
}
