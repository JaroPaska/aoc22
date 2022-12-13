#include <gsl/narrow>

#include <algorithm>
#include <array>
#include <iostream>
#include <numeric>
#include <ranges>
#include <string>
#include <vector>

constexpr auto priority(char item) -> int {
    if (item >= 'a')
        return item + 1 - 'a';
    return item + 27 - 'A';
}

constexpr auto chars = 128;

constexpr auto bitmask(std::string_view rucksack) -> std::array<bool, chars> {
    std::array<bool, chars> mask{};
    std::ranges::for_each(rucksack, [&mask](char c) { mask.at(c) = true; });
    return mask;
}

constexpr auto intersect(const std::vector<std::array<bool, chars>>& masks) -> std::array<bool, chars> {
    std::array<bool, chars> res;
    std::ranges::fill(res, true);
    std::ranges::for_each(masks, [&](const std::array<bool, chars>& mask) {
        std::ranges::for_each(std::views::iota(0, chars), [&](int i) { res.at(i) = std::min(res.at(i), mask.at(i)); });
    });
    return res;
}

constexpr auto badge(const std::vector<std::string>& group) -> char {
    std::vector<std::array<bool, chars>> masks;
    std::ranges::transform(group, std::back_inserter(masks), [](std::string_view rucksack) {
        return bitmask(rucksack);
    });
    auto mask = intersect(masks);
    return gsl::narrow_cast<char>(std::ranges::distance(mask.begin(), std::ranges::find(mask, true)));
}

constexpr auto sum_priorities(const std::vector<std::vector<std::string>>& groups) -> int {
    return std::transform_reduce(
        groups.begin(), groups.end(), 0, std::plus(),
        [](const std::vector<std::string>& group) -> int { return priority(badge(group)); }
    );
}

constexpr auto tests() -> void {
    static_assert(priority('p') == 16);
    static_assert(priority('L') == 38);
    static_assert(badge({"vJrwpWtwJgWrhcsFMMfFFhFp", "jqHRNqRjqzjGDLGLrsFMfFZSrLrFZsSL", "PmmdzqPrVvPwwTWBwg"}) == 'r');
}

auto main() -> int {
    std::vector<std::vector<std::string>> groups;
    {
        std::vector<std::string> group(3);
        while (std::getline(std::cin, group[0])) {
            for (int i = 1; i < 3; ++i)
                std::getline(std::cin, group[i]);
            groups.emplace_back(group);
        }
    }
    std::cout << sum_priorities(groups) << '\n';
}
