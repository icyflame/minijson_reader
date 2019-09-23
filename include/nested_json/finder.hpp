#include <string>
#include <iostream>
#include <fstream>
#include <vector>

#include "minijson_reader.hpp"
#include "parser.hpp"

namespace nested_json {
  class finder : public parser {
    protected:
      int m_want_offset;
      std::string m_want_path;

    public:
      explicit finder(
          const char* json_string,
          int str_length,
          int want_offset) :
        nested_json::parser(json_string, str_length),
        m_want_offset(want_offset) {
          m_want_path = "";
        }

      void handle_value(minijson::const_buffer_context &ctx, minijson::value &v) override {
        if (ctx.read_offset() >= m_want_offset && m_want_path == "") {
          m_want_path = parser::join(m_current_path, "");
          minijson::ignore(ctx);
          return;
        }

        parser::handle_value(ctx, v);
      }

      std::string start() {
        parser::start();
        return m_want_path;
      }

  }; // class finder
} // namespace nested_json
