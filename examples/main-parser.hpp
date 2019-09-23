#include <fstream>
#include "nested_json/parser.hpp"

int main(int argc, char* argv[]) {
  // Read from a file
  std::ifstream ifs(argv[1]);
  std::string file_contents {
    std::istreambuf_iterator<char>(ifs),
      std::istreambuf_iterator<char>()
  };

  const char* json_obj = file_contents.c_str();

  nested_json::parser parser(json_obj, strlen(json_obj)-1);

  parser.start();

  return 0;
}
