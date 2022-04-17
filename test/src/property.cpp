#include "property.hpp"
#include "doctest/doctest.h"

using namespace properties;

TEST_CASE("Property getters") {
  int val;
  property<int> p(val);

  CHECK(p.operator int() == val);
  CHECK(p.operator int &() == val);
  CHECK(p.get() == val);

  const property<int> pc(val);

  CHECK(p.operator int() == val);
  CHECK(p.operator int &() == val);
  CHECK(p.get() == val);
}

TEST_CASE("Property setters") {
  int val = 12;
  property<int> p(val);

  p.set(9);
  CHECK(val == 9);
  CHECK(p.get() == 9);

  p = 126;
  CHECK(val == 126);
  CHECK(p.get() == 126);
}

TEST_CASE("Property setters (copy)") {
  int val = 12;
  property<int, true> p(val);

  p.set(234);
  CHECK(p.get() == 234);
  CHECK(val == 12);

  p.set(189);
  CHECK(p.get() == 189);
  CHECK(val == 12);
}

TEST_CASE("Events") {
  int val = 12;
  property<int> p(val);
  int calls = 0;

  auto callback = [&calls](int &v) { calls++; };
  p + callback;

  p = 14;
  CHECK_EQ(calls, 1);

  p.set(123);
  CHECK_EQ(calls, 2);

  property<int, true> p2(12);
  p = p2;
  CHECK_EQ(calls, 3);
}
