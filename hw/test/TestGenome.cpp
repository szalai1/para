#include "gtest/gtest.h"
#include "../src/Genome.h"

TEST(Individual, IndividualConst) {
  Individual h{};
  EXPECT_EQ(1.0, 1.0);
}
