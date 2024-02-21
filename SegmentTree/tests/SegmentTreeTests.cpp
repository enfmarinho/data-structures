#include "../src/SegmentTree.h"
#include "gtest/gtest.h"
#include <utility> // move
#include <vector>

TEST(SpecialFunctions, ConstructorsQuerySize) {
  std::vector<int> list1{21, 72, 13, 91, 12}, list2{1, 9, 3, 7},
      list3{2, 13, 99, 111, 101, 103};
  tree::SegmentTree<int, operations::Sum<int>> segtree1(list1, 0);
  EXPECT_EQ(segtree1.size(), 5);
  EXPECT_EQ(segtree1.query(0, list1.size()), 209);
  EXPECT_EQ(segtree1.query(0, list1.size() - 1), 197);
  EXPECT_EQ(segtree1.query(1, 3), 85);
  EXPECT_EQ(segtree1.query(3, 5), 103);

  tree::SegmentTree<int, operations::Max<int>> segtree2(
      list2, operations::Max<int>::neutral_element());
  EXPECT_EQ(segtree2.size(), 4);
  EXPECT_EQ(segtree2.query(0, list2.size()), 9);
  EXPECT_EQ(segtree2.query(1, list2.size()), 9);
  EXPECT_EQ(segtree2.query(2, list2.size()), 7);
  EXPECT_EQ(segtree2.query(2, list2.size() - 1), 3);
  EXPECT_EQ(segtree2.query(1, 3), 9);

  tree::SegmentTree<int, operations::Min<int>> segtree3(
      list3, operations::Min<int>::neutral_element());
  EXPECT_EQ(segtree3.size(), 6);
  EXPECT_EQ(segtree3.query(0, list3.size()), 2);
  EXPECT_EQ(segtree3.query(1, list3.size()), 13);
  EXPECT_EQ(segtree3.query(2, list3.size() - 1), 99);
  EXPECT_EQ(segtree3.query(3, list3.size()), 101);
}

TEST(SpecialFunctions, CopyConstructor) {
  std::vector<int> list1{21, 72, 13, 91, 12}, list2{1, 9, 3, 7},
      list3{2, 13, 99, 111, 101, 103};
  tree::SegmentTree<int, operations::Sum<int>> segtree1(list1, 0);
  tree::SegmentTree<int, operations::Max<int>> segtree2(
      list2, operations::Max<int>::neutral_element());
  tree::SegmentTree<int, operations::Min<int>> segtree3(
      list3, operations::Min<int>::neutral_element());

  tree::SegmentTree<int, operations::Sum<int>> segtree1_copy(segtree1);
  EXPECT_EQ(segtree1_copy.size(), 5);
  EXPECT_EQ(segtree1_copy.query(0, list1.size()), 209);
  EXPECT_EQ(segtree1_copy.query(0, list1.size() - 1), 197);
  EXPECT_EQ(segtree1_copy.query(1, 3), 85);
  EXPECT_EQ(segtree1_copy.query(3, 5), 103);

  tree::SegmentTree<int, operations::Max<int>> segtree2_copy(segtree2);
  EXPECT_EQ(segtree2_copy.size(), 4);
  EXPECT_EQ(segtree2_copy.query(0, list2.size()), 9);
  EXPECT_EQ(segtree2_copy.query(1, list2.size()), 9);
  EXPECT_EQ(segtree2_copy.query(2, list2.size()), 7);
  EXPECT_EQ(segtree2_copy.query(2, list2.size() - 1), 3);
  EXPECT_EQ(segtree2_copy.query(1, 3), 9);

  tree::SegmentTree<int, operations::Min<int>> segtree3_copy(segtree3);
  EXPECT_EQ(segtree3_copy.size(), 6);
  EXPECT_EQ(segtree3_copy.query(0, list3.size()), 2);
  EXPECT_EQ(segtree3_copy.query(1, list3.size()), 13);
  EXPECT_EQ(segtree3_copy.query(2, list3.size() - 1), 99);
  EXPECT_EQ(segtree3_copy.query(3, list3.size()), 101);
}

TEST(SpecialFunctions, MoveConstructor) {
  std::vector<int> list1{21, 72, 13, 91, 12}, list2{1, 9, 3, 7},
      list3{2, 13, 99, 111, 101, 103};
  tree::SegmentTree<int, operations::Sum<int>> segtree1(list1, 0);
  tree::SegmentTree<int, operations::Max<int>> segtree2(
      list2, operations::Max<int>::neutral_element());
  tree::SegmentTree<int, operations::Min<int>> segtree3(
      list3, operations::Min<int>::neutral_element());

  tree::SegmentTree<int, operations::Sum<int>> segtree1_copy(
      std::move(segtree1));
  EXPECT_EQ(segtree1_copy.size(), 5);
  EXPECT_EQ(segtree1.size(), 0);
  EXPECT_EQ(segtree1_copy.query(0, list1.size()), 209);
  EXPECT_EQ(segtree1_copy.query(0, list1.size() - 1), 197);
  EXPECT_EQ(segtree1_copy.query(1, 3), 85);
  EXPECT_EQ(segtree1_copy.query(3, 5), 103);

  tree::SegmentTree<int, operations::Max<int>> segtree2_copy(
      std::move(segtree2));
  EXPECT_EQ(segtree2_copy.size(), 4);
  EXPECT_EQ(segtree2.size(), 0);
  EXPECT_EQ(segtree2_copy.query(0, list2.size()), 9);
  EXPECT_EQ(segtree2_copy.query(1, list2.size()), 9);
  EXPECT_EQ(segtree2_copy.query(2, list2.size()), 7);
  EXPECT_EQ(segtree2_copy.query(1, 3), 9);

  tree::SegmentTree<int, operations::Min<int>> segtree3_copy(
      std::move(segtree3));
  EXPECT_EQ(segtree3_copy.size(), 6);
  EXPECT_EQ(segtree3.size(), 0);
  EXPECT_EQ(segtree3_copy.query(0, list3.size()), 2);
  EXPECT_EQ(segtree3_copy.query(1, list3.size()), 13);
  EXPECT_EQ(segtree3_copy.query(2, list3.size() - 1), 99);
  EXPECT_EQ(segtree3_copy.query(3, list3.size()), 101);
}

TEST(Modifiers, update) {
  std::vector<int> list1{21, 72, 13, 91, 12}, list2{1, 9, 3, 7},
      list3{2, 13, 99, 111, 101, 103};

  tree::SegmentTree<int, operations::Sum<int>> segtree1(list1, 0);
  segtree1.update(0, 40);
  EXPECT_EQ(segtree1.query(0, list1.size()), 228);
  EXPECT_EQ(segtree1.query(0, 1), 40);
  EXPECT_EQ(segtree1.query(0, 2), 112);

  tree::SegmentTree<int, operations::Max<int>> segtree2(
      list2, operations::Max<int>::neutral_element());
  segtree2.update(2, 11);
  EXPECT_EQ(segtree2.query(0, list2.size()), 11);
  EXPECT_EQ(segtree2.query(2, list2.size() - 1), 11);
  segtree2.update(3, 13);
  EXPECT_EQ(segtree2.query(0, list2.size()), 13);
  EXPECT_EQ(segtree2.query(2, list2.size() - 1), 11);

  tree::SegmentTree<int, operations::Min<int>> segtree3(
      list3, operations::Min<int>::neutral_element());
  segtree3.update(4, 2);
  EXPECT_EQ(segtree3.query(0, list3.size()), 2);
  EXPECT_EQ(segtree3.query(3, list3.size()), 2);
  EXPECT_EQ(segtree3.query(3, 5), 2);
}

int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
