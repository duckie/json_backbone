#ifndef __NESTED_COMPILER_JSON__
#define __NESTED_COMPILER_JSON__

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/spirit/include/qi_real.hpp>
#include <boost/spirit/include/qi_int.hpp>
#include <boost/spirit/include/qi_uint.hpp>
#include <boost/fusion/include/std_pair.hpp> 
#include <boost/spirit/include/karma.hpp>
#include <boost/spirit/include/karma_real.hpp>
#include <boost/spirit/include/karma_int.hpp>
#include <boost/spirit/include/karma_uint.hpp>
#include <boost/spirit/include/karma_string.hpp>
#include "json_forward.hpp"

namespace nested_container {
namespace json {

// namespaces aliases
namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;
namespace phoenix = boost::phoenix;
namespace karma = boost::spirit::karma;

template<typename Container, typename Iterator> struct raw_grammar : qi::grammar<Iterator, Container(), boost::spirit::ascii::space_type> {
  template <typename T> struct strict_real_policies : qi::real_policies<T> { static bool constexpr expect_dot = true; };

  raw_grammar() : raw_grammar<Container, Iterator>::base_type(root) {
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


  using st_t = ascii::space_type;
  qi::rule<Iterator, Container(), st_t> root;
  qi::rule<Iterator, typename Container::map_type (), st_t> object;
  qi::rule<Iterator, std::pair<typename Container::key_type, Container> (), st_t> object_pair;
  qi::rule<Iterator, typename Container::vector_type (), st_t> array;
  qi::rule<Iterator, boost::optional<Container> (), st_t> value;
  qi::rule<Iterator, typename Container::key_type (), st_t> key_value;
  qi::rule<Iterator, typename Container::str_type (), st_t> string_value;
  qi::rule<Iterator, typename Container::int_type (), st_t> int_value;
  qi::rule<Iterator, typename Container::uint_type (), st_t> uint_value;
  qi::rule<Iterator, typename Container::float_type (), st_t> float_value;
  qi::rule<Iterator, bool (), st_t> bool_value;
  qi::rule<Iterator, typename std::nullptr_t (), st_t> null_value;
  qi::symbols<char const, char const> unesc_char;
};

template <typename T> struct type_traits {
  static bool constexpr is_float = std::is_floating_point<T>::value;
  static bool constexpr is_int = 
    std::is_integral<T>::value 
    && std::is_signed<T>::value 
    && !std::is_same<bool,T>::value
    && !std::is_same<char,T>::value
    && !std::is_same<char16_t,T>::value
    && !std::is_same<char32_t,T>::value
    && !std::is_same<wchar_t,T>::value
    ;
  static bool constexpr is_uint = 
    std::is_integral<T>::value 
    && !std::is_signed<T>::value 
    && !std::is_same<bool,T>::value
    && !std::is_same<char,T>::value
    && !std::is_same<char16_t,T>::value
    && !std::is_same<char32_t,T>::value
    && !std::is_same<wchar_t,T>::value
    ;
};

template <typename T, typename V> struct string_conversion_traits {};

template <typename T> struct string_conversion_traits<T, typename std::enable_if<type_traits<T>::is_int, void>::type> {
  static size_t constexpr max_size = std::numeric_limits<T>::digits + 1u;
};

template <typename T> struct string_conversion_traits<T, typename std::enable_if<type_traits<T>::is_uint, void>::type> {
  static size_t constexpr max_size = std::numeric_limits<T>::digits;
};

template <> struct string_conversion_traits<float, void> {
  static size_t constexpr max_size = 3 + FLT_MANT_DIG - FLT_MIN_EXP;
};

template <> struct string_conversion_traits<double, void> {
  static size_t constexpr max_size = 3 + DBL_MANT_DIG - DBL_MIN_EXP;
};

template<typename Container, typename StreamType> serializer_impl_envelop<Container, StreamType>::~serializer_impl_envelop() {}

template<typename Container, typename StreamType> class serializer_impl : public serializer_impl_envelop<Container, StreamType> {
  using string_type         = typename Container::str_type;
  using ostream_type        = std::basic_ostringstream<typename StreamType::value_type, typename StreamType::traits_type>;
  using key_type            = typename Container::key_type;
  using map_type            = typename Container::map_type;
  using vector_type         = typename Container::vector_type;
  using float_type          = typename Container::float_type;
  using int_type            = typename Container::int_type;
  using uint_type           = typename Container::uint_type;

  // Grammar member
  using grammar = raw_grammar<Container, typename StreamType::const_iterator>;
  grammar const grammar_;


  // Size computation
  struct visitor_size {
    size_t size = 0u;
    
    void apply(std::nullptr_t) { size += 3u; }
    
    void apply(map_type const& v) {
      size += 2u;  // {}
      bool first = true;
      for(auto const& element : v) {
        if (!first) ++size;
        first = false;
        size += 3u + element.first.size();
        element.second.const_visit(*this);
      }
    }
    
    void apply(vector_type const& v) {
      size += 2; // []
      bool first = true;
      for(Container const& element : v) {
        if (!first) ++size;
        first = false;
        element.const_visit(*this);
      }
    }
    
    void apply(string_type const& v) { size += 2u + v.size(); }
    void apply(float_type v) { size += string_conversion_traits<float_type, void>::max_size; }
    void apply(int_type v) { size += string_conversion_traits<int_type, void>::max_size; }
    void apply(uint_type v) { size += string_conversion_traits<uint_type, void>::max_size; }
    void apply(bool v) { size += 5; }
  };

  // Visitor karma
  struct visitor_karma {
    typename string_type::value_type* init_buffer_ = nullptr;
    typename string_type::value_type* buffer_ = nullptr;

    // Generators
    //using boost::spirit::karma::generate;
    karma::real_generator<float_type> float_generator_;
    karma::int_generator<int_type> int_generator_;
    karma::uint_generator<uint_type> uint_generator_;
    
    void apply(std::nullptr_t) { 
      karma::generate(buffer_,karma::lit("null"));  
    }
    
    void apply(map_type const& v) {
      karma::generate(buffer_,karma::char_, '{');  
      bool first = true;
      for(auto const& element : v) {
        if (!first) karma::generate(buffer_,karma::char_(','));
        first = false;
        karma::generate(buffer_, karma::char_('"') << karma::string(element.first) << karma::lit("\":"));
        element.second.const_visit(*this);
      }
      karma::generate(buffer_,karma::char_, '}');
    }
    
    void apply(vector_type const& v) {
      karma::generate(buffer_,karma::char_, '[');  
      bool first = true;
      for(Container const& element : v) {
        if (!first) karma::generate(buffer_,karma::char_, ',');
        first = false;
        element.const_visit(*this);
      }
      karma::generate(buffer_,karma::char_, ']');  
    }
    
    void apply(string_type const& v) { 
      //karma::generate(buffer_,karma::char_('"') << karma::string(v) << karma::char_('"'));  
      karma::generate(buffer_,karma::char_ << karma::string << karma::char_, '"', v, '"');  
    }
    void apply(float_type v) { 
      karma::generate(buffer_, float_generator_, v);
    }
    void apply(int_type v) { 
      karma::generate(buffer_, int_generator_, v);
    }
    void apply(uint_type v) { 
      karma::generate(buffer_, uint_generator_, v);
    }
    void apply(bool v) { 
      if (v)
        karma::generate(buffer_,karma::lit("true"));  
      else
        karma::generate(buffer_,karma::lit("false"));  
    }
  };
  // Private visitors structures
  struct visitor_ostream {
    ostream_type output_stream;
    
    void apply(std::nullptr_t) { output_stream << "null"; }
    
    void apply(map_type const& v) {
      output_stream << "{";
      bool first = true;
      for(auto const& element : v) {
        if (!first) output_stream << ",";
        first = false;
        output_stream << "\"" << element.first << "\":";
        element.second.const_visit(*this);
      }
      output_stream << "}";
    }
    
    void apply(vector_type const& v) {
      output_stream << "[";
      bool first = true;
      for(Container const& element : v) {
        if (!first) output_stream << ",";
        first = false;
        element.const_visit(*this);
      }
      output_stream << "]";
    }
    
    void apply(string_type const& v) { output_stream << "\"" << v << "\""; }
    void apply(float_type v) { output_stream << v; }
    void apply(int_type v) { output_stream << v; }
    void apply(uint_type v) { output_stream << v; }
    void apply(bool v) { output_stream << (v?"true":"false"); }

    StreamType render() const { return output_stream.str(); }
  };

  mutable visitor_size size_calculator_;
  mutable visitor_karma printer_;

 public:
  ~serializer_impl() {}

  StreamType serialize(Container const& input) const override {
    // Size
    size_calculator_.size = 0u;
    input.const_visit(size_calculator_);

    // Rendering
    printer_.buffer_ = reinterpret_cast<char*>(std::malloc(sizeof(typename string_type::value_type)*size_calculator_.size + 1u));
    printer_.init_buffer_ = printer_.buffer_;
    input.const_visit(printer_);
    std::string result(printer_.init_buffer_, printer_.buffer_ - printer_.init_buffer_);
    //std::free(printer_.init_buffer_);
    return result;

    //visitor_ostream visitor_;
    //input.const_visit(visitor_);
    //return visitor_.render();
  };

  Container deserialize(StreamType const& input) const override {
    Container result;
    typename string_type::const_iterator iter = input.begin();
    typename string_type::const_iterator end = input.end();
    bool success = qi::phrase_parse(iter, end, grammar_, boost::spirit::ascii::space, result);
    //bool success = qi::phrase_parse(iter, end, grammar_, boost::spirit::ascii::space);
    std::cout << "Parsing " << (success ? "succeeded":"failed") << std::endl;
    return result;
  };
};

template<typename Container, typename StreamType> serializer<Container, StreamType>::serializer() 
  : serializer_impl_(new serializer_impl<Container, StreamType>)
{}
template<typename Container, typename StreamType> StreamType serializer<Container, StreamType>::serialize(Container const& input) const {
  return serializer_impl_->serialize(input); 
}
template<typename Container, typename StreamType> Container serializer<Container, StreamType>::deserialize(StreamType const& input) const {
  return serializer_impl_->deserialize(input); 
}

}  // namespace json
}  // namespace nested_container

#endif  // __NESTED_COMPILER_JSON__
