#include <algorithm>
#include <array>
#include <iostream>
#include <sstream>
#include <string>
#include <variant>
#include <vector>

struct Add {
    long long y;

    constexpr auto operator()(long long x) -> long long { return x + y; }
};

struct Mul {
    long long y;

    constexpr auto operator()(long long x) -> long long { return x * y; }
};

struct Sq {
    constexpr auto operator()(long long x) -> long long { return x * x; }
};

struct Monkey {
    std::vector<long long> items;
    std::variant<Add, Mul, Sq> operation;
    long long modulo;
    std::array<int, 2> throw_to;
    long long inspected = 0;

    constexpr auto inspect(long long x) -> long long {
        ++inspected;
        return std::visit([&](auto&& op) { return op(x); }, operation);
    }
};

constexpr auto mod_product(const std::vector<Monkey>& monkeys) -> long long {
    long long p = 1;
    for (const auto& monkey : monkeys)
        p *= monkey.modulo;
    return p;
}

constexpr auto turn(std::vector<Monkey>&& monkeys, int i) -> std::vector<Monkey> {
    auto& monkey = monkeys.at(i);
    for (long long item : monkey.items) {
        item = monkey.inspect(item) % mod_product(monkeys);
        monkeys.at(monkey.throw_to.at(bool(item % monkey.modulo))).items.push_back(item);
    }
    monkey.items.clear();
    return monkeys;
}

constexpr auto round(std::vector<Monkey>&& monkeys) -> std::vector<Monkey> {
    for (int i = 0; i < monkeys.size(); ++i)
        monkeys = turn(std::move(monkeys), i);
    return monkeys;
}

constexpr auto monkey_business(std::vector<Monkey>&& monkeys, int rounds) -> long long {
    for (int i = 0; i < rounds; ++i)
        monkeys = round(std::move(monkeys));
    std::ranges::partial_sort(monkeys, monkeys.begin() + 2, [](const Monkey& a, const Monkey& b) {
        return a.inspected > b.inspected;
    });
    return monkeys.at(0).inspected * monkeys.at(1).inspected;
}

constexpr auto tests() -> void {
    static_assert(
        mod_product(
            {{{79, 98}, Mul{19}, 23, {2, 3}},
             {{54, 65, 75, 74}, Add{6}, 19, {2, 0}},
             {{79, 60, 97}, Sq{}, 13, {1, 3}},
             {{74}, Add{3}, 17, {0, 1}}}
        ) == 96577
    );
    static_assert(
        monkey_business(
            {{{79, 98}, Mul{19}, 23, {2, 3}},
             {{54, 65, 75, 74}, Add{6}, 19, {2, 0}},
             {{79, 60, 97}, Sq{}, 13, {1, 3}},
             {{74}, Add{3}, 17, {0, 1}}},
            20
        ) == 10197
    );
}

auto main() -> int {
    std::vector<Monkey> monkeys;
    {
        std::string line;
        while (std::getline(std::cin, line)) {
            Monkey monkey;
            std::string s;
            int n;

            std::getline(std::cin, line);
            std::stringstream ss(line);
            while (ss >> s) {
                if (std::stringstream(s) >> n)
                    monkey.items.push_back(n);
            }

            std::getline(std::cin, line);
            if (sscanf(line.data(), "  Operation: new = old + %d", &n) == 1)
                monkey.operation = Add{n};
            else if (sscanf(line.data(), "  Operation: new = old * %d", &n) == 1)
                monkey.operation = Mul{n};
            else
                monkey.operation = Sq{};

            std::getline(std::cin, line);
            sscanf(line.data(), "  Test: divisible by %d", &n);
            monkey.modulo = n;

            std::getline(std::cin, line);
            sscanf(line.data(), "    If true: throw to monkey %d", &n);
            monkey.throw_to[0] = n;

            std::getline(std::cin, line);
            sscanf(line.data(), "    If false: throw to monkey %d", &n);
            monkey.throw_to[1] = n;

            std::getline(std::cin, line);
            monkeys.push_back(monkey);
        }
    }
    std::cout << monkey_business(std::move(monkeys), 10000) << '\n';
}
