#include <gsl/assert>

#include <algorithm>
#include <iostream>
#include <string>

constexpr auto is_marker(std::string s) -> bool {
    std::ranges::sort(s);
    return std::ranges::unique(s).begin() == s.end();
}

constexpr auto first_marker(std::string_view buffer, int marker_length) -> int {
    for (int i = marker_length; i <= buffer.size(); ++i)
        if (is_marker(std::string(buffer.substr(i - marker_length, marker_length))))
            return i;
    Ensures(false);
}

namespace tests {

static_assert(!is_marker("mjqjpqmgbljsph"));
static_assert(is_marker("qmgbljsphdztnv"));
static_assert(first_marker("mjqjpqmgbljsphdztnvjfqwrcgsmlb", 14) == 19);

} // namespace tests

auto main() -> int {
    std::string buffer;
    std::getline(std::cin, buffer);
    std::cout << first_marker(buffer, 14) << '\n';
}
