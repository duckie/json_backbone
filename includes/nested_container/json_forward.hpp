#ifndef __NESTED_COMPILER_JSON_FORWARD__
#define __NESTED_COMPILER_JSON_FORWARD__
#include <memory>

namespace nested_container {
namespace json {

template<typename Container, typename StreamType> struct serializer_impl_envelop {
  virtual ~serializer_impl_envelop();
  virtual StreamType serialize(Container const&) const = 0;
  virtual Container deserialize(StreamType const&) const = 0;
};

template<typename Container, typename StreamType = typename Container::str_type> class serializer {
  std::unique_ptr<serializer_impl_envelop<Container, StreamType>> serializer_impl_;
 public:
  serializer();
  StreamType serialize(Container const& container) const;
  Container deserialize(StreamType const& input) const;
};

}  // namespace json
}  // namespace nested_container

#endif  // __NESTED_COMPILER_JSON_FORWARD__
