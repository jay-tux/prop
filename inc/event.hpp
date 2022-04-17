#ifndef _PROP_EVENT
#define _PROP_EVENT

#include <functional>
#include <type_traits>
#include <vector>

/**
 * @brief Main namespace for the properties library.
 */
namespace properties {
/**
 *  @brief Event type.
 *  @details This type holds a set of callbacks that will be called when the
 * event is triggered. The callbacks will be called in the order they were
 * registered (added). Callbacks are of the signature `void(Out)`.
 *
 *  @tparam Out The type of value that will be passed to the callbacks.
 */
template <typename Out> struct event {
public:
  /**
   *  @brief The callable type is an alias for `std::function<void(Out)>`.
   */
  using Callable = std::function<void(Out)>;
  /**
   *  @brief Triggers the event.
   *  @details Each of the callbacks will be called in the order they were
   * registered with the proved value as parameter. If the parameter is a
   * reference or pointer, the value it refers/points to can be changed by the
   * callbacks.
   *
   *  @param val The value to pass to the callbacks.
   */
  void trigger(Out val) {
    for (auto &listener : listeners) {
      listener(val);
    }
  }

  /**
   *  @brief Registers a callback.
   *  @details The callback is moved to the storage of the event. Calling the
   * callback after this call is undefined behavior.
   *
   *  @tparam Call The type of the callback. This type should be convertible to
   * the `event<Out>::Callback` type.
   *  @param other The callback. It will be moved from.
   */
  template <typename Call,
            typename _ = typename std::enable_if<
                std::is_convertible<Call, Callable>::value>::type>
  void operator+(Call &other) {
    listeners.emplace_back(std::move(other));
  }

private:
  std::vector<Callable> listeners;
};
} // namespace properties

#endif /* _PROP_EVENT */
