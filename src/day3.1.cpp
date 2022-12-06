#include <GSL/assert>

#include <algorithm>
#include <array>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

constexpr auto priority(char item) -> int {
    if (item >= 'a')
        return item + 1 - 'a';
    return item + 27 - 'A';
}

constexpr auto duplicate(std::string_view rucksack) -> char {
    std::string s(rucksack);
    const auto mid = s.begin() + s.size() / 2;
    std::sort(s.begin(), mid);
    std::sort(mid, s.end());
    std::string o;
    std::set_intersection(s.begin(), mid, mid, s.end(), std::back_inserter(o));
    Ensures(!o.empty());
    return o[0];
}

constexpr auto sum_priorities(const std::vector<std::string>& rucksacks) -> int {
    return std::transform_reduce(rucksacks.begin(), rucksacks.end(), 0, std::plus(),
                                 [](std::string_view rucksack) { return priority(duplicate(rucksack)); });
}

namespace tests {

static_assert(priority('p') == 16);
static_assert(duplicate("vJrwpWtwJgWrhcsFMMfFFhFp") == 'p');
static_assert(sum_priorities({"vJrwpWtwJgWrhcsFMMfFFhFp", "jqHRNqRjqzjGDLGLrsFMfFZSrLrFZsSL", "PmmdzqPrVvPwwTWBwg",
                              "wMqvLMZHhHMvwLHjbvcjnnSBnvTQFn", "ttgJtRGJQctTZtZT", "CrZsJsPPZsGzwwsLwLmpwMDw"}) ==
              157);

} // namespace tests

auto main() -> int {
    std::vector<std::string> rucksacks;
    {
        std::string line;
        while (std::getline(std::cin, line))
            rucksacks.emplace_back(line);
    }
    std::cout << sum_priorities(rucksacks) << '\n';
}
