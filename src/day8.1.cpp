#include <gsl/narrow>

#include <array>
#include <iostream>
#include <string>
#include <vector>

constexpr auto smaller_trees(const std::vector<std::string>& grid, int r, int c, int di) -> int {
    constexpr std::array<int, 4> dr{-1, 0, 1, 0};
    constexpr std::array<int, 4> dc{0, 1, 0, -1};
    int nr = r + dr.at(di);
    int nc = c + dc.at(di);
    int cnt = 0;
    while (nr >= 0 && nr < grid.size() && nc >= 0 && nc < grid.at(0).size() && grid.at(nr).at(nc) < grid.at(r).at(c)) {
        nr += dr.at(di);
        nc += dc.at(di);
        ++cnt;
    }
    return cnt;
}

constexpr auto trees(const std::vector<std::string>& grid, int r, int c) -> std::array<int, 4> {
    return {r, gsl::narrow_cast<int>(grid.at(0).size()) - 1 - c, gsl::narrow_cast<int>(grid.size()) - 1 - r, c};
}

constexpr auto visible(const std::vector<std::string>& grid) -> int {
    auto is_visible = [&](int r, int c) {
        auto t = trees(grid, r, c);
        for (int di = 0; di < 4; ++di)
            if (smaller_trees(grid, r, c, di) == t.at(di))
                return true;
        return false;
    };

    int s = 0;
    for (int r = 0; r < grid.size(); ++r)
        for (int c = 0; c < grid.at(0).size(); ++c)
            s += is_visible(r, c);
    return s;
}

namespace tests {
    static_assert(smaller_trees({"30373", "25512", "65332", "33549", "35390"}, 2, 1, 1) == 3);
    static_assert(trees({"30373", "25512", "65332", "33549", "35390"}, 2, 1) == std::array{2, 3, 2, 1});
    static_assert(visible({"30373", "25512", "65332", "33549", "35390"}) == 21);
}

auto main() -> int {
    std::vector<std::string> grid;
    {
        std::string line;
        while (std::getline(std::cin, line))
            grid.emplace_back(line);
    }
    std::cout << visible(grid) << '\n';
}