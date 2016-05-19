Json backbone documentation
==================

`json_backbone` is a C++14 container to hold dynamically structured data. `json_backbone` is easy to use, generic and type safe.

`json_backbone` is made of three main parts:
* A versatile `variant` type optimized for small types.
* An helper to create recursive variants through a *Associative* container and a *RandomAccess* container.
* A view interface to loosely visit a structure without exceptions.

## `json_backbone::variant`

Most of `json_backbone` features are implemented through a discriminated union optimized for small types. This `variant`is similar to `boost::variant`but differs from it on a variety of topics.

The declaration is the same. Let's use and example :

```c++
using variant_t = json_backbone::variant<std::nullptr_t, int, std::string>;
```

The variant does not comply to the [current state of the standard](http://open-std.org/JTC1/SC22/WG21/docs/papers/2016/p0088r1.html) and is not supposed to.

Constraints on bounded types:
* Bounded types must be *CopyConstructible* and *MoveConstructible*.
* Complete at the point of the `variant`instantiation.

The `variant`:
* Does not have an empty state.
* is *DefaultConstructible* if at least one of the bounded types is default constructible.
* is *CopyConstructible* and *MoveConstructible*.
* is *Assignable* from every *Assignable* bounded types and every type convertible to a bounded type.
* is *MoveAssignable* from every *MoveAssignable* bounded types and every type convertible to a bounded type.
* is *EqualityComparable* if all bounded types are *EqualityComparable*.
* is *LessThanComparable* if all bounded types are *LessThanComparable*.
* is not *OutputStreamable*
* is not *Hashable*
* is explicitely convertible to any of its bounded types.
* is implicitly convertible from any of its bounded types.
* supports any constructor supported by one of its bounded types.

Rules to select assignable types and valid constructors will be detailed in a dedicated section.

### Small types optimizations

By default, the `variant`stores small types on the stack and other ones on the heap. A mix of small and big types may be used transparently.

By default, a small type is any type `T`verifying  ``sizeof(T) <= sizeof(void*)``.

This behavior can be customized by the user. Suppose we are using a type such that``sizeof(void*) < sizeof(BigType)`` and we define this variant:

```c++
using small_variant_t = variant<std::nullptr_t,BigType>;
```

In the context of this variant, any `BigType` value would not be considered a small type and would be allocated on the heap. You can override this behavior by specializing a template:

```c++
namespace json_backbone {
template <> struct is_small_type<BigType> { static constexpr bool value = true; }
}
```

Now, any variant using `BigType` will consider it a small type and allocate it on the stack. Moreover, any other type in those variants which size is less than or equal to `sizeof(BigType)` will be optimized as a small type too, even though it was not explicitely requested:

```c++
struct BigType {
  char data[128];
};

struct BigType2 {
  BigType data;
};

namespace json_backbone {
template <> struct is_small_type<BigType> {
  static constexpr bool value = true;
};
}

int main(void) {
  std::cout
    << sizeof(variant<BigType,BigType2>) // BigType2 values allocated on stack
    << " != "
    << sizeof(variant<BigType2>) // BigType2 values allocated on heap
    << std::endl;
}
```

### Construction

`variant`is *DefaultConstructible* if any of its bounded types is default constructible. If several of them are, the selected one is the first in the list. `variant` also supports any constructor supported by its bounded types. This allow you to write things like:

```c++
variant<std::string> value{"Roger"};
```

Instead of:

```c++
variant<std::string> value{std::string("Roger")};
```

Each of the following rules are tried in this order until on of them gives a result. If none of them applies, the `variant` does not support the constructor. Given a construction signature `T&& ...`:
* If there is a single argument and it is a reference (either l-value or r-value) to the same `variant` type, the copy or move constructor applies.
* If there is a single argument and it is a reference (either l-value or r-value) to a bounded type, the copy or move constructor of said type applies and is stored.
* If there is a single argument and it is a reference (either l-value or r-value) to an integral type, the first integral type of the list wide enough and with *the same* signature is used to store the value.
* If there is a single argument and it is a reference (either l-value or r-value) to an integral type, the first integral type of the list wide enough and with *any* signature is used to store the value.
* If there is a single argument and it is a reference (either l-value or r-value) to an arithmetic type, the first arityhmetic but not integral type of the list wide enough is used to store the value.
* If there is a single argument and it is a pointer or an array , the first non-integral type supporting construction over this pointer is used to store the value.
* If none of those rules applies, the first type encountered supporting the signature is used to store the value.

This allows you to use `std::nullptr_t` as the default type and not being bothered when it comes to integers or pointers.

```c++
using variant_t = variant<std::nullptr_t, double, int, std::string>;
variant_t v1{};          // stored as a std::nullptr_t
variant_t v2{short(1)};  // stored as int
variant_t v3{1.f};       // stored as double
variant_t v4{"Roger"};   // stored as std::string
// variant_t v5{23u};    // would not compile

std::cout << v1.is<std::nullptr_t>() << v2.is<int>() << v3.is<double>() << v4.is<std::string>()
          << std::endl;
```

### Access to data

Data can be accessed in a variety of ways:

```c++
std::string& s1 = v4.get<std::string>();  // Checks actual type stored, throws if does not match
auto s2 = get<std::string>(v4);           // Equivalent to previous call
auto s3 = v4.raw<std::string>();          // Access the data directly without any check
auto s4 = raw<std::string>(v4);           // Equivalent to previous call

std::string& s5 = static_cast<std::string&>(v4);

bool is_string = v4.is<std::string>();  // Check actual type
```

Other ways of investigating the actual type will be covered in the visiting part.

### Visiting

Data can be visited with a callable implementing one overload for each bounded type. An example:

```c++
struct Visitor {
  void operator()(int v) const {
    std::cout << v << "\n";
  }

  void operator()(std::string const& v) const {
    std::cout << v << "\n";
  }
};

int main(void) {
  using variant_t = variant<int, std::string>;
  variant_t t1{1};
  variant_t t2{"Roger"};
  Visitor v;
  apply_visitor<void>(t1, v);
  apply_visitor<void>(t2, v);
  return 0;
}
```

Visitors can take additional parameters and return a value :

```c++
struct Visitor {
  int operator()(int v,int start) const {
    return v + start;
  }

  int operator()(std::string const& v, int start) const {
    return v.size() + start;
  }
};

int main(void) {
  using variant_t = variant<int,std::string>;
  variant_t t1{1};
  variant_t t2{"Roger"};
  Visitor v;
  std::cout << apply_visitor<int>(t1, v, 0) << "\n";
  std::cout << apply_visitor<int>(t2, v, 1) << "\n";
}
```

Visitors can have a state.

```c++
struct Visitor {
  int sum = 0;
  void operator()(int v) {
    sum += v;
  }
};

int main(void) {
  using variant_t = variant<int>;
  variant_t t1{1};
  variant_t t2{2};
  Visitor v;
  apply_visitor<void>(t1, v);
  apply_visitor<void>(t2, v);
  std::cout << v.sum << "\n";
}
```

Visitors can be defined close to where they are used by aggregating functions.

```c++
using variant_t = variant<int,std::string>;
variant_t t1{1};
variant_t t2{"Roger"};
static const_funcptr_aggregate_visitor<bool, variant_t> is_string {
  [](auto) { return false; },
  [](auto) { return true; }
};
std::cout << apply_visitor<bool>(t1, is_string) << "\n";
std::cout << apply_visitor<bool>(t2, is_string) << "\n";
```

`const_func_aggregate_visitor` can be used instead of `const_funcptr_aggregate_visitor` if you want to resolve to a `std::function` callable rather than a function pointer. Beware of the performance cost. Non-const versions `func_aggregate_visitor` and `funcptr_aggregate_visitor` are also available.

*Tip*: Resolving generic lambdas to function pointers will only work if said lambdas and function pointers do not return `void`.

*Tip*: Prefixing a lambda with the `+` symbol to force casting to a function pointer is not supported in MSVC.

### Recursive variant

As in `boost::variant`, you can define recursive variants.

```c++
namespace {
struct add;
struct sub;
template <typename OpTag>
struct binary_op;

using expression =
    variant<int, recursive_wrapper<binary_op<add>>, recursive_wrapper<binary_op<sub>>>;

template <typename OpTag>
struct binary_op {
  expression left;
  expression right;

  binary_op(const expression& lhs, const expression& rhs) : left(lhs), right(rhs) {}
};
}
```

## `json_backbone::container`


A `container` is a recursive `variant` aggregating an *Associative* container of (pointers to) itself, a *RandomAccess* container of (pointers to) itself, and any set of other bounded types. It is meant to ease representing data structure similar to `json`. Along with integrated

