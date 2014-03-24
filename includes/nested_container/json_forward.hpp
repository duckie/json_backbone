#ifndef __NESTED_COMPILER_JSON_FORWARD__
#define __NESTED_COMPILER_JSON_FORWARD__

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/qi_real.hpp>
#include <boost/fusion/include/std_pair.hpp> 

namespace nested_container {
namespace json {

// namespaces aliases
namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;
namespace phoenix = boost::phoenix;

//enum class number_parsing_policy {
  //adaptive,
  //always_float
//};

//template <number_parsing_policy NumberPolicy> struct parsing_traits {
  //static constexpr number_parsing_policy number_policy = NumberPolicy;
//};

template<typename Container, typename Iterator> struct raw_grammar : qi::grammar<Iterator, Container(), boost::spirit::ascii::space_type> {
  template <typename T> struct strict_real_policies : qi::real_policies<T> { static bool constexpr expect_dot = true; };

  raw_grammar();

  using st_t = ascii::space_type;
  qi::rule<Iterator, Container(), st_t> root;
  qi::rule<Iterator, typename Container::map_type (), st_t> object;
  qi::rule<Iterator, std::pair<typename Container::key_type, Container> (), st_t> object_pair;
  qi::rule<Iterator, typename Container::vector_type (), st_t> array;
  qi::rule<Iterator, boost::optional<Container> (), st_t> value;
  //qi::rule<Iterator, Container(), st_t> value;
  qi::rule<Iterator, typename Container::key_type (), st_t> key_value;
  qi::rule<Iterator, typename Container::str_type (), st_t> string_value;
  qi::rule<Iterator, typename Container::int_type (), st_t> int_value;
  qi::rule<Iterator, typename Container::uint_type (), st_t> uint_value;
  qi::rule<Iterator, typename Container::float_type (), st_t> float_value;
  qi::rule<Iterator, bool (), st_t> bool_value;
  qi::rule<Iterator, typename std::nullptr_t (), st_t> null_value;
  
  qi::symbols<char const, char const> unesc_char;
};

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

  template <class Iterator> using grammar = raw_grammar<container_type, Iterator>;
  grammar<typename string_type::const_iterator> const grammar_;

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
  //using parsing_traits = ParsingTraits;

  string_type serialize(container_type const& container) const {
    visitor_ostream visitor;
    container.visit(visitor);
    return visitor.output_stream.str();
  }

  container_type deserialize(string_type const& input);
};

}  // namespace json
}  // namespace nested_container

#endif  // __NESTED_COMPILER_JSON_FORWARD__
