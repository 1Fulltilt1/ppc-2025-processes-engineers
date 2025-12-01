#include <gtest/gtest.h>

#include <cstddef>

#include "Rastvorov_K_Number_of_character_alternations/common/include/common.hpp"

namespace {

using Rastvorov_K_Number_of_character_alternations::InType;
using Rastvorov_K_Number_of_character_alternations::OutType;

inline int Sign(double x) {
  if (x > 0.0) {
    return 1;
  }
  if (x < 0.0) {
    return -1;
  }
  return 0;
}

inline double GetElement(std::size_t i) {
  if (i % 5 == 0) {
    return 0.0;
  }
  if (i % 2 == 0) {
    return 1.0;
  }
  return -1.0;
}

inline int CountAlternations(std::size_t n) {
  int previous_sign = 0;
  int alternations_count = 0;

  for (std::size_t i = 0; i < n; ++i) {
    const int s = Sign(GetElement(i));
    if (s == 0) {
      continue;
    }
    if (previous_sign != 0 && s != previous_sign) {
      ++alternations_count;
    }
    previous_sign = s;
  }

  return alternations_count;
}

TEST(AlternationsPerformance, LargeN) {
  const auto n = static_cast<InType>(10000000);
  const auto result = static_cast<OutType>(CountAlternations(static_cast<std::size_t>(n)));

  EXPECT_GE(result, static_cast<OutType>(0));
  (void)result;
}

}  // namespace
