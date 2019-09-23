#include <fstream>
#include "nested_json/finder.hpp"

int main(int argc, char* argv[]) {
    // Read from a file
    std::ifstream ifs(argv[1]);
    std::string file_contents {
        std::istreambuf_iterator<char>(ifs),
            std::istreambuf_iterator<char>()
    };

    const char* json_obj = file_contents.c_str();

    int offset = atoi(argv[2]);

    nested_json::finder finder_obj(json_obj, strlen(json_obj)-1, offset);

    std::string want_path = finder_obj.start();

    std::cout << want_path << std::endl;

    return 0;
}
