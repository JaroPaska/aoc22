#include <algorithm>
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
};

constexpr auto operator<=>(const List& l, int r) -> std::strong_ordering { return l <=> List{{r}}; }

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

auto parse(std::string_view line, const std::vector<std::optional<int>>& rb, int l, int r) -> List {
    List list;
    ++l;
    while (l < r) {
        if (rb.at(l)) {
            list.elements.emplace_back(parse(line, rb, l, rb.at(l).value()));
            l = rb.at(l).value() + 2;
        } else {
            int sz = 0;
            while (isdigit(line.at(l + sz)))
                ++sz;
            list.elements.emplace_back(std::stoi(std::string(line.substr(l, sz))));
            l += sz + 1;
        }
    }
    return list;
}

constexpr auto decoder_key(std::vector<List>&& lists) -> int {
    std::ranges::sort(lists);
    int p1 = std::ranges::distance(std::ranges::begin(lists), std::ranges::find(lists, List{{List{{2}}}}));
    int p2 = std::ranges::distance(std::ranges::begin(lists), std::ranges::find(lists, List{{List{{6}}}}));
    return p1 * p2;
}

constexpr auto tests() -> void {
    static_assert(List{{1, 1, 3, 1, 1}} < List{{1, 1, 5, 1, 1}});
    static_assert(List{{List{{1}}}} == List{{List{{1}}}});
    static_assert(List{{List{{1}}, List{{2, 3, 4}}}} < List{{List{{1}}, 4}});
    static_assert(List{{9}} > List{{List{{8, 7, 6}}}});
}

auto main() -> int {
    std::vector<List> lists;
    {
        std::string line;
        while (std::getline(std::cin, line)) {
            if (line.empty())
                continue;
            lists.push_back(parse(line, match_brackets(line), 0, line.size() - 1));
        }
    }
    std::cout << decoder_key(std::move(lists)) << '\n';
}