#ifndef HEADER_GUARD_NESTED_COMPILER_JSON_FORWARD
#define HEADER_GUARD_NESTED_COMPILER_JSON_FORWARD
#include <memory>

namespace json_backbone {
namespace json {

enum class generation_policies : unsigned char {
  visitor_ostream,
  visitor_partial_karma,
  full_karma
};

enum class parsing_policies : unsigned char { full_spirit, partial_spirit };

template <typename Container, typename StreamType,
          generation_policies GenPolicy, parsing_policies ParsePolicy>
struct serializer_impl_envelop {
  static generation_policies constexpr generation_policy = GenPolicy;
  static parsing_policies constexpr parsing_policy = ParsePolicy;

  virtual ~serializer_impl_envelop();
  virtual StreamType serialize(Container const&) const = 0;
  virtual Container deserialize(StreamType const&) const = 0;
};

template <typename Container, typename StreamType,
          generation_policies GenPolicy>
struct serializer_impl_envelop<Container, StreamType, GenPolicy,
                               parsing_policies::partial_spirit> {
  static generation_policies constexpr generation_policy = GenPolicy;
  static parsing_policies constexpr parsing_policy =
      parsing_policies::partial_spirit;

  virtual ~serializer_impl_envelop();
  virtual StreamType serialize(Container const&) const = 0;
  virtual Container deserialize(StreamType const&,
                                size_t vector_reserve = 0u) const = 0;
};

template <
    typename Container, typename StreamType = typename Container::str_type,
    generation_policies GenPolicy = generation_policies::visitor_partial_karma,
    parsing_policies ParsePolicy = parsing_policies::full_spirit>
class serializer {
  static generation_policies constexpr generation_policy = GenPolicy;
  static parsing_policies constexpr parsing_policy = ParsePolicy;
  std::unique_ptr<serializer_impl_envelop<Container, StreamType,
                                          generation_policy, parsing_policy>>
      serializer_impl_;

 public:
  serializer();
  StreamType serialize(Container const& container) const;
  Container deserialize(StreamType const& input) const;
};

// We specialize it for partial spirit to add some features such as vector pre
// reserve
template <typename Container, typename StreamType,
          generation_policies GenPolicy>
class serializer<Container, StreamType, GenPolicy,
                 parsing_policies::partial_spirit> {
  static generation_policies constexpr generation_policy = GenPolicy;
  static parsing_policies constexpr parsing_policy =
      parsing_policies::partial_spirit;
  std::unique_ptr<serializer_impl_envelop<Container, StreamType,
                                          generation_policy, parsing_policy>>
      serializer_impl_;

 public:
  serializer();
  StreamType serialize(Container const& container) const;
  Container deserialize(StreamType const& input,
                        size_t vector_reserve = 0u) const;
};
} // namespace json
} // namespace json_backbone

#endif // __NESTED_COMPILER_JSON_FORWARD__
