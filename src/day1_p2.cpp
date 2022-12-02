#include <algorithm>
#include <iostream>
#include <fstream>
#include <numeric>
#include <string>
#include <vector>

constexpr auto most_calories(const std::vector<std::string>& lines) -> int {
    int sum = 0;
    std::vector<int> h;
    for (const auto& line : lines) {
        if (line.empty()) {
            h.push_back(sum);
            std::ranges::push_heap(h, std::greater<>());
            if (h.size() > 3) {
                std::ranges::pop_heap(h, std::greater<>());
                h.pop_back();
            }
            sum = 0;
        } else
            sum += std::stoi(line);
    }
    return std::reduce(h.begin(), h.end());
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

    std::cout << most_calories(lines) << '\n';
}
