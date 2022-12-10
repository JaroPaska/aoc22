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

constexpr auto sum_signals(const std::vector<int>& sig) -> int {
    int sum = 0;
    for (int i = 20; i <= sig.size(); i += 40)
        sum += i * sig.at(i - 1);
    return sum;
}

constexpr auto tests() -> void {
    static_assert(signal({None{}, Add{3}, Add{-5}}) == std::vector{1, 1, 1, 4, 4, -1});
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
    std::cout << sum_signals(signal(ops)) << '\n';
}