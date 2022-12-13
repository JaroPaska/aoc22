#include <gsl/narrow>

#include <array>
#include <compare>
#include <iostream>
#include <optional>
#include <string>
#include <string_view>
#include <variant>
#include <vector>

struct List;
constexpr auto operator<=>(const std::variant<int, List>& l, const std::variant<int, List>& r) -> std::strong_ordering;

struct List {
    std::vector<std::variant<int, List>> elements;
    constexpr auto operator<=>(const List& other) const -> std::strong_ordering = default;
    constexpr auto operator<=>(int n) const -> std::strong_ordering { return *this <=> List{{n}}; }
};

constexpr auto operator<=>(const std::variant<int, List>& l, const std::variant<int, List>& r) -> std::strong_ordering {
    return std::visit([&](auto&& lv) { return std::visit([&](auto&& rv) { return lv <=> rv; }, r); }, l);
}

constexpr auto match_brackets(std::string_view line) -> std::vector<std::optional<int>> {
    std::vector<std::optional<int>> rb(line.size());
    std::vector<int> stack;
    for (int i = 0; i < line.size(); ++i)
        if (line.at(i) == '[')
            stack.push_back(i);
        else if (line.at(i) == ']') {
            rb.at(stack.back()) = i;
            stack.pop_back();
        }
    return rb;
}

constexpr auto _isdigit(char c) -> bool {
    return c >= '0' && c <= '9';
}

constexpr auto _stoi(std::string_view s) -> int {
    int result = 0, sign = 1;
    if (s.at(0) == '-')
        sign = -1;
    for (int i = 0; i < s.size(); i++)
        if (_isdigit(s.at(i)))
            result = result * 10 + (s.at(i) - '0') * sign;
    return result;
}

constexpr auto _parse(std::string_view line, const std::vector<std::optional<int>>& rb, int l, int r) -> List {
    List list;
    ++l;
    while (l < r) {
        if (rb.at(l)) {
            list.elements.emplace_back(_parse(line, rb, l, rb.at(l).value()));
            l = rb.at(l).value() + 2;
        } else {
            int sz = 0;
            while (_isdigit(line.at(l + sz)))
                ++sz;
            list.elements.emplace_back(_stoi(std::string(line.substr(l, sz))));
            l += sz + 1;
        }
    }
    return list;
}

constexpr auto parse(std::string_view line) -> List {
    return _parse(line, match_brackets(line), 0, gsl::narrow_cast<int>(line.size()));
}

constexpr auto sum_indices(const std::vector<std::array<List, 2>>& lists) -> int {
    int s = 0;
    for (int i = 0; i < lists.size(); ++i)
        if (lists.at(i).at(0) <= lists.at(i).at(1))
            s += i + 1;
    return s;
}

constexpr auto tests() -> void {
    static_assert(_isdigit('9'));
    static_assert(!_isdigit('a'));
    static_assert(_stoi("123") == 123);
    static_assert(match_brackets("[[]][]") == std::vector<std::optional<int>>{3, 2, std::nullopt, std::nullopt, 5, std::nullopt});
    static_assert(parse("[1,1,3,1,1]") == List{{1, 1, 3, 1, 1}});
    static_assert(parse("[[[]]]") == List{{List{{List{{}}}}}});
    static_assert(parse("[1,1,3,1,1]") < parse("[1,1,5,1,1]"));
    static_assert(parse("[[1],[2,3,4]]") < parse("[[1],4]"));
    static_assert(parse("[9]") > parse("[[8,7,6]]"));
}

auto main() -> int {
    std::vector<std::array<List, 2>> lists;
    {
        std::string line;
        while (std::getline(std::cin, line)) {
            auto list = parse(line);

            std::getline(std::cin, line);
            lists.push_back({list, parse(line)});

            std::getline(std::cin, line);
        }
    }
    std::cout << sum_indices(lists) << '\n';
}