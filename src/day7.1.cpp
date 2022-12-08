#include <algorithm>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <variant>
#include <vector>

struct Dir {
    std::string name;
};

struct File {
    int size;
    std::string name;
};

using Entry = std::variant<Dir, File>;

struct ListCmd {
    std::vector<Entry> entries;
};

struct ChangeDirCmd {
    std::string where;
};

using Cmd = std::variant<ListCmd, ChangeDirCmd>;
using Path = std::vector<std::string>;

constexpr auto update_path(const ChangeDirCmd& cd, Path& path) -> void {
    if (cd.where == "/")
        path.clear();
    else if (cd.where == "..")
        path.pop_back();
    else
        path.emplace_back(cd.where);
}

constexpr auto get_own_sizes(const std::vector<Cmd>& cmds) -> std::vector<std::pair<Path, int>> {
    Path path;
    std::vector<std::pair<Path, int>> sizes;
    for (const auto& cmd : cmds) {
        if (std::holds_alternative<ListCmd>(cmd)) {
            if (std::ranges::find_if(sizes, [&](const auto& p) { return p.first == path; }) != std::ranges::end(sizes))
                continue;
            const auto& ls = std::get<ListCmd>(cmd);
            int sz = 0;
            for (const auto& entry : ls.entries)
                if (std::holds_alternative<File>(entry)) {
                    const auto& file = std::get<File>(entry);
                    sz += file.size;
                }
            sizes.push_back({path, sz});
        } else {
            const auto& cd = std::get<ChangeDirCmd>(cmd);
            update_path(cd, path);
        }
    }
    return sizes;
}

constexpr auto get_children(const std::vector<Cmd>& cmds) -> std::vector<std::pair<Path, std::vector<std::string>>> {
    Path path;
    std::vector<std::pair<Path, std::vector<std::string>>> children;
    for (const auto& cmd : cmds) {
        if (std::holds_alternative<ListCmd>(cmd)) {
            if (std::ranges::find_if(children, [&](const auto& p) { return p.first == path; }) != std::ranges::end(children))
                continue;
            const auto& ls = std::get<ListCmd>(cmd);
            std::vector<std::string> names;
            for (const auto& entry : ls.entries)
                if (std::holds_alternative<Dir>(entry)) {
                    const auto& dir = std::get<Dir>(entry);
                    names.emplace_back(dir.name);
                }
            children.push_back({path, names});
        } else {
            const auto& cd = std::get<ChangeDirCmd>(cmd);
            update_path(cd, path);
        }
    }
    return children;
}

auto main() -> int {
    std::vector<Cmd> cmds;
    {
        std::vector<std::string> lines;
        {
            std::string line;
            while (std::getline(std::cin, line))
                lines.emplace_back(line);
        }

        for (const auto& line : lines) {
            if (line.starts_with("$")) {
                if (line == "$ ls")
                    cmds.emplace_back(ListCmd{});
                else
                    cmds.emplace_back(ChangeDirCmd{line.substr(5)});
            } else {
                auto& ls = std::get<ListCmd>(cmds.back());
                if (line.starts_with("dir"))
                    ls.entries.emplace_back(Dir{line.substr(4)});
                else {
                    std::stringstream ss(line);
                    int size;
                    std::string name;
                    ss >> size >> name;
                    ls.entries.emplace_back(File{size, name});
                }
            }
        }
    }
}
