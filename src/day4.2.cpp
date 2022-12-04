#include <GSL/assert>

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
        [](const std::array<std::array<int, 2>, 2>& elf_pair) { return overlap(elf_pair); });
};

namespace tests {

static_assert(!overlap({{{{2, 4}}, {{6, 8}}}}));
static_assert(overlap({{{{5, 7}}, {{7, 9}}}}));

} // namespace tests

auto main() -> int {
    FILE* f = nullptr;
    fopen_s(&f, "input", "r");
    Expects(f);

    std::vector<std::array<std::array<int, 2>, 2>> elf_pairs;
    {
        std::array<std::array<int, 2>, 2> a{};
        while (fscanf_s(f, "%d-%d,%d-%d", &a[0][0], &a[0][1], &a[1][0], &a[1][1]) == 4)
            elf_pairs.emplace_back(a);
    }

    std::cout << total_overlap(elf_pairs) << '\n';
}
