/**
 * @file CompressedSetTests.cpp
 *
 * Tests for the CompressedSet class.
 */

#include "varint/varint.hpp"

#include "gtest/gtest.h"

#include <set>
#include <iostream>

void testvec(std::set<unsigned int>& data) {
  std::stringstream ss;

  CompressedSet myset2;
  for (auto i : data) {
    myset2.addDoc(i);
  }
  myset2.compact();
  myset2.write(ss);

  CompressedSet myset1;
  myset1.read(ss);

  ASSERT_TRUE(data.size() == myset1.size());
  CompressedSet::Iterator it2(&myset1);
  for (auto idx : data) {
    ASSERT_TRUE(it2.nextDoc() != NO_MORE_DOCS);
    ASSERT_TRUE(it2.docID() == idx);
  }
  ASSERT_TRUE(it2.nextDoc() == NO_MORE_DOCS);
}

/**
 * @test General CompressedSet test.
 */
TEST(CompressedSetTests, GeneralTest) {
  for (uint32_t b = 0; b <= 28; ++b) {
    std::cout << "testing1... b = " << b << std::endl;
    for (size_t length = 128; length < (1U << 12); length += 128) {
      std::set<unsigned int> data;
      for (size_t i = 0; i < length; ++i) {
        unsigned int x = (i + (24 - i) * (12 - i)) % (1U << b);
        data.insert(x);
      }
      testvec(data);
    }

    std::cout << "testing2... b = " << b << std::endl;
    for (size_t length = 1; length < (1U << 9); ++length) {
      std::set<unsigned int> data;
      for (size_t i = 0; i < length; ++i) {
        data.insert((33231 - i + i * i) % (1U << b));
      }
      testvec(data);
    }
  }
}
