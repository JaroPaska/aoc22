#include <iostream>
#include <fstream>
#include <string>
#include <vector>

constexpr auto solve(const std::vector<std::string>& lines) -> int {
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
    if (!ifs) {
        std::cout << "input not found\n";
        return 1;
    }

    std::vector<std::string> lines;
    {
        std::string line;
        while (std::getline(ifs, line))
            lines.emplace_back(line);
    }

    std::cout << solve(lines) << '\n';
}
