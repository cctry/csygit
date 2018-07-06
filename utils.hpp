#pragma once
#include <climits>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
namespace util {
extern int get_file_size(const std::string& path);
extern std::string get_file_content(const std::string& path);
extern bool is_file_exist(const std::string& path);
extern std::vector<std::string> get_all_files(const std::string& path);
extern std::vector<std::string> get_all_dirs(const std::string& path);
extern int get_dir_permission(const std::string& path);
extern int get_file_permission(const std::string& path);
std::vector<std::string> get_lines(std::ifstream& f);
bool is_dir_exist(const std::string& path);
int load_a_int(std::ifstream& f);
int cmkdir(const std::string& path, int mode);
std::string load_string_from_file(int len, std::ifstream& f);

/*
 * The result include begin and exclude end
 * If vec = {1,2,3,4,5}, then the return of (0,3) will be {1,2,3}
 * If omit end, end will be the end of vector
 * The negative paras mean the number being counted from the last one.
 * -1 means the very last one, and so on.
 * Literally, it equals to vec.size() - 1
 */
template <typename T>
std::vector<T> slice_vector(const std::vector<T>& vec,
                            int begin,
                            long end = LONG_MAX) {
    int tbegin = begin;
    int tend = end;
    if (end < 0) {
        tend = vec.size() + end;
    }
    if (end == LONG_MAX) {
        tend = vec.size();
    }
    if (begin < 0) {
        tbegin = vec.size() + begin;
    }
    auto first = vec.begin() + tbegin;
    auto last = vec.begin() + tend;
    std::vector<T> res(first, last);
    return res;
}

typedef std::uint64_t hash_t;

constexpr hash_t prime = 0x100000001B3ull;
constexpr hash_t basis = 0xCBF29CE484222325ull;

hash_t str_hash(const std::string& str);
constexpr hash_t hash_compile_time(char const* str, hash_t last_value = basis) {
    return *str ? hash_compile_time(str + 1, (*str ^ last_value) * prime)
                : last_value;
}

}  // namespace util
