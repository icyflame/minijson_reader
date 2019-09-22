#include <string>
#include <iostream>
#include <fstream>
#include <vector>

#include "minijson_reader.hpp"

void handle_array(minijson::const_buffer_context &ctx);
void handle_object(minijson::const_buffer_context &ctx);
void handle_final(minijson::const_buffer_context &ctx, minijson::value &v);
void handle_value(minijson::const_buffer_context &ctx, minijson::value &v);
std::string join(std::vector<std::string> arr, std::string joining_string);

// Required global variables
std::vector<std::string> current_path;

int main(int argc, char* argv[]) {
    // Read from a file
    std::ifstream ifs(argv[1]);
    std::string file_contents { std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>() };
    const char* json_obj = file_contents.c_str();

    // Initiate the JSON parsing buffer
    minijson::const_buffer_context ctx(json_obj, strlen(json_obj) - 1);

    current_path.push_back("root");

    switch (json_obj[0]) {
        case '[':
            handle_array(ctx);
            break;
        case '{':
            handle_object(ctx);
            break;
        default:
            throw std::runtime_error("Invalid JSON");
    }

    return 0;
}

void handle_value(minijson::const_buffer_context &ctx, minijson::value &v) {
    switch (v.type()) {
        case minijson::String:
        case minijson::Number:
        case minijson::Boolean:
        case minijson::Null:
            handle_final(ctx, v);
            break;
        case minijson::Object:
            handle_object(ctx);
            break;
        case minijson::Array:
            handle_array(ctx);
            break;
    }
}

void handle_array(minijson::const_buffer_context &ctx) {
    int index = 0;
    minijson::parse_array(ctx, [&](minijson::value v) {
            current_path.push_back(std::to_string(index));
            handle_value(ctx, v);

            index++;
            });
}

void handle_object(minijson::const_buffer_context &ctx) {
    minijson::parse_object(ctx, [&](const char *k, minijson::value v) {
            current_path.push_back(std::string(k));
            handle_value(ctx, v);
            });
}

void handle_final(minijson::const_buffer_context &ctx, minijson::value &v) {
    std::cout << join(current_path, " > ") << " = " << v.as_string() << std::endl;
    current_path.pop_back();
}

std::string join(std::vector<std::string> arr, std::string joining_string) {
    std::string output;
    for (int i = 0; i < (arr.size()-1); i++) {
        output += arr[i];
        output += joining_string;
    }

    output += arr[arr.size()-1];
    return output;
}
