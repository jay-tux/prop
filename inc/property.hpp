#ifndef _PROP_PROPERTY
#define _PROP_PROPERTY

#include "event.hpp"

/**
 * @brief Main namespace for the properties library.
 */
namespace properties {
/**
 *  @brief Property type.
 *  @details This type holds a (reference to) a single value, with getters and
 * setters. Whenever the value is modified, the property will trigger the
 * property change event. You can add callbacks which will trigger when the
 * value is updated using the `operator +(callback)`.
 *
 *  @tparam T The type of the value.
 *  @tparam copy Whether or not the value is copied (false means that the
 * value is a reference to the original value).
 */
template <typename T, bool copy = false> struct property {
public:
  /**
   * @brief Creates a new reference property from a reference.
   * @details The passed reference is kept and modifications are passed down to
   * the original reference.
   *
   *  @param ref The reference to the object.
   */
  property(T &ref) : ref{ref} {}
  /**
   * @brief You can't create a reference property from an rvalue-reference.
   */
  property(T &&) = delete;
  /**
   *  @brief You can't create a reference property from another property.
   */
  template <bool _copy> property(const property<T, _copy> &) = delete;
  /**
   *  @brief Creates a new reference property by moving the data from another
   * property.
   *  @details The reference in the other property is moved to this property.
   * Using the other property after this call is undefined behavior.
   *
   *  @param prop The other property.
   */
  property(property<T, false> &&prop) : ref{std::move(prop.ref)} {}

  /**
   *  @brief Extracts the reference from this property.
   *  @return A constant reference to the value.
   */
  const T &get() const { return ref; }
  /**
   *  @brief Extracts the reference from this property.
   *  @details Modifying the reference will never trigger the event.
   *  @return A reference to the value.
   */
  T &get() { return ref; }

  /**
   *  @brief Sets the value of this property.
   *  @details This function copies the value in the reference to this property,
   * then triggers the event.
   *
   *  @param value The new value.
   */
  void set(const T &value) {
    ref = value;
    _set.trigger(ref);
  }

  /**
   *  @brief Sets the value of this property.
   *  @details This function copies the value in the other property to this
   * property, then triggers the event.
   *
   *  @tparam _copy The copy state of the other property.
   *  @param value The property holding the new value.
   */
  template <bool _copy> void set(const property<T, _copy> &value) {
    ref = value.get();
    _set.trigger(ref);
  }

  /**
   *  @brief Gets the value of this property (by-const).
   *  @return The value.
   */
  operator T() const { return ref; }
  /**
   *  @brief Gets the value of this property (by const ref).
   *  @return A const reference to the value.
   */
  operator T &() const { return ref; }
  /**
   *  @brief Gets the value of this property (by ref).
   *  @details Modifying this value will not trigger the event.
   *  @return A non-const reference to the value.
   */
  operator T &() { return ref; }

  /**
   *  @brief Copies the data from the other property into this property.
   *  @details The value is copied into the contained reference, which
   * overwrites any value present in it. After the copy is complete, the event
   * is triggered.
   *
   *  @tparam _copy The copy state of the other property.
   *  @param other The other property.
   *  @return A reference to this property.
   */
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

  /**
   *  @brief Moves the data from the other property into this property.
   *  @details The value is moved into the contained reference, which
   * overwrites any value present in it. After the copy is complete, the event
   * is triggered. Using the other property after this operation is undefined
   * behavior.
   *
   *  @tparam _copy The copy state of the other property.
   *  @param other The other property.
   *  @return A reference to this property.
   */
  template <bool _copy>
  property<T, copy> &operator=(const property<T, _copy> &&other) {
    ref = std::move(other.ref);
    _set.trigger(ref);
    return *this;
  }

  /**
   *  @brief Updates the value of this property, then triggers the event.
   *  @details The value is updated. The event will only see the new value (by
   * reference).
   *
   *  @param val The new value for this property.
   *  @return The value this property holds.
   */
  T operator=(T val) {
    ref = val;
    _set.trigger(ref);
    return ref;
  }
  /**
   *  @brief Updates the value of this property, then triggers the event.
   *  @details The value is updated. The event will only see the new value (by
   * reference).
   *
   *  @param val The new value for this property.
   *  @return The value this property holds.
   */
  T operator=(T &val) {
    ref = val;
    _set.trigger(ref);
    return ref;
  }

  /**
   *  @brief Adds a callback to the event.
   *  @details The callback is passed through to the event's `operator +`.
   * Whenever the value in this property is modified, the callback, the callback
   * will be called with a reference to the value. Modifying that value will
   * never trigger the event.
   *
   * @param callback The callback to add to the event.
   */
  void operator+(typename event<T &>::Callable callback) { _set + callback; }

  /**
   *  @brief Destroys the property.
   */
  ~property() = default;

private:
  T &ref;
  event<T &> _set;
};

/**
 *  @brief Property type.
 *  @details This partial specification describes a property whose value is
 * contained entirely withing the property (no reference to an outside object is
 * kept).
 *
 *  @tparam T The type of the value.
 */
template <typename T> struct property<T, true> {
public:
  /**
   *  @brief Constructs and initializes a property.
   *  @details This constructor copies the given value into the property.
   *
   *  @param val The new value for this property.
   */
  property(T val) : val{val} {}

  /**
   *  @brief Extracts the reference from this property.
   *  @return A constant reference to the value.
   */
  const T &get() const { return val; }
  /**
   *  @brief Extracts the reference from this property.
   *  @details Modifying the reference will never trigger the event.
   *  @return A reference to the value.
   */
  T &get() { return val; }

  /**
   *  @brief Sets the value of this property.
   *  @details This function copies the value in the reference to this property,
   * then triggers the event.
   *
   *  @param value The new value.
   */
  void set(const T &value) {
    val = value;
    _set.trigger(val);
  }

  /**
   *  @brief Sets the value of this property.
   *  @details This function copies the value in the other property to this
   * property, then triggers the event.
   *
   *  @tparam _copy The copy state of the other property.
   *  @param value The property holding the new value.
   */
  template <bool _copy> void set(const property<T, _copy> &value) {
    val = value.get();
    _set.trigger(val);
  }

  /**
   *  @brief Copies the data from the other property into this property.
   *  @details The value is copied into the contained reference, which
   * overwrites any value present in it. After the copy is complete, the event
   * is triggered.
   *
   *  @tparam _copy The copy state of the other property.
   *  @param other The other property.
   *  @return A reference to this property.
   */
  template <bool _copy>
  property<T, true> &operator=(const property<T, _copy> &other) {
    if (this == &other)
      return *this;
    val = other.val;
    _set.trigger(val);
    return *this;
  }

  /**
   *  @brief Moves the data from the other property into this property.
   *  @details The value is moved into the contained reference, which
   * overwrites any value present in it. After the copy is complete, the event
   * is triggered. Using the other property after this operation is undefined
   * behavior.
   *
   *  @tparam _copy The copy state of the other property.
   *  @param other The other property.
   *  @return A reference to this property.
   */
  template <bool _copy>
  property<T, true> &operator=(const property<T, _copy> &&other) {
    val = std::move(other.val);
    _set.trigger(val);
    return *this;
  }

  /**
   *  @brief Gets the value of this property (by const ref).
   *  @return A const reference to the value.
   */
  operator const T &() const { return val; }

  /**
   *  @brief Updates the value of this property, then triggers the event.
   *  @details The value is updated. The event will only see the new value (by
   * reference).
   *
   *  @param val The new value for this property.
   *  @return The value this property holds.
   */
  T &operator=(T other) {
    val = other;
    _set.trigger(val);
    return val;
  }
  /**
   *  @brief Updates the value of this property, then triggers the event.
   *  @details The value is updated. The event will only see the new value (by
   * reference).
   *
   *  @param val The new value for this property.
   *  @return The value this property holds.
   */
  T &operator=(T &other) {
    val = other;
    _set.trigger(val);
    return val;
  }

  /**
   *  @brief Adds a callback to the event.
   *  @details The callback is passed through to the event's `operator +`.
   * Whenever the value in this property is modified, the callback, the callback
   * will be called with a reference to the value. Modifying that value will
   * never trigger the event.
   *
   * @param callback The callback to add to the event.
   */
  void operator+(typename event<T &>::Callable callback) { _set + callback; }

  /**
   *  @brief Destroys the property.
   */
  ~property() = default;

private:
  T val;
  event<T &> _set;
};
} // namespace properties

#endif /* _PROP_PROPERTY */
