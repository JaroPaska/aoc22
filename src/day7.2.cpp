#include <algorithm>
#include <iostream>
#include <ranges>
#include <span>
#include <sstream>
#include <string>
#include <variant>
#include <vector>

template<class T1, class T2>
struct map {
    std::vector<std::pair<T1, T2>> vec;

    constexpr auto operator[](const T1& x) -> T2& {
        for (auto& [k, v] : vec)
            if (k == x)
                return v;

        vec.emplace_back(x, T2());
        return vec.back().second;
    }
};

struct Dir {
    std::string name;
};

struct File {
    int size;
    std::string name;
};

struct Ls {
    std::vector<std::variant<Dir, File>> entries;
};

struct Cd {
    std::string where;
};

using Cmd = std::variant<Ls, Cd>;
using Path = std::vector<std::string>;

struct Metadata {
    map<Path, int> own_sizes;
    map<Path, std::vector<std::string>> folders;
    map<Path, int> total_sizes;
};

constexpr auto change_dir(Path path, std::string_view where) -> Path {
    if (where == "/")
        path.clear();
    else if (where == "..")
        path.pop_back();
    else
        path.emplace_back(where);
    return path;
}

template<class... Ts>
struct visitor : Ts... {
    using Ts::operator()...;
};

constexpr auto process(Path path, std::span<const Cmd> cmds, Metadata&& metadata) -> Metadata {
    if (cmds.empty())
        return metadata;

    const auto& cmd = cmds.front();
    metadata = std::visit(
        visitor{
            [&](const Ls& ls) {
                auto& [own_sizes, folders, _] = metadata;
                auto& own_size = own_sizes[path];
                auto& my_folders = folders[path];
                own_size = 0;
                my_folders.clear();
                for (const auto& entry : ls.entries)
                    std::visit(
                        visitor{
                            [&](const Dir& dir) { my_folders.emplace_back(dir.name); },
                            [&](const File& file) { own_size += file.size; }},
                        entry
                    );
                return process(path, cmds.subspan(1), std::move(metadata));
            },
            [&](const Cd& cd) { return process(change_dir(path, cd.where), cmds.subspan(1), std::move(metadata)); }},
        cmd
    );
    auto& [own_sizes, folders, total_sizes] = metadata;
    total_sizes[path] = own_sizes[path];
    for (const auto& folder : folders[path]) {
        auto size = total_sizes[change_dir(path, folder)];
        total_sizes[path] += size;
    }
    return metadata;
}

static constexpr auto total_space = 70000000;
static constexpr auto need_unused = 30000000;

constexpr auto smallest_big_dir(const std::vector<Cmd>& cmds) -> int {
    // clang-format off
    auto metadata = process({}, std::span{cmds}, Metadata{});
    auto used = metadata.total_sizes[{}];
    auto small = metadata.total_sizes.vec
               | std::views::values
               | std::views::filter([&](int size) { return size >= need_unused - (total_space - used); });
    return *std::ranges::min_element(small);
    // clang-format on
}

namespace tests {
    static_assert(change_dir(Path{}, "a") == Path{"a"});
    static_assert(
        smallest_big_dir(
            {Cd{"/"}, Ls{{Dir{"a"}, File{14848514, "b.txt"}, File{8504156, "c.dat"}, Dir{"d"}}}, Cd{"a"},
             Ls{{Dir{"e"}, File{29116, "f"}, File{2557, "g"}, File{62596, "h.lst"}}}, Cd{"e"}, Ls{{File{584, "i"}}},
             Cd{".."}, Cd{".."}, Cd{"d"},
             Ls{{File{4060174, "j"}, File{8033020, "d.log"}, File{5626152, "d.ext"}, File{7214296, "k"}}}}
        ) == 24933642
    );
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
                    cmds.emplace_back(Ls{});
                else
                    cmds.emplace_back(Cd{line.substr(5)});
            } else {
                auto& ls = std::get<Ls>(cmds.back());
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
    std::cout << smallest_big_dir(cmds) << '\n';
}
