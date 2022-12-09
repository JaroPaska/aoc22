#include <array>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

constexpr auto all_visible(const std::vector<std::string>& grid) -> int {
    int max = 0;
    for (int i = 0; i < grid.size(); ++i) {
        for (int j = 0; j < grid.at(0).size(); ++j) {
            std::array<int, 4> dy{-1, 0, 1, 0};
            std::array<int, 4> dx{0, 1, 0, -1};
            int p = 1;
            for (int k = 0; k < 4; ++k) {
                int s = 0;
                int y = i + dy[k];
                int x = j + dx[k];
                while (y >= 0 && y < grid.size() && x >= 0 && x < grid.at(0).size()) {
                    ++s;
                    if (grid.at(y).at(x) >= grid.at(i).at(j))
                        break;
                    y = y + dy[k];
                    x = x + dx[k];
                }
                p *= s;
            }
            max = std::max(max, p);
        }
    }
    return max;
}

constexpr auto tests()  {
}

auto main() -> int {
    std::vector<std::string> grid;
    {
        std::string line;
        while (std::getline(std::cin, line))
            grid.emplace_back(line);
    }
    std::cout << all_visible(grid) << '\n';
}