#include "varint/varint.hpp"

#include "gtest/gtest.h"

TEST(CompressedDeltaChunkTests, Initialization) {
  CompressedDeltaChunk cdc1;
  ASSERT_TRUE(cdc1.getCompressedSize() == 0);
}
