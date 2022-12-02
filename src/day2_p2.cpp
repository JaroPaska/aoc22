#include <algorithm>
#include <iostream>
#include <fstream>
#include <numeric>
#include <vector>

constexpr auto deduce(const std::vector<std::pair<int, int>> &strategy) -> std::vector<std::pair<int, int>> {
    std::vector<std::pair<int, int>> v;
    std::ranges::transform(strategy, std::back_inserter(v),
                   [](const std::pair<int, int> &round) -> std::pair<int, int> {
                       return {round.first, (round.first + round.second + 2) % 3};
                   });
    return v;
}

enum class Score : int { lose = 0, draw = 3, win = 6 };

constexpr auto outcome(const std::pair<int, int>& round) -> Score {
    if (round.first == round.second)
        return Score::draw;
    return round.first == (round.second + 1) % 3 ? Score::lose : Score::win;
}

constexpr auto total_score(const std::vector<std::pair<int, int>> &strategy) -> int {
    return std::transform_reduce(strategy.begin(), strategy.end(), 0, std::plus<>(),
                                 [](const std::pair<int, int> &round) -> int {
                                     return round.second + 1 + static_cast<int>(outcome(round));
                                 });
};

auto main() -> int {
    std::ifstream ifs("input");
    if (!ifs) {
        std::cout << "input not found\n";
        return 1;
    }

    std::vector<std::pair<int, int>> strategy;
    {
        char a = 0, x = 0;
        while (ifs >> a >> x)
            strategy.emplace_back(a - 'A', x - 'X');
    }

    std::cout << total_score(deduce(strategy)) << '\n';
}
