#include <string>
#include <iostream>
#include <fstream>

#include "minijson_reader.hpp"

int wantOffset = 23;

void handle_array(minijson::const_buffer_context &ctx);
void handle_object(minijson::const_buffer_context &ctx);
void handle_final(minijson::const_buffer_context &ctx, minijson::value &v);

void handle_array(minijson::const_buffer_context &ctx) {
    std::cout << "Array" << std::endl;
    int index = 0;
    minijson::parse_array(ctx, [&](minijson::value v) {
            std::cout << "Array[" << index << "]" << std::endl;
            std::cout << ctx.read_offset() << std::endl;
            std::cout << ctx.length() << std::endl;
            std::cout << "Type: " << v.type() << std::endl;
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
            index++;
    });
}

void handle_object(minijson::const_buffer_context &ctx) {
    std::cout << "Object" << std::endl;
    minijson::parse_object(ctx, [&](const char *k, minijson::value v) {
            std::cout << "Key = " << k << std::endl;
            std::cout << ctx.read_offset() << std::endl;
            std::cout << ctx.length() << std::endl;
            std::cout << v.type() << std::endl;
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
            });
}

void handle_final(minijson::const_buffer_context &ctx, minijson::value &v) {
    std::cout << "FINAL " << v.as_string() << " at " << "Nesting level " << ctx.nesting_level() << std::endl;
}

int main(int argc, char* argv[]) {
    // Read from a file
    std::ifstream ifs(argv[1]);
    std::string file_contents { std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>() };
    const char* json_obj = file_contents.c_str();

    // Initiate the JSON parsing buffer
    minijson::const_buffer_context ctx(json_obj, strlen(json_obj) - 1);

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
