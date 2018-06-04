#if !defined(UTIL_H)
#define UTIL_H
#include <string>
namespace util {
extern int get_file_size(std::string path);
extern std::string get_file_content(std::string path);
extern bool is_file_exist(std::string path);
}  // namespace util
#endif  // UTIL_H
