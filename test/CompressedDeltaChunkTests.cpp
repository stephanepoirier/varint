/**
 * @file CompressedDeltaChunkTests.cpp
 *
 * Tests for the CompressedDeltaChunk class.
 */

 #include "varint/varint.hpp"

#include "gtest/gtest.h"

/**
 * @test General CompressedDeltaChunk test.
 */
TEST(CompressedDeltaChunkTests, Initialization) {
  CompressedDeltaChunk cdc1;
  ASSERT_TRUE(cdc1.getCompressedSize() == 0);
}
