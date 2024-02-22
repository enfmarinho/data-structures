#include "Queue/Queue.h"
#include "gtest/gtest.h"
#include <initializer_list>
#include <list>
#include <queue>

/*
 * The "lib" macro can be used to select which lib to test. To ensure the
 * integrity of the tests, the "std" namespace must be used, to effectively test
 * the vector developed the "sc" namespace must be used. Note: the test for the
 * range constructor does not use the "lib" macro, because it is not compatible
 * with "std", instead the namespace "sc" is used directly.
 */
#define lib sc
// #define lib std

TEST(SpecialFunctions, DefaultConstructor) {
  lib::queue<int> queue1;
  EXPECT_TRUE(queue1.empty());
  EXPECT_EQ(queue1.size(), 0);

  lib::queue<std::string> queue2;
  EXPECT_TRUE(queue2.empty());
  EXPECT_EQ(queue2.size(), 0);

  lib::queue<int, std::list<int>> queue3;
  EXPECT_TRUE(queue3.empty());
  EXPECT_EQ(queue3.size(), 0);

  lib::queue<std::string, std::vector<std::string>> queue4;
  EXPECT_TRUE(queue4.empty());
  EXPECT_EQ(queue4.size(), 0);
}

TEST(SpecialFunctions, InitializerListConstructor) {
  std::initializer_list<int> ilist1{1, 2, 3, 4};
  lib::queue<int> queue1(ilist1);
  ASSERT_EQ(queue1.size(), ilist1.size());
  int counter{0};
  while (not queue1.empty()) {
    ASSERT_EQ(queue1.front(), ++counter);
    queue1.pop();
  }

  lib::queue<int, std::list<int>> queue2(ilist1);
  ASSERT_EQ(queue2.size(), ilist1.size());
  counter = 0;
  while (not queue2.empty()) {
    ASSERT_EQ(queue2.front(), ++counter);
    queue2.pop();
  }

  lib::queue<int> queue3{};
  ASSERT_TRUE(queue3.empty());

  lib::queue<int, std::list<int>> queue4{};
  ASSERT_TRUE(queue4.empty());
}

TEST(SpecialFunctions, CopyConstructor) {
  std::initializer_list<int> ilist1{1, 2, 3, 4, 5};
  lib::queue<int> queue1(ilist1);
  lib::queue<int> queue1_copy(queue1);
  ASSERT_EQ(queue1_copy.size(), 5);
  int counter{0};
  while (not queue1_copy.empty()) {
    ASSERT_EQ(queue1_copy.front(), ++counter);
    queue1_copy.pop();
  }

  lib::queue<int, std::list<int>> queue2(ilist1);
  lib::queue<int, std::list<int>> queue2_copy(queue2);
  counter = 0;
  while (not queue2_copy.empty()) {
    ASSERT_EQ(queue2_copy.front(), ++counter);
    queue2_copy.pop();
  }

  lib::queue<int, std::list<int>> queue3;
  lib::queue<int, std::list<int>> queue3_copy(queue3);
  EXPECT_TRUE(queue3_copy.empty());
}

TEST(SpecialFunctions, RangeConstructor) {
  int array[] = {1, 2, 3, 4, 5, 6};
  sc::queue<int> queue1(array, array + 6);
  EXPECT_EQ(queue1.size(), 6);
  int counter{0};
  while (not queue1.empty()) {
    EXPECT_EQ(queue1.front(), ++counter);
    queue1.pop();
  }

  sc::queue<int, std::list<int>> queue2(array, array + 6);
  counter = 0;
  while (not queue2.empty()) {
    EXPECT_EQ(queue2.front(), ++counter);
    queue2.pop();
  }

  sc::queue<int> queue3(array, array);
  EXPECT_TRUE(queue3.empty());
}

TEST(SpecialFunctions, AssignOperator) {
  std::initializer_list<int> ilist1{1, 2, 3, 4, 5};
  lib::queue<int> queue1(ilist1);
  lib::queue<int> queue1_copy = queue1;
  EXPECT_EQ(queue1_copy.size(), queue1.size());
  for (int counter{1}; counter < 6; ++counter) {
    EXPECT_EQ(queue1_copy.front(), counter);
    queue1_copy.pop();
  }

  lib::queue<int, std::list<int>> queue2(ilist1);
  lib::queue<int, std::list<int>> queue2_copy = queue2;
  EXPECT_EQ(queue2_copy.size(), queue2.size());
  for (int counter{1}; counter < 6; ++counter) {
    EXPECT_EQ(queue2_copy.front(), counter);
    queue2_copy.pop();
  }

  lib::queue<int> queue3;
  lib::queue<int> queue3_copy = queue3;
  EXPECT_EQ(queue3_copy.size(), queue3.size());
}

TEST(ElementAccess, front) {
  std::initializer_list<int> ilist1{1, 2, 3, 4, 5, 6, 7};
  lib::queue<int> queue1(ilist1);
  EXPECT_EQ(queue1.size(), 7);
  for (int counter{1}; counter < 8; ++counter) {
    EXPECT_EQ(queue1.front(), counter);
    queue1.pop();
  }

  lib::queue<int, std::list<int>> queue2(ilist1);
  EXPECT_EQ(queue2.size(), 7);
  for (int counter{1}; counter < 8; ++counter) {
    EXPECT_EQ(queue2.front(), counter);
    queue2.pop();
  }
}

TEST(ElementAccess, back) {
  lib::queue<int> queue1;
  for (int counter{1}; counter < 8; ++counter) {
    queue1.push(counter);
    EXPECT_EQ(queue1.back(), counter);
  }

  lib::queue<int, std::list<int>> queue2;
  for (int counter{1}; counter < 8; ++counter) {
    queue2.push(counter);
    EXPECT_EQ(queue2.back(), counter);
  }
}

TEST(Capacity, empty) {
  std::initializer_list<int> ilist1{1, 2, 3, 4, 5};
  lib::queue<int> queue1(ilist1);
  EXPECT_FALSE(queue1.empty());

  lib::queue<int, std::list<int>> queue2(ilist1);
  EXPECT_FALSE(queue2.empty());

  lib::queue<int> queue3;
  EXPECT_TRUE(queue3.empty());
}

TEST(Capacity, size) {
  std::initializer_list<int> ilist1{1, 2, 3, 4, 5};
  lib::queue<int> queue1(ilist1);
  EXPECT_EQ(queue1.size(), 5);

  lib::queue<int, std::list<int>> queue2(ilist1);
  EXPECT_EQ(queue1.size(), 5);

  lib::queue<int> queue3;
  EXPECT_EQ(queue3.size(), 0);
}

TEST(Modifiers, push) {
  lib::queue<int> queue1;
  for (int counter{0}; counter < 9; ++counter) {
    queue1.push(counter);
    EXPECT_EQ(queue1.front(), counter);
    queue1.pop();
  }

  lib::queue<int, std::list<int>> queue2;
  for (int counter{0}; counter < 9; ++counter) {
    queue2.push(counter);
    EXPECT_EQ(queue2.front(), counter);
    queue2.pop();
  }
}

TEST(Modifiers, pop) {
  std::initializer_list<int> ilist1{1, 2, 3, 4, 5};
  lib::queue<int> queue1(ilist1);
  int counter{0};
  while (not queue1.empty()) {
    EXPECT_EQ(queue1.front(), ++counter);
    queue1.pop();
  }

  lib::queue<int, std::list<int>> queue2(ilist1);
  counter = 0;
  while (not queue1.empty()) {
    EXPECT_EQ(queue1.front(), ++counter);
    queue1.pop();
  }
}

int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
