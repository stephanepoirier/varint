/**
 * @file LazyAndSetTests.cpp
 *
 * Tests for the LazyAndSet class.
 */

 #include "varint/varint.hpp"

#include "gtest/gtest.h"

/**
 * @test General LazyAndSet test.
 */
TEST(LazyAndSetTests, Initialization) {
  LazyAndSet las1;
  ASSERT_TRUE(las1.size() == 0);
}
