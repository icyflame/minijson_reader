#include <fstream>
#include "finder.hpp"

int main(int argc, char* argv[]) {
    // Read from a file
    std::ifstream ifs(argv[1]);
    std::string file_contents {
        std::istreambuf_iterator<char>(ifs),
            std::istreambuf_iterator<char>()
    };

    const char* json_obj = file_contents.c_str();

    nested_json::finder finder(
        json_obj, strlen(json_obj)-1, 8150);

    std::string want_path = finder.start();

    std::cout << want_path << std::endl;

    return 0;
}
