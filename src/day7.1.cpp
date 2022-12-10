#include <iostream>
#include <numeric>
#include <ranges>
#include <span>
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

struct CdCmd {
    std::string where;
};

using Cmd = std::variant<ListCmd, CdCmd>;
using Path = std::vector<std::string>;

struct Metadata {
    std::vector<std::pair<Path, int>> own_sizes;
    std::vector<std::pair<Path, std::vector<std::string>>> folders;
    std::vector<std::pair<Path, int>> total_sizes;
};

template<class T1, class T2>
constexpr auto find_or_insert(std::vector<std::pair<T1, T2>>& v, const T1& x) -> T2& {
    auto it = std::ranges::find_if(v, [&](const std::pair<T1, T2>& p) { return p.first == x; });
    if (it == std::ranges::end(v))
        it = v.insert(v.end(), {x, T2()});
    return it->second;
}

constexpr auto change_dir(Path path, std::string_view where) -> Path {
    if (where == "/")
        path.clear();
    else if (where == "..")
        path.pop_back();
    else
        path.emplace_back(where);
    return path;
}

constexpr auto process(Path path, std::span<const Cmd> cmds, Metadata&& metadata) -> Metadata {
    if (cmds.empty())
        return metadata;

    const auto& cmd = cmds.front();
    if (std::holds_alternative<ListCmd>(cmd)) {
        const auto& list_cmd = std::get<ListCmd>(cmd);
        auto& [own_sizes, folders, _] = metadata;
        auto& own_size = find_or_insert(own_sizes, path);
        own_size = 0;
        auto& my_folders = find_or_insert(folders, path);
        my_folders.clear();
        for (const auto& entry : list_cmd.entries) {
            if (std::holds_alternative<Dir>(entry)) {
                const auto& dir = std::get<Dir>(entry);
                my_folders.emplace_back(dir.name);
            } else {
                const auto& file = std::get<File>(entry);
                own_size += file.size;
            }
        }
        metadata = process(path, cmds.subspan(1), std::move(metadata));
    } else {
        const auto& cd_cmd = std::get<CdCmd>(cmd);
        metadata = process(change_dir(path, cd_cmd.where), cmds.subspan(1), std::move(metadata));
    }
    auto& [own_sizes, folders, total_sizes] = metadata;
    find_or_insert(total_sizes, path) = find_or_insert(own_sizes, path);
    for (const auto& folder : find_or_insert(folders, path)) {
        auto size = find_or_insert(total_sizes, change_dir(path, folder));
        find_or_insert(total_sizes, path) += size;
    }
    return metadata;
}

static constexpr auto size_limit = 100000;

constexpr auto small_dirs_size(const std::vector<Cmd>& cmds) -> int {
    // clang-format off
    auto metadata = process({}, std::span{cmds}, Metadata{});
    auto small = metadata.total_sizes
                           | std::views::transform([](const std::pair<Path, int>& p) { return p.second; })
                           | std::views::filter([&](int size) { return size <= size_limit; });
    return std::reduce(small.begin(), small.end());
    // clang-format on
}

constexpr auto tests() -> void {
    static_assert(
        small_dirs_size({CdCmd{"/"}, ListCmd{{Dir{"a"}, File{14848514, "b.txt"}, File{8504156, "c.dat"}, Dir{"d"}}},
                         CdCmd{"a"}, ListCmd{{Dir{"e"}, File{29116, "f"}, File{2557, "g"}, File{62596, "h.lst"}}},
                         CdCmd{"e"}, ListCmd{{File{584, "i"}}}, CdCmd{".."}, CdCmd{".."}, CdCmd{"d"},
                         ListCmd{{File{4060174, "j"}, File{8033020, "d.log"}, File{5626152, "d.ext"},
                                  File{7214296, "k"}}}}) == 95437);
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
                    cmds.emplace_back(CdCmd{line.substr(5)});
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
    std::cout << small_dirs_size(cmds) << '\n';
}
