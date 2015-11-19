#include "gtest/gtest.h"
#include "../src/Genome.h"

TEST(Genome, GenomeConst) {
  Genome h{10,0.2};
  Genome g{10,0.2,[](size_t s) {return 1;}};
  Genome f{10,0.2,[](size_t s) {return 1;}};
  EXPECT_EQ(1.0, 1.0);
}
