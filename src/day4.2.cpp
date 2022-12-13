#include <array>
#include <cstdio>
#include <iostream>
#include <numeric>
#include <vector>

constexpr auto overlap(const std::array<std::array<int, 2>, 2>& elf_pair) -> bool {
    const auto& [e1, e2] = elf_pair;
    return e1[1] >= e2[0] && e1[0] <= e2[1];
}

constexpr auto total_overlap(const std::vector<std::array<std::array<int, 2>, 2>>& elf_pairs) -> int {
    return std::transform_reduce(
        elf_pairs.begin(), elf_pairs.end(), 0, std::plus(),
        [](const std::array<std::array<int, 2>, 2>& elf_pair) { return overlap(elf_pair); }
    );
}

constexpr auto tests() -> void {
    static_assert(!overlap({{{{2, 4}}, {{6, 8}}}}));
    static_assert(overlap({{{{5, 7}}, {{7, 9}}}}));
}

auto main() -> int {
    std::vector<std::array<std::array<int, 2>, 2>> elf_pairs;
    {
        std::array<int, 2> e1, e2;
        while (scanf("%d-%d,%d-%d", &e1[0], &e1[1], &e2[0], &e2[1]) == 4)
            elf_pairs.push_back({e1, e2});
    }
    std::cout << total_overlap(elf_pairs) << '\n';
}
