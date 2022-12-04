#include <GSL/assert>

#include <array>
#include <cstdio>
#include <iostream>
#include <numeric>
#include <vector>

constexpr auto contains(const std::array<int, 2>& e1, const std::array<int, 2>& e2) -> bool {
    return e1[0] <= e2[0] && e1[1] >= e2[1];
}

constexpr auto sym_contains(const std::array<std::array<int, 2>, 2>& elf_pair) -> bool {
    const auto& [e1, e2] = elf_pair;
    return contains(e1, e2) || contains(e2, e1);
}

constexpr auto total_contained(const std::vector<std::array<std::array<int, 2>, 2>>& elf_pairs) -> int {
    return std::transform_reduce(
        elf_pairs.begin(), elf_pairs.end(), 0, std::plus(),
        [](const std::array<std::array<int, 2>, 2>& elf_pair) { return sym_contains(elf_pair); });
};

namespace tests {

static_assert(contains({1, 3}, {2, 2}) == true);
static_assert(contains({2, 2}, {1, 3}) == false);
static_assert(sym_contains({{{{2, 2}}, {{1, 3}}}}) == true);

} // namespace tests

auto main() -> int {
    FILE* f;
    fopen_s(&f, "input", "r");
    Expects(f);

    std::vector<std::array<std::array<int, 2>, 2>> elf_pairs;
    {
        std::array<std::array<int, 2>, 2> a{};
        while (fscanf_s(f, "%d-%d,%d-%d", &a[0][0], &a[0][1], &a[1][0], &a[1][1]) == 4)
            elf_pairs.emplace_back(a);
    }

    std::cout << total_contained(elf_pairs) << '\n';
}
