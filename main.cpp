#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include "Blob.hpp"
#include "Dir.hpp"
#include "Object.hpp"
#include "getopt.hpp"
#include "utils.hpp"
#include <initializer_list>
#include "repo.hpp"

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

bool find_arg(const struct getopt& args, std::initializer_list<std::string> lst) {
    bool res = false;
    for (std::string item : lst) {
        res = res || args.has(item.c_str());
    }
    return res;
}

void init_repo() {
    repo::init_db();
}

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
    switch(util::str_hash(command)) {
        case util::hash_compile_time("init"):
            init_repo();
            break;
        default:
            print_help();
            break;
    }
    
    return 0;
}