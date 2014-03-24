#ifndef __NESTED_COMPILER_JSON__
#define __NESTED_COMPILER_JSON__

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/spirit/include/qi_real.hpp>
#include <boost/spirit/include/qi_int.hpp>
#include <boost/spirit/include/qi_uint.hpp>
#include <boost/fusion/include/std_pair.hpp> 
#include "json_forward.hpp"

namespace nested_container {
namespace json {

// namespaces aliases
namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;
namespace phoenix = boost::phoenix;

template<typename Container, typename Iterator> raw_grammar<Container, Iterator>::raw_grammar() : raw_grammar<Container, Iterator>::base_type(root) {
  namespace ascii = boost::spirit::ascii;
  using namespace qi::labels;
  using phoenix::at_c;
  using phoenix::push_back;

  unesc_char.add("\\a", '\a')("\\b", '\b')("\\f", '\f')("\\n", '\n')
    ("\\r", '\r')("\\t", '\t')("\\v", '\v')
    ("\\\\", '\\')("\\\'", '\'')("\\\"", '\"')
    (" ",' ')("/",'/')
    ;

  qi::int_parser< typename Container::int_type> int_parser;
  qi::uint_parser< typename Container::uint_type> uint_parser;
  qi::real_parser< typename Container::float_type, strict_real_policies<typename Container::float_type> > float_parser;

  root = object | array;
  null_value = qi::lit("null") [ _val = nullptr ];
  bool_value = qi::bool_;
  int_value = int_parser [_val = _1];
  uint_value = uint_parser [_val = _1];
  float_value = float_parser [_val = _1];
  string_value = '"' >> *(unesc_char | qi::alnum | "\\x" >> qi::hex) >> '"';
  key_value = '"' >> *qi::alnum >> '"';
  value = float_value | uint_value | int_value | bool_value | null_value | string_value;
  array = '[' >> -(value % ',') >>']';
  object = '{' >> -(object_pair % ',') >>'}';
  object_pair =  key_value  >> ':' >> (value | array | object);
}


template<typename container_type> container_type serializer<container_type>::deserialize(string_type const& input) {
  container_type result;
  typename string_type::const_iterator iter = input.begin();
  typename string_type::const_iterator end = input.end();
  bool success = qi::phrase_parse(iter, end, grammar_, boost::spirit::ascii::space, result);
  //bool success = qi::phrase_parse(iter, end, grammar_, boost::spirit::ascii::space);
  std::cout << "Parsing " << (success ? "succeeded":"failed") << std::endl;
  return result;
}

}  // namespace json
}  // namespace nested_container

#endif  // __NESTED_COMPILER_JSON__
