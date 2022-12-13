#include <gsl/narrow>

#include <algorithm>
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

constexpr auto _isdigit(char c) -> bool { return c >= '0' && c <= '9'; }

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

constexpr auto decoder_key(std::vector<List>&& lists) {
    lists.emplace_back(List{{List{{2}}}});
    lists.emplace_back(List{{List{{6}}}});
    std::ranges::sort(lists);
    auto p1 = std::ranges::distance(std::ranges::begin(lists), std::ranges::find(lists, List{{List{{2}}}}));
    auto p2 = std::ranges::distance(std::ranges::begin(lists), std::ranges::find(lists, List{{List{{6}}}}));
    return (p1 + 1) * (p2 + 1);
}

constexpr auto tests() -> void {
    static_assert(_isdigit('9'));
    static_assert(!_isdigit('a'));
    static_assert(_stoi("123") == 123);
    static_assert(
        match_brackets("[[]][]") == std::vector<std::optional<int>>{3, 2, std::nullopt, std::nullopt, 5, std::nullopt}
    );
    static_assert(parse("[1,1,3,1,1]") == List{{1, 1, 3, 1, 1}});
    static_assert(parse("[[[]]]") == List{{List{{List{{}}}}}});
    static_assert(parse("[1,1,3,1,1]") < parse("[1,1,5,1,1]"));
    static_assert(parse("[[1],[2,3,4]]") < parse("[[1],4]"));
    static_assert(parse("[9]") > parse("[[8,7,6]]"));
}

auto main() -> int {
    std::vector<List> lists;
    {
        std::string line;
        while (std::getline(std::cin, line)) {
            if (line.empty())
                continue;
            lists.push_back(parse(line));
        }
    }
    std::cout << decoder_key(std::move(lists)) << '\n';
}