#include <string>
#include <iostream>
#include <fstream>
#include <vector>

#include "minijson_reader.hpp"
#include "nested_json_parser.hpp"

namespace nested_json_finder {
  class nested_json_finder : public nested_json_parser::nested_json_parser {
    protected:
      int m_want_offset;
      std::string m_want_path;

    public:
      explicit nested_json_finder(
          const char* json_string,
          int str_length,
          int want_offset) :
        nested_json_parser::nested_json_parser(json_string, str_length),
        m_want_offset(want_offset) {
          m_want_path = "";
        }

      void handle_value(minijson::const_buffer_context &ctx, minijson::value &v) override {
        if (ctx.read_offset() >= m_want_offset && m_want_path == "") {
          m_want_path = nested_json_parser::nested_json_parser::join(m_current_path, "");
          return;
        }

        nested_json_parser::nested_json_parser::handle_value(ctx, v);
      }

      std::string start() {
        nested_json_parser::nested_json_parser::start();
        return m_want_path;
      }

  }; // class nested_json_finder
} // namespace nested_json_finder
