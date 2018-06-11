#if !defined(REPO_H)
#define REPO_H
#include "Object.hpp"
#define DEFAULT_REPO_NAME ".repo"
#define HASH_LEN 40
namespace repo {
void init_db();
void save_obj(Object& obj);
std::string load_obj(std::string hash);
}  // namespace repo

#endif  // REPO_H
