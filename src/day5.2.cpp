#include <GSL/assert>
#include <GSL/narrow>

#include <array>
#include <fstream>
#include <iostream>
#include <numeric>
#include <ranges>
#include <span>
#include <string>
#include <vector>

auto main() -> int {
    std::vector<std::vector<char>> stacks;
    std::vector<std::array<int, 3>> cmds;
    {
        std::ifstream ifs("input");
        Expects(ifs);

        std::string line;
        std::vector<std::string> lines;
        while (std::getline(ifs, line))
            lines.emplace_back(line);

        auto empty = std::ranges::find(lines, "");
        std::span stack_lines(lines.begin(), empty - 1);
        stacks.resize((stack_lines[0].size() + 1) / 4);
        for (int r = gsl::narrow_cast<int>(stack_lines.size()) - 1; r >= 0; --r)
            for (int c = 0; c < gsl::narrow_cast<int>(stack_lines[r].size()); ++c)
                if (isupper(stack_lines[r][c])) {
                    int s = (c - 1) / 4;
                    stacks[s].emplace_back(stack_lines[r][c]);
                }

        std::span cmd_lines(empty + 1, lines.end());
        for (const auto& cmd_line : cmd_lines) {
            const char* data = cmd_line.data();
            std::array<int, 3> cmd;
            sscanf_s(data, "move %d from %d to %d", &cmd[0], &cmd[1], &cmd[2]);
            --cmd[1];
            --cmd[2];
            cmds.emplace_back(cmd);
        }
    }

    for (const auto& cmd : cmds) {
        const auto& [num, from, to] = cmd;
        stacks[to].insert(stacks[to].end(), stacks[from].end() - num, stacks[from].end());
        stacks[from].erase(stacks[from].end() - num, stacks[from].end());
    }

    auto res = stacks | std::views::transform([](const std::vector<char>& stack) { return stack.back(); });
    for (const auto& c : res)
        std::cout << c;
    std::cout << '\n';
}