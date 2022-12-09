#include <iostream>
#include <numeric>
#include <string>
#include <vector>

constexpr auto all_visible(const std::vector<std::string>& grid) -> int {
    std::vector<std::vector<bool>> b(grid.size(), std::vector<bool>(grid.at(0).size()));
    for (int i = 0; i < grid.size(); ++i) {
        int max = -1;
        for (int j = 0; j < grid.at(0).size(); ++j)
            if (grid.at(i).at(j) > max) {
                max = grid.at(i).at(j);
                b.at(i).at(j) = true;
            }

        max = -1;
        for (int j = grid.at(0).size() - 1; j >= 0; --j)
            if (grid.at(i).at(j) > max) {
                max = grid.at(i).at(j);
                b.at(i).at(j) = true;
            }
    }

    for (int i = 0; i < grid.at(0).size(); ++i) {
        int max = -1;
        for (int j = 0; j < grid.size(); ++j)
            if (grid.at(j).at(i) > max) {
                max = grid.at(j).at(i);
                b.at(j).at(i) = true;
            }

        max = -1;
        for (int j = grid.size() - 1; j >= 0; --j)
            if (grid.at(j).at(i) > max) {
                max = grid.at(j).at(i);
                b.at(j).at(i) = true;
            }
    }

    return std::transform_reduce(b.begin(), b.end(), 0, std::plus(), [](const std::vector<bool>& v) {
        return std::reduce(v.begin(), v.end(), 0, std::plus());
    });
}

constexpr auto tests() {
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