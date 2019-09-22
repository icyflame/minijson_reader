#include <string>
#include <iostream>
#include <fstream>
#include <vector>

#include "minijson_reader.hpp"

namespace nested_json_parser {
    class nested_json_parser {
        private:
            const char* m_json_string;
            int m_length;
            std::vector<std::string> m_current_path;

        public:
            explicit nested_json_parser(const char* json_string, int length) :
                m_json_string(json_string),
                m_length(length) {
                }

            void start() {
                minijson::const_buffer_context ctx(m_json_string, m_length);
                m_current_path.push_back("");

                switch (ctx.toplevel_type()) {
                    case minijson::Array:
                        handle_array(ctx);
                        break;
                    case minijson::Object:
                        handle_object(ctx);
                        break;
                    default:
                        throw std::runtime_error("Invalid JSON");
                }
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
                        m_current_path.push_back("[" + std::to_string(index) + "]");
                        handle_value(ctx, v);

                        index++;
                        });
                m_current_path.pop_back();
            }

            void handle_object(minijson::const_buffer_context &ctx) {
                minijson::parse_object(ctx, [&](const char *k, minijson::value v) {
                        m_current_path.push_back(std::string(k));
                        handle_value(ctx, v);
                        });
            }

            void handle_final(minijson::const_buffer_context &ctx, minijson::value &v) {
                std::cout << join(m_current_path, ".")
                    << " = "
                    << v.as_string()
                    << " ("
                    << minijson::value_type_string(v.type()).to_string()
                    << ")"
                    << std::endl;
                m_current_path.pop_back();
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
    }; // class nested_json_parser
} // namespace nested_json_parser
