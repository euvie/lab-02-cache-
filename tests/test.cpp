// Copyright 2020 Your Name <your_email>

// Copyright 2018 Your Name <your_email>
#include "tester.hpp"
#include <gtest/gtest.h>



TEST(Example, EmptyTest) {
  EXPECT_TRUE(true);
}

TEST(Ex, EmptyTest) {
  struct Cache{
    int L1 = 64;
    int L3 = 16384;//= 16 Mb
  };
  Cache cache;

  Tester tester(cache.L1, cache.L3);
  tester.define_series();
  tester.experiment();
  EXPECT_TRUE(true);
}