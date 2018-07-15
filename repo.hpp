#pragma once
#include "Object.hpp"
#define DEFAULT_REPO_NAME ".repo"
#define SLASH "/"
#define DEFAULT_OBJECTS_NAME "objects"
#define DEFAULT_OBJECTS_PATH DEFAULT_REPO_NAME SLASH DEFAULT_OBJECTS_NAME SLASH
#define DEFAULT_HEADS_PATH "heads"
#define HASH_LEN 40
#define HEAD_FILE "HEAD"
#define HEAD_PATH DEFAULT_REPO_NAME SLASH HEAD_FILE

namespace repo {
void init_db();
void save_obj(Object &obj);
std::string load_obj(const std::string &hash);
const std::string get_head_hash();
std::ofstream create_head();
} // namespace repo
