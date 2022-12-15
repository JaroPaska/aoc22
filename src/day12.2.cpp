#include <gsl/assert>
#include <gsl/narrow>

#include <array>
#include <iostream>
#include <optional>
#include <string>
#include <vector>

constexpr auto find_square(const std::vector<std::string>& grid, char ch) -> std::pair<int, int> {
    for (int r = 0; r < grid.size(); ++r)
        for (int c = 0; c < grid.at(0).size(); ++c)
            if (grid.at(r).at(c) == ch)
                return {r, c};
    Ensures(false);
}

constexpr auto height(char ch) -> int {
    if (ch == 'S')
        return height('a');
    if (ch == 'E')
        return height('z');
    return ch - 'a';
}

constexpr auto fewest_steps(const std::vector<std::string>& grid) -> int {
    int h = gsl::narrow_cast<int>(grid.size());
    int w = gsl::narrow_cast<int>(grid.at(0).size());
    auto [er, ec] = find_square(grid, 'E');
    std::vector<std::vector<std::optional<int>>> dist(h, std::vector<std::optional<int>>(w));
    std::vector<std::pair<int, int>> q;
    for (int r = 0; r < h; ++r)
        for (int c = 0; c < w; ++c)
            if (!height(grid.at(r).at(c))) {
                dist.at(r).at(c) = 0;
                q.emplace_back(r, c);
            }
    while (!q.empty()) {
        auto [r, c] = q.front();
        q.erase(q.begin());

        constexpr std::array<int, 4> dr{-1, 0, 1, 0};
        constexpr std::array<int, 4> dc{0, 1, 0, -1};
        for (int di = 0; di < 4; ++di) {
            int nr = r + dr.at(di);
            int nc = c + dc.at(di);
            if (nr < 0 || nr >= h || nc < 0 || nc >= w)
                continue;
            if (!dist.at(nr).at(nc) && height(grid.at(nr).at(nc)) - height(grid.at(r).at(c)) <= 1) {
                dist.at(nr).at(nc) = dist.at(r).at(c).value() + 1;
                q.emplace_back(nr, nc);
            }
        }
    }
    return dist.at(er).at(ec).value();
}

namespace tests {
    static_assert(
        find_square({"Sabqponm", "abcryxxl", "accszExk", "acctuvwj", "abdefghi"}, 'E') == std::make_pair(2, 5)
    );
    static_assert(height('S') == 0);
    static_assert(height('E') == 25);
    static_assert(height('j') == 9);
    static_assert(fewest_steps({"Sabqponm", "abcryxxl", "accszExk", "acctuvwj", "abdefghi"}) == 29);
}

auto main() -> int {
    std::vector<std::string> grid;
    {
        std::string line;
        while (std::getline(std::cin, line))
            grid.emplace_back(line);
    }
    std::cout << fewest_steps(grid) << '\n';
}
