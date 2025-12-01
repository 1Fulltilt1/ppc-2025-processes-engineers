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

TEST(AlternationsFunctional, NIsZero) {
  const auto n = static_cast<InType>(0);
  const OutType expected = 0;

  const auto actual = static_cast<OutType>(CountAlternations(static_cast<std::size_t>(n)));

  EXPECT_EQ(actual, expected);
}

TEST(AlternationsFunctional, SingleElement) {
  const auto n = static_cast<InType>(1);
  const OutType expected = 0;
  const auto actual = static_cast<OutType>(CountAlternations(static_cast<std::size_t>(n)));

  EXPECT_EQ(actual, expected);
}

TEST(AlternationsFunctional, FiveElements) {
  const auto n = static_cast<InType>(5);
  const OutType expected = 3;

  const auto actual = static_cast<OutType>(CountAlternations(static_cast<std::size_t>(n)));

  EXPECT_EQ(actual, expected);
}

TEST(AlternationsFunctional, TenElements) {
  const auto n = static_cast<InType>(10);
  const OutType expected = 6;

  const auto actual = static_cast<OutType>(CountAlternations(static_cast<std::size_t>(n)));

  EXPECT_EQ(actual, expected);
}

TEST(AlternationsFunctional, TwentyFiveElements) {
  const auto n = static_cast<InType>(25);
  const OutType expected = 15;

  const auto actual = static_cast<OutType>(CountAlternations(static_cast<std::size_t>(n)));

  EXPECT_EQ(actual, expected);
}

}  // namespace
