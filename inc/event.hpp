#ifndef _PROP_EVENT
#define _PROP_EVENT

#include <functional>
#include <type_traits>
#include <vector>

namespace properties {
template <typename Out> struct event {
public:
  using Callable = std::function<void(Out)>;
  void trigger(Out val) {
    for (auto &listener : listeners) {
      listener(val);
    }
  }

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
