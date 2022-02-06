#include <iostream>
#include <vector>
#include <filesystem>

const std::string path_separator = ":";

std::vector<std::string> split(const std::string& path_env, const std::string& separator) {
    auto offset = std::string::size_type(0);
    std::vector<std::string> list;
    auto separator_length = separator.length();
    while (true) {
        auto pos = path_env.find(separator, offset);
        if (pos == std::string::npos) {
            list.push_back(path_env.substr(offset));
            break;
        }
        list.push_back(path_env.substr(offset, pos - offset));
        offset = pos + separator_length;
    }

    return list;
}

std::filesystem::path pathCombine(const std::string& directory, const std::string& fileName) {
    using namespace std::filesystem;
    return path(directory) / path(fileName);
}

bool pathExists(const std::filesystem::path& path) {
    using namespace std::filesystem;
    return exists(path);
}

std::string getFilePath(const std::string& directory, const std::string& fileName) {
    using namespace std::filesystem;

    if (!pathExists(path(directory)))
        return "";

    const auto full_path = pathCombine(directory, fileName);
    const directory_iterator iter(directory);
    for (auto &file: directory_iterator(directory)) {
        if (file.path().string() == full_path.string()) {
            return full_path.string();
        }
    }

    return "";
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cout << "no argument" << std::endl;
        return -1;
    }

    const std::string search_file = std::string(argv[1]);
    const char* p = std::getenv("PATH");
    if (p == nullptr) {
        std::cout << "environment variable \"PATH\" read error." << std::endl;
        return -1;
    }

    const std::string path_env = std::string(p);
    auto paths = split(path_env, path_separator);
    for (auto &path : paths) {
//        const auto file = getFilePath(path, search_file);
//        if (!file.empty()) {
//            std::cout << file << std::endl;
//            return 0;
//        }

        auto combine_path = pathCombine(path, search_file);
        if (pathExists(combine_path)) {
            std::cout << combine_path.string() << std::endl;
            return 0;
        }
    }

    std::cout << std::endl;
    return 0;
}
