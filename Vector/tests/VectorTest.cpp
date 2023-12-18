#include "../src/Vector.h"
#include "gtest/gtest.h"
#include <initializer_list>
#include <vector>

/*
 * The "lib" macro can be used to select which lib to test. To ensure the
 * integrity of the tests, the "std" namespace must be used, to effectively test
 * the vector developed the "sc" namespace must be used. Note: some tests, such
 * as push_front, does not use the "lib" macro, because they are not defined in
 * "std", instead the namespace "sc" is used directly.
 */
#define lib sc
// #define lib std

TEST(SpecialFunctions, DefaultConstructor) {
  lib::vector<int> list1;
  EXPECT_TRUE(list1.empty());
  EXPECT_EQ(list1.size(), 0);
  EXPECT_EQ(list1.capacity(), 0);

  lib::vector<char> list2(5);
  EXPECT_FALSE(list2.empty());
  EXPECT_EQ(list2.size(), 5);
  EXPECT_EQ(list2.capacity(), 5);

  lib::vector<std::string> list3(9, "exemple");
  EXPECT_FALSE(list3.empty());
  EXPECT_EQ(list3.size(), 9);
  EXPECT_EQ(list3.capacity(), 9);
}

TEST(SpecialFunctions, AssignInitializerList) {
  std::initializer_list<int> ilist = {1, 2, 3, 4, 5, 6};
  lib::vector<int> list1 = ilist;
  EXPECT_FALSE(list1.empty());
  EXPECT_EQ(list1.size(), 6);
  EXPECT_EQ(list1.capacity(), 6);
}

TEST(SpecialFunctions, InitializerListConstructor) {
  lib::vector<char> list1{};
  EXPECT_TRUE(list1.empty());
  EXPECT_EQ(list1.size(), 0);
  EXPECT_EQ(list1.capacity(), 0);

  lib::vector<int> list2{1, 2, 3, 4, 5};
  EXPECT_FALSE(list2.empty());
  EXPECT_EQ(list2.size(), 5);
  EXPECT_EQ(list2.capacity(), 5);
}

TEST(SpecialFunctions, CopyConstructor) {
  lib::vector<int> list1{1, 2, 3, 4, 5};
  lib::vector<int> list2(list1);
  EXPECT_FALSE(list2.empty());
  EXPECT_EQ(list2.size(), 5);
  EXPECT_EQ(list2.capacity(), 5);
}

TEST(SpecialFunctions, AssignRange) {
  int array[] = {1, 2, 3, 4, 5};
  lib::vector<int> list1;
  EXPECT_TRUE(list1.empty());
  EXPECT_EQ(list1.size(), 0);
  EXPECT_EQ(list1.capacity(), 0);

  list1.assign(array, array + 5);
  EXPECT_FALSE(list1.empty());
  EXPECT_EQ(list1.size(), 5);
  EXPECT_EQ(list1.capacity(), 5);
}

TEST(SpecialFunctions, RangeConstructor) {
  int array[] = {1, 2, 3, 4, 5};
  lib::vector<int> list1(array, array + 5);
  EXPECT_FALSE(list1.empty());
  EXPECT_EQ(list1.size(), 5);
  EXPECT_EQ(list1.capacity(), 5);
}
int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
