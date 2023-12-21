#include "../src/DoubleLinkedList.h"
#include "gtest/gtest.h"

/*
 * The "lib" macro can be used to select which lib to test. To ensure the
 * integrity of the tests, the "std" namespace must be used, to effectively test
 * the vector developed the "sc" namespace must be used. Some tests do not use
 * this macro because the functions they tests are not defined in the standard
 * library, instead the namespace "sc" is used directly.
 */
#define lib sc
// #define lib std

// The 3 following iterator tests are based in the Initializer List Constructor,
// so if they failed, it can be a problem with that. The Initializer List
// Constructor tests can not be done before, because it depend on these
// iterator functions.
TEST(Iterator, begin) {
  lib::list<int> list1{1, 2, 3}, list2{0};
  ASSERT_EQ(*list1.begin(), 1);
  ASSERT_EQ(*list2.begin(), 0);
}

TEST(Iterator, Increment) {
  sc::list<int> list1{1, 2, 3}, list2{0, 1};
  ASSERT_EQ(*++list1.begin(), 2);
  ASSERT_EQ(*++list2.begin(), 1);

  auto it = list1.begin();
  ASSERT_EQ(*it++, 1);
  ASSERT_EQ(*it, 2);

  ASSERT_EQ(*(list1.begin() + 1), 2);
  ASSERT_EQ(*(list2.begin() + 0), 0);
  ASSERT_TRUE(list1.begin() + 3 == list1.end());

  ASSERT_EQ(*(1 + list1.begin()), 2);
  ASSERT_EQ(*(0 + list2.begin()), 0);
  ASSERT_TRUE(3 + list1.begin() == list1.end());
  ASSERT_TRUE(2 + list1.begin() + 1 == list1.end());
}

TEST(Iterator, end) {
  lib::list<int> list1{1, 2, 3}, list2{0}, list3{1, 2, 3, 4, 5, 6, 7};
  ASSERT_FALSE(++list1.begin() == list1.end());
  ASSERT_TRUE(++list2.begin() == list2.end());
  int counter{0};
  for (auto v : list3) {
    ASSERT_EQ(v, ++counter);
  }
}
TEST(Iterator, Decrement) {
  lib::list<int> list1{1, 2, 3, 4, 5};
  int counter{6};
  for (auto it = --list1.end(); it != list1.begin(); --it) {
    EXPECT_EQ(*it, --counter);
  }
  counter = 6;
  for (auto it = --list1.end(); it != list1.begin(); it--) {
    EXPECT_EQ(*it, --counter);
  }

  sc::list<int> list2{1, 2, 3, 4};
  auto it = list2.end() - 3;
  EXPECT_EQ(*it, 2);
  std::cout << "poassout\n\n\n" << std::flush;
}

TEST(Iterator, Difference) {
  sc::list<int> list1{12, 145, 190, 10};
  EXPECT_EQ(list1.end() - list1.begin(), list1.size());
  EXPECT_EQ(list1.end() - ++list1.begin(), list1.size() - 1);
}

TEST(Iterator, DereferenceOperator) {
  lib::list<int> list1{1, 2, 3, 4, 5};
  int counter{0};
  for (auto it = list1.begin(); it != list1.end(); ++it) {
    ASSERT_EQ(*it, ++counter);
  }
}

TEST(Iterator, ArrowOperator) {
  lib::list<std::list<int>> list1(5);
  EXPECT_EQ(list1.size(), 5);
  EXPECT_EQ(list1.begin()->size(), 0);
  list1.begin()->push_back(4);
  EXPECT_EQ(list1.begin()->front(), 4);
}

TEST(Iterator, AssignOperator) {
  lib::list<int> list1{1, 2, 3};
  auto it = list1.begin();
  EXPECT_EQ(*it, 1);
  EXPECT_TRUE(it == list1.begin());
  *it = 90;
  EXPECT_EQ(*it, 90);
  EXPECT_EQ(list1.front(), 90);
}

TEST(Iterator, DifferenceOperator) {
  lib::list<int> list1{1, 2, 3, 4, 5};
  EXPECT_TRUE(list1.begin() != list1.end());
  EXPECT_FALSE(list1.begin() != list1.begin());
}

TEST(Iterator, EquivalentOperator) {
  lib::list<int> list1{1, 2, 3, 4, 5};
  EXPECT_FALSE(list1.begin() == list1.end());
  EXPECT_TRUE(list1.begin() == list1.begin());
}
int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
