#include <unistd.h>
#include <ctime>
#include <initializer_list>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include "Blob.hpp"
#include "Commit.hpp"
#include "Dir.hpp"
#include "Object.hpp"
#include "cerror.hpp"
#include "getopt.hpp"
#include "repo.hpp"
#include "utils.hpp"

void print_help();
struct getopt get_opt_obj(int argc, char** const argv);
bool find_arg(const struct getopt& args,
              std::initializer_list<std::string> lst);
void commit();
void init_repo();
bool is_init();
void list_commits();

int main(int argc, char** argv) {
    if (argc < 2) {
        print_help();
        return 0;
    }
    std::string command = argv[1];
    struct getopt args = get_opt_obj(argc, argv);
    if (find_arg(args, {"-h", "--help", "-?"})) {
        print_help();
        return 0;
    }
    switch (util::str_hash(command)) {
        case util::hash_compile_time("init"):
            init_repo();
            break;
        case util::hash_compile_time("commit"):
            commit();
            break;
        case util::hash_compile_time("list"):
            list_commits();
            break;
        default:
            print_help();
            break;
    }
    return 0;
}

void list_commits() {
    std::string head_hash = repo::get_head_hash();
    std::string prev_hash;
    do {
        std::unique_ptr<Commit> head(Commit::get_from_db(head_hash));
        head->print();
        prev_hash = head->get_prev();
    } while (!util::is_zero_hash(prev_hash));
}

bool is_init() {
    return util::is_dir_exist(DEFAULT_REPO_NAME);
}

void init_repo() {
    if (is_init()) {
        return;
    }
    repo::init_db();
    std::cout << "Sucessfully initiate repo" << std::endl;
}

void print_help() {
    std::cout << "This is the help information" << std::endl;
}

struct getopt get_opt_obj(int argc, char** const argv) {
    std::vector<std::string> vec;
    vec.reserve(argc);
    for (int i = 0; i < argc; i++) {
        vec.push_back(argv[i]);
    }
    struct getopt res(vec);
    return res;
}

bool find_arg(const struct getopt& args,
              std::initializer_list<std::string> lst) {
    bool res = false;
    for (std::string item : lst) {
        res = res || args.has(item.c_str());
    }
    return res;
}

void commit() {
    if (!is_init()) {
        cerror::occur_error("Please init first");
    }
    std::unique_ptr<Commit> commit(Commit::get_from_dir(util::get_cwd()));
    std::cout << "input commit message:" << std::endl;
    getline(std::cin, commit->set_content());
    repo::save_obj(*commit);
    std::cout << commit->get_hash() << std::endl;
}
