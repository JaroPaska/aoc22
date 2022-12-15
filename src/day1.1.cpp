#include <iostream>
#include <numeric>
#include <string>
#include <vector>

constexpr auto most_calories(const std::vector<std::vector<int>>& elves) -> int {
    return std::transform_reduce(
        elves.begin(), elves.end(), 0, [](int a, int b) { return std::max(a, b); },
        [](const std::vector<int>& elf) { return std::reduce(elf.begin(), elf.end()); }
    );
}

namespace tests {
    static_assert(most_calories({{1000, 2000, 3000}, {4000}, {5000, 6000}, {7000, 8000, 9000}, {10000}}) == 24000);
}

auto main() -> int {
    std::vector<std::vector<int>> elves;
    {
        std::string line;
        std::vector<int> elf;
        while (std::getline(std::cin, line)) {
            if (line.empty()) {
                elves.emplace_back(elf);
                elf.clear();
            } else
                elf.emplace_back(std::stoi(line));
        }
    }
    std::cout << most_calories(elves) << '\n';
}
