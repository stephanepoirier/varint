#include "varint/varint.hpp"

#include "gtest/gtest.h"

TEST(BasicSetTests, Initialization) {
  BasicSet bs1;
  ASSERT_TRUE(bs1.size() == 0);
}
