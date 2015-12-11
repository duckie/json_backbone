#pragma once
#include <array>
#include <stack>
#include <iterator>
#include <functional>
#include <rapidjson/reader.h>

namespace json_backbone {
namespace extensions {
namespace rapidjson {

template <class RootType, class Encoding> class reader_handler;

template <class RootType, class Encoding>
class reader_handler : public ::rapidjson::BaseReaderHandler<
                           Encoding,
                           reader_handler<RootType, Encoding>> {
  enum class State {
    empty,
    wait_start_object,
    wait_key,
    wait_value,
    wait_end_object,
    wait_start_sequence,
    wait_element,
    wait_end_sequence
  };

  using Ch = typename Encoding::Ch;
  using SizeType = ::rapidjson::SizeType;
  using StdString = typename RootType::str_type;

  RootType& root_;
  std::stack<std::reference_wrapper<RootType>> nodes_;
  State state_;
  std::string current_key_;

 public:
  reader_handler(RootType& root)
      : ::rapidjson::BaseReaderHandler<Encoding, reader_handler>()
      , root_(root)
      , state_(State::empty)
      , current_key_()
   {}

  bool Null() {
    if (State::wait_value == state_ && nodes_.top().get().is_map()) {
      nodes_.top().get()[current_key_].transform_null();
      state_ = State::wait_key;
      return true;
    } else if (State::wait_element == state_ && nodes_.top().get().is_vector()) {
      nodes_.top().get().ref_vector().push_back(nullptr);
      return true;
    }
    std::cout << "Failure " << static_cast<unsigned>(state_) << " " << __FUNCTION__ << " " << __LINE__ << std::endl;
    return false;
  }

  bool Bool(bool value) {
    if (State::wait_value == state_ && nodes_.top().get().is_map()) {
      nodes_.top().get()[current_key_].transform_bool() = static_cast<bool>(value);
      state_ = State::wait_key;
      return true;
    } else if (State::wait_element == state_ && nodes_.top().get().is_vector()) {
      nodes_.top().get().ref_vector().push_back(static_cast<bool>(value));
      return true;
    }
    std::cout << "Failure " << static_cast<unsigned>(state_) << " " << __FUNCTION__ << " " << __LINE__ << std::endl;
    return false;
  }

  bool Int(int value) {
    if (State::wait_value == state_ && nodes_.top().get().is_map()) {
      nodes_.top().get()[current_key_].transform_int() = static_cast<typename RootType::int_type>(value);
      state_ = State::wait_key;
      return true;
    } else if (State::wait_element == state_ && nodes_.top().get().is_vector()) {
      nodes_.top().get().ref_vector().push_back(static_cast<typename RootType::int_type>(value));
      return true;
    }
    std::cout << "Failure " << static_cast<unsigned>(state_) << " " << __FUNCTION__ << " " << __LINE__ << std::endl;
    return false;
  }

  bool Uint(unsigned value) {
    if (State::wait_value == state_ && nodes_.top().get().is_map()) {
      nodes_.top().get()[current_key_].transform_uint() = static_cast<typename RootType::uint_type>(value);
      state_ = State::wait_key;
      return true;
    } else if (State::wait_element == state_ && nodes_.top().get().is_vector()) {
      nodes_.top().get().ref_vector().push_back(static_cast<typename RootType::uint_type>(value));
      return true;
    }
    std::cout << "Failure " << static_cast<unsigned>(state_) << " " << __FUNCTION__ << " " << __LINE__ << std::endl;
    return false;
  }

  bool Int64(int64_t value) {
    if (State::wait_value == state_ && nodes_.top().get().is_map()) {
      nodes_.top().get()[current_key_].transform_int() = static_cast<typename RootType::int_type>(value);
      state_ = State::wait_key;
      return true;
    } else if (State::wait_element == state_ && nodes_.top().get().is_vector()) {
      nodes_.top().get().ref_vector().push_back(static_cast<typename RootType::int_type>(value));
      return true;
    }
    std::cout << "Failure " << static_cast<unsigned>(state_) << " " << __FUNCTION__ << " " << __LINE__ << std::endl;
    return false;
  }

  bool Uint64(uint64_t value) {
    if (State::wait_value == state_ && nodes_.top().get().is_map()) {
      nodes_.top().get()[current_key_].transform_uint() = static_cast<typename RootType::uint_type>(value);
      state_ = State::wait_key;
      return true;
    } else if (State::wait_element == state_ && nodes_.top().get().is_vector()) {
      nodes_.top().get().ref_vector().push_back(static_cast<typename RootType::uint_type>(value));
      return true;
    }
    std::cout << "Failure " << static_cast<unsigned>(state_) << " " << __FUNCTION__ << " " << __LINE__ << std::endl;
    return false;
  }

  bool Double(double value) {
    if (State::wait_value == state_ && nodes_.top().get().is_map()) {
      nodes_.top().get()[current_key_].transform_float() = static_cast<typename RootType::float_type>(value);
      state_ = State::wait_key;
      return true;
    } else if (State::wait_element == state_ && nodes_.top().get().is_vector()) {
      nodes_.top().get().ref_vector().push_back(static_cast<typename RootType::float_type>(value));
      return true;
    }
    std::cout << "Failure " << static_cast<unsigned>(state_) << " " << __FUNCTION__ << " " << __LINE__ << std::endl;
    return false;
  }

  bool String(const Ch* data, SizeType length, bool) {
    if (State::wait_value == state_ && nodes_.top().get().is_map()) {
      nodes_.top().get()[current_key_].transform_string() = StdString(data, length);
      state_ = State::wait_key;
      return true;
    } else if (State::wait_element == state_ && nodes_.top().get().is_vector()) {
      nodes_.top().get().ref_vector().push_back(StdString(data, length));
      return true;
    }
    std::cout << "Failure " << static_cast<unsigned>(state_) << " " << __FUNCTION__ << " " << __LINE__ << std::endl;
    return false;
  }

  bool StartObject() {
    if (State::empty != state_ && State::wait_start_object != state_ && State::wait_value != state_ &&
        State::wait_element != state_)
      return false;


    bool object_created = false;
    if (nodes_.empty()) {
      root_.transform_map();
      nodes_.emplace(root_);
      object_created = true;
    } else if (nodes_.top().get().is_map()) {
      nodes_.top().get()[current_key_].transform_map();
      nodes_.emplace(nodes_.top().get()[current_key_]);
      object_created = true;
    } else if (nodes_.top().get().is_vector()) {
      //nodes_.top().get().ref_vector().push_back(typename RootType::map_type());
      nodes_.top().get().ref_vector().push_back(nullptr);
      nodes_.top().get().ref_vector().back().transform_map();
      nodes_.emplace(nodes_.top().get().ref_vector().back());
      object_created = true;
    }

    if (object_created) {
      state_ = State::wait_key;
      return true;
    }

    std::cout << "Failure " << static_cast<unsigned>(state_) << " " << __FUNCTION__ << " " << __LINE__ << std::endl;
    return false;
  }

  bool Key(const Ch* str, SizeType len, bool copy) {
    if (state_ != State::wait_key)
      return false;

    current_key_ = std::string(str);
    state_ = State::wait_value;
    return true;
  }

  bool EndObject(SizeType) {
    if (State::wait_key != state_ && State::wait_end_object != state_)
      return false;
    nodes_.pop();
    if (!nodes_.empty()) {
      state_ = nodes_.top().get().is_map() ? State::wait_key : State::wait_element;
    }
    else {
      //state_ = State::empty;
      //current_key_.clear();
    }
    return true;
  }

  bool StartArray() {
    if (State::empty != state_ && State::wait_start_sequence != state_ && State::wait_value != state_ &&
        State::wait_element != state_)
      return false;

    bool object_created = false;
    if (nodes_.empty()) {
      root_.transform_vector();
      nodes_.emplace(root_);
      object_created = true;
    } else if (nodes_.top().get().is_map()) {
      nodes_.top().get()[current_key_].transform_vector();
      nodes_.emplace(nodes_.top().get()[current_key_]);
      object_created = true;
    } else if (nodes_.top().get().is_vector()) {
      //nodes_.top().get().ref_vector().push_back(typename RootType::vector_type());
      nodes_.top().get().ref_vector().push_back(nullptr);
      nodes_.top().get().ref_vector().back().transform_vector();
      nodes_.emplace(nodes_.top().get().ref_vector().back());
      object_created = true;
    }

    if (object_created ) {
      state_ = State::wait_element;
      return true;
    }

    std::cout << "Failure " << static_cast<unsigned>(state_) << " " << __FUNCTION__ << " " << __LINE__ << std::endl;
    return false;
  }

  bool EndArray(SizeType) {
    if (State::wait_element != state_ && State::wait_end_sequence != state_)
      return false;
    nodes_.pop();
    if (!nodes_.empty()) {
      state_ = nodes_.top().get().is_map() ? State::wait_key : State::wait_element;
    }
    else {
      //state_ = State::empty;
      //current_key_.clear();
    }
    return true;
  }
};

template <class RootType>
reader_handler<RootType, ::rapidjson::UTF8<>>
make_reader_handler(RootType& root) {
  return reader_handler<RootType, ::rapidjson::UTF8<>>(root);
}

template <class Encoding, class RootType>
reader_handler<RootType, Encoding> make_reader_handler(RootType& root) {
  return reader_handler<RootType, Encoding>(root);
}

} // namespace rapidjson
} // namespace extensions
} // namespace named_types
