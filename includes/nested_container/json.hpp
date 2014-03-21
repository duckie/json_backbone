#ifndef __NESTED_COMPILER_JSON__
#define __NESTED_COMPILER_JSON__

namespace nested_container {
namespace json {

template<typename container_type> class serializer {
  using base_visitor_type   = typename container_type::const_visitor;
  using string_type         = typename container_type::str_type;
  using ostream_type        = std::basic_ostringstream<typename string_type::value_type, typename string_type::traits_type>;

  using key_type    = typename container_type::key_type;
  using map_type    = typename container_type::map_type;
  using vector_type = typename container_type::vector_type;
  using float_type  = typename container_type::float_type;
  using int_type    = typename container_type::int_type;
  using uint_type   = typename container_type::uint_type;

  struct visitor_ostream : public base_visitor_type {
    ostream_type output_stream;
    
    virtual void apply(std::nullptr_t) override { output_stream << "null"; }
    
    virtual void apply(map_type const& v) override {
      output_stream << "{";
      bool first = true;
      for(auto const& element : v) {
        if (!first) output_stream << ",";
        first = false;
        output_stream << "\"" << element.first << "\":";
        element.second.visit(*this);
      }
      output_stream << "}";
    }
    
    virtual void apply(vector_type const& v) override {
      output_stream << "[";
      bool first = true;
      for(container_type const& element : v) {
        if (!first) output_stream << ",";
        first = false;
        element.visit(*this);
      }
      output_stream << "]";
    }
    
    virtual void apply(string_type const& v) override { output_stream << "\"" << v << "\""; }
    virtual void apply(float_type v) override { output_stream << v; }
    virtual void apply(int_type v) override { output_stream << v; }
    virtual void apply(uint_type v) override { output_stream << v; }
    virtual void apply(bool v) override { output_stream << (v?"true":"false"); }
  };

 public:
  string_type serialize(container_type const& container) const {
    visitor_ostream visitor;
    container.visit(visitor);
    return visitor.output_stream.str();
  }

  container_type deserialize(string_type const& input) {
  }
};

}  // namespace json
}  // namespace nested_container

#endif  // __NESTED_COMPILER_JSON__
