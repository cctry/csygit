#if !defined(REPO_H)
#define REPO_H
#include "Object.hpp"
namespace repo {
void init_db();
void save_obj(Object& obj);
std::string load_obj(std::string hash);
}  // namespace repo

#endif  // REPO_H
