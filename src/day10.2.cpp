#include <iostream>
#include <string>
#include <variant>
#include <vector>

struct None {};

struct Add {
    int x;
};

using Op = std::variant<None, Add>;

constexpr auto signal(const std::vector<Op>& ops) -> std::vector<int> {
    std::vector<int> sig{1};
    for (const auto& op : ops)
        if (std::holds_alternative<None>(op))
            sig.emplace_back(sig.back());
        else {
            const auto& add = std::get<Add>(op);
            sig.emplace_back(sig.back());
            sig.emplace_back(sig.back() + add.x);
        }
    return sig;
}

constexpr auto _abs(int x) -> int { return x >= 0 ? x : -x; }

constexpr auto render(const std::vector<int>& sig) -> std::vector<std::string> {
    std::vector<std::string> screen(6, std::string(40, '.'));
    for (int r = 0; r < 6; ++r)
        for (int c = 0; c < 40; ++c) {
            int i = 40 * r + c;
            if (_abs(sig.at(i) - c) <= 1)
                screen.at(r).at(c) = '#';
        }
    return screen;
}

namespace tests {
    static_assert(signal({None{}, Add{3}, Add{-5}}) == std::vector{1, 1, 1, 4, 4, -1});
    static_assert(_abs(-3) == 3);
}

auto main() -> int {
    std::vector<Op> ops;
    {
        std::string line;
        while (std::getline(std::cin, line))
            if (line == "noop")
                ops.emplace_back(None{});
            else
                ops.emplace_back(Add{std::stoi(line.substr(5))});
    }
    auto screen = render(signal(ops));
    for (const auto& row : screen)
        std::cout << row << '\n';
}