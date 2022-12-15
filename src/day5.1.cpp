#include <gsl/narrow>

#include <algorithm>
#include <array>
#include <iostream>
#include <ranges>
#include <span>
#include <string>
#include <vector>

using Crate = char;
using Stack = std::vector<Crate>;
using Cmd = std::array<int, 3>;

constexpr auto rearrange(std::vector<Stack>&& stacks, const std::vector<Cmd>& cmds) -> std::vector<Stack> {
    for (const auto& cmd : cmds) {
        const auto& [num, from, to] = cmd;
        auto first = stacks[from].end() - num;
        auto last = stacks[from].end();
        std::reverse(first, last);
        stacks[to].insert(stacks[to].end(), first, last);
        stacks[from].erase(first, last);
    }
    return stacks;
}

constexpr auto top_crates(const std::vector<Stack>& stacks) -> std::vector<char> {
    auto top = stacks | std::views::transform([](const Stack& stack) { return stack.back(); });
    return std::vector<char>(top.begin(), top.end());
}

namespace tests {
    static_assert(
        rearrange({{'Z', 'N', 'D'}, {'M', 'C'}, {'P'}}, {{3, 0, 2}}) ==
        std::vector<Stack>{{}, {'M', 'C'}, {'P', 'D', 'N', 'Z'}}
    );
    static_assert(top_crates({{'Z', 'N'}, {'M', 'C', 'D'}, {'P'}}) == std::vector<char>{'N', 'D', 'P'});
}

auto main() -> int {
    std::vector<Stack> stacks;
    std::vector<Cmd> cmds;
    {
        std::string line;
        std::vector<std::string> lines;
        while (std::getline(std::cin, line))
            lines.emplace_back(line);

        auto empty = std::ranges::find(lines, "");
        std::span stack_lines(lines.begin(), empty - 1);
        stacks.resize((stack_lines[0].size() + 1) / 4);
        for (int r = gsl::narrow_cast<int>(stack_lines.size()) - 1; r >= 0; --r)
            for (int c = 0; c < stack_lines[r].size(); ++c)
                if (isupper(stack_lines[r][c])) {
                    int s = (c - 1) / 4;
                    stacks[s].emplace_back(stack_lines[r][c]);
                }

        std::span cmd_lines(empty + 1, lines.end());
        for (const auto& cmd_line : cmd_lines) {
            const char* data = cmd_line.data();
            int num, from, to;
            sscanf(data, "move %d from %d to %d", &num, &from, &to);
            --from;
            --to;
            cmds.push_back({num, from, to});
        }
    }
    auto top = top_crates(rearrange(std::move(stacks), cmds));
    std::cout << std::string(top.begin(), top.end()) << '\n';
}
