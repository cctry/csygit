#if !defined(UTIL_H)
#define UTIL_H
#include <string>
#include <vector>
namespace util {
extern int get_file_size(std::string path);
extern std::string get_file_content(std::string path);
extern bool is_file_exist(std::string path);
extern std::vector<std::string> get_all_files(std::string path);
extern std::vector<std::string> get_all_dirs(std::string path);
}  // namespace util
#endif  // UTIL_H
