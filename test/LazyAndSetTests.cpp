#include "varint/varint.hpp"

#include "gtest/gtest.h"

TEST(LazyAndSetTests, Initialization) {
  LazyAndSet las1;
  ASSERT_TRUE(las1.size() == 0);
}
