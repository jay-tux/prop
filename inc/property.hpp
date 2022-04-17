#ifndef _PROP_PROPERTY
#define _PROP_PROPERTY

#include "event.hpp"

namespace properties {
template <typename T, bool copy = false> struct property {
public:
  property(T &ref) : ref{ref} {}
  property(T &&) = delete;
  template <bool _copy> property(const property<T, _copy> &) = delete;
  template <bool _copy>
  property(property<T, _copy> &&prop) : ref{std::move(prop.ref)} {}

  T &get() const { return ref; }
  T &get() { return ref; }

  void set(const T &value) {
    ref = value;
    _set.trigger(ref);
  }

  template <bool _copy> void set(const property<T, _copy> &value) {
    ref = value.get();
    _set.trigger(ref);
  }

  operator T() const { return ref; }
  operator T &() const { return ref; }
  operator T() { return ref; }
  operator T &() { return ref; }

  template <bool _copy>
  property<T, copy> &operator=(const property<T, _copy> &other) {
    if constexpr (!_copy) {
      if (this != &other)
        return *this;
    }
    ref = (const T &)other;
    _set.trigger(ref);
    return *this;
  }

  template <bool _copy>
  property<T, copy> &operator=(const property<T, _copy> &&other) {
    ref = std::move(other.ref);
    _set.trigger(ref);
    return *this;
  }

  T &operator=(T val) {
    ref = val;
    _set.trigger(ref);
    return ref;
  }
  T &operator=(T &val) {
    ref = val;
    _set.trigger(ref);
    return ref;
  }

  void operator+(typename event<T &>::Callable callback) { _set + callback; }

  ~property() = default;

private:
  T &ref;
  event<T &> _set;
};

template <typename T> struct property<T, true> {
public:
  property(T val) : val{val} {}

  T &get() const { return val; }
  T &get() { return val; }

  void set(const T &value) {
    val = value;
    _set.trigger(val);
  }
  template <bool _copy> void set(const property<T, _copy> &value) {
    val = value.get();
    _set.trigger(val);
  }

  template <bool _copy>
  property<T, true> &operator=(const property<T, _copy> &other) {
    if (this == &other)
      return *this;
    val = other.val;
    _set.trigger(val);
    return *this;
  }

  template <bool _copy>
  property<T, true> &operator=(const property<T, _copy> &&other) {
    val = std::move(other.val);
    _set.trigger(val);
    return *this;
  }

  operator const T &() const { return val; }
  operator const T &() { return val; }

  T &operator=(T other) {
    val = other;
    _set.trigger(val);
    return val;
  }
  T &operator=(T &other) {
    val = other;
    _set.trigger(val);
    return val;
  }

  void operator+(typename event<T &>::Callable callback) { _set + callback; }

  ~property() = default;

private:
  T val;
  event<T &> _set;
};
} // namespace properties

#endif /* _PROP_PROPERTY */
