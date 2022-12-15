#include <algorithm>
#include <iostream>
#include <ranges>
#include <string>

constexpr auto is_marker(std::string s) -> bool {
    std::ranges::sort(s);
    return std::ranges::unique(s).begin() == s.end();
}

constexpr auto first_marker(const std::string& buffer, int marker_length) -> int {
    // clang-format off
    return (std::views::iota(marker_length)
          | std::views::transform([&](int i) { return std::make_pair(i, buffer.substr(i - marker_length, marker_length)); })
          | std::views::filter([&](const std::pair<int, std::string_view>& p) { return is_marker(std::string(p.second)); })
          | std::views::take(1)).front().first;
    // clang-format on
}

namespace tests {
    static_assert(!is_marker("mjqjpqmgbljsph"));
    static_assert(is_marker("qmgbljsphdztnv"));
    static_assert(first_marker("mjqjpqmgbljsphdztnvjfqwrcgsmlb", 14) == 19);
}

auto main() -> int {
    std::string buffer;
    std::getline(std::cin, buffer);
    std::cout << first_marker(buffer, 14) << '\n';
}
