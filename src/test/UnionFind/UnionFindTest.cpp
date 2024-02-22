#include "UnionFind/UnionFind.h"
#include "gtest/gtest.h"
#include <utility> // move

TEST(SpecialFunctions, DefaultConstructor) {
  tree::UnionFind ufind(10);
  EXPECT_EQ(ufind.size(), 10);
  EXPECT_EQ(ufind.groups(), 10);
  for (int i = 0; i < 10; ++i) {
    EXPECT_EQ(ufind.find(i), i);
    EXPECT_EQ(ufind.group_size(i), 1);
  }
}

TEST(SpecialFunctions, CopyConstructor) {
  tree::UnionFind ufind(9);
  tree::UnionFind ufind_copy(ufind);
  EXPECT_EQ(ufind.size(), 9);
  EXPECT_EQ(ufind_copy.size(), 9);
  EXPECT_EQ(ufind.groups(), 9);
  EXPECT_EQ(ufind_copy.groups(), 9);
  for (int i = 0; i < 9; ++i) {
    EXPECT_EQ(ufind.find(i), i);
    EXPECT_EQ(ufind_copy.find(i), i);
    EXPECT_EQ(ufind.group_size(i), 1);
    EXPECT_EQ(ufind_copy.group_size(i), 1);
  }
}

TEST(SpecialFunctions, MoveConstructor) {
  tree::UnionFind ufind(13);
  EXPECT_EQ(ufind.size(), 13);
  tree::UnionFind ufind_copy(std::move(ufind));
  EXPECT_EQ(ufind.size(), 0);
  EXPECT_EQ(ufind_copy.size(), 13);
  EXPECT_EQ(ufind.groups(), 0);
  EXPECT_EQ(ufind_copy.groups(), 13);
  for (int i = 0; i < 13; ++i) {
    EXPECT_EQ(ufind_copy.find(i), i);
    EXPECT_EQ(ufind_copy.group_size(i), 1);
  }
}

TEST(ElementAccess, UniteFind) {
  tree::UnionFind ufind(5);
  for (int index{0}; index < 5; ++index) {
    EXPECT_EQ(ufind.find(index), index);
  }
  ufind.unite(0, 2);
  for (int index{0}; index < 5; ++index) {
    if (index == 0 or index == 2) {
      EXPECT_EQ(ufind.find(0), ufind.find(2));
    } else {
      EXPECT_EQ(ufind.find(index), index);
    }
  }
  ufind.unite(2, 4);
  ufind.unite(1, 3);
  EXPECT_EQ(ufind.find(0), ufind.find(2));
  EXPECT_EQ(ufind.find(2), ufind.find(4));
  EXPECT_EQ(ufind.find(1), ufind.find(3));
  ufind.unite(1, 4);
  for (int index{0}; index < 5 - 1; ++index) {
    EXPECT_EQ(ufind.find(index), ufind.find(index + 1));
  }
}

TEST(Capacity, size) {
  tree::UnionFind ufind1(10), ufind2(13), ufind3(0), ufind4(1000);
  EXPECT_EQ(ufind1.size(), 10);
  EXPECT_EQ(ufind2.size(), 13);
  EXPECT_EQ(ufind3.size(), 0);
  EXPECT_EQ(ufind4.size(), 1000);
}

TEST(Capacity, groups) {
  tree::UnionFind ufind(4);
  EXPECT_EQ(ufind.groups(), 4);
  ufind.unite(0, 1);
  EXPECT_EQ(ufind.groups(), 3);
  ufind.unite(2, 3);
  EXPECT_EQ(ufind.groups(), 2);
  ufind.unite(2, 3);
  EXPECT_EQ(ufind.groups(), 2);
  ufind.unite(0, 3);
  EXPECT_EQ(ufind.groups(), 1);
}

TEST(Capacity, group_size) {
  tree::UnionFind ufind(5);
  for (int index{0}; index < 5; ++index) {
    EXPECT_EQ(ufind.group_size(index), 1);
  }
  ufind.unite(1, 4);
  for (int index{0}; index < 5; ++index) {
    if (index == 1 or index == 4) {
      EXPECT_EQ(ufind.group_size(index), 2);
    } else {
      EXPECT_EQ(ufind.group_size(index), 1);
    }
  }
  ufind.unite(4, 2);
  ufind.unite(0, 3);
  for (int index{0}; index < 5; ++index) {
    if (index == 1 or index == 4 or index == 2) {
      EXPECT_EQ(ufind.group_size(index), 3);
    } else {
      EXPECT_EQ(ufind.group_size(index), 2);
    }
  }
  ufind.unite(1, 0);
  for (int index{0}; index < 5; ++index) {
    EXPECT_EQ(ufind.group_size(index), 5);
  }
}

TEST(Operations, connected) {
  tree::UnionFind ufind(6);
  for (int index{0}; index < 6 - 1; ++index) {
    EXPECT_FALSE(ufind.connected(index, index + 1));
  }
  ufind.unite(0, 1);
  for (int index{0}; index < 6 - 1; ++index) {
    if (index == 0) {
      EXPECT_TRUE(ufind.connected(index, index + 1));
    } else {
      EXPECT_FALSE(ufind.connected(index, index + 1));
    }
  }
  ufind.unite(1, 4);
  EXPECT_TRUE(ufind.connected(1, 4));
  EXPECT_TRUE(ufind.connected(0, 4));
  EXPECT_FALSE(ufind.connected(2, 4));
}

int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
