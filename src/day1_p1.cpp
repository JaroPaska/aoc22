#include <GSL/assert>

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

constexpr auto most_calories(const std::vector<std::string>& lines) -> int {
    int sum = 0;
    int max = 0;
    for (const auto& line : lines) {
        if (line.empty()) {
            max = std::max(max, sum);
            sum = 0;
        } else
            sum += std::stoi(line);
    }
    return max;
};

auto main() -> int {
    std::ifstream ifs("input");
    Expects(ifs);

    std::vector<std::string> lines;
    {
        std::string line;
        while (std::getline(ifs, line))
            lines.emplace_back(line);
    }

    std::cout << most_calories(lines) << '\n';
}
