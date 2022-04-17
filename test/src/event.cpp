#include "event.hpp"
#include "doctest/doctest.h"

using namespace properties;

TEST_CASE("Event without callback") {
  event<int &> e;
  int v = 8;

  e.trigger(v);
  CHECK_EQ(v, 8);
}

TEST_CASE("Simple event") {
  event<int> e;

  int calls = 0;
  auto callback = [&calls](int v) {
    calls++;
    CHECK_EQ(v, 8);
  };

  e + callback;
  e.trigger(8);
  CHECK_EQ(calls, 1);
}

void callback3(int &v) {
  CHECK_EQ(v, 18);
  v = 12;
}

struct callable {
  callable(int &counter) : counter{counter} {}

  void operator()(int &v) {
    CHECK_EQ(v, 19);
    v = 18;
    counter++;
  }

  int &counter;
};

TEST_CASE("Event with multiple types of callbacks") {
  event<int &> e;
  int val = 8;
  int calls = 0;

  auto callback1 = [&calls](int &v) {
    CHECK_EQ(v, 8);
    calls++;
    v = 19;
  };

  callable callback2(calls);

  e + callback1;
  e + callback2;
  e + callback3;

  e.trigger(val);

  CHECK_EQ(calls, 2);
  CHECK_EQ(val, 12);
}
