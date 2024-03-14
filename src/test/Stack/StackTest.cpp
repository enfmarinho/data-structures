#include "Stack/Stack.h"
#include "gtest/gtest.h"
#include <initializer_list>
#include <list>
#include <stack>
#include <vector>

/*
 * The "lib" macro can be used to select which lib to test. To ensure the
 * integrity of the tests, the "std" namespace must be used, to effectively test
 * the vector developed the "sc" namespace must be used. Note: the test for the
 * range constructor does not use the "lib" macro, because it is incompatible
 * with the standard library, instead the namespace "sc" is used directly.
 */
#define lib sc
// #define lib std

TEST(SpecialFunctions, DefaultConstructor) {
  lib::stack<int> stack1;
  EXPECT_TRUE(stack1.empty());
  EXPECT_EQ(stack1.size(), 0);

  lib::stack<std::string> stack2;
  EXPECT_TRUE(stack2.empty());
  EXPECT_EQ(stack2.size(), 0);

  lib::stack<int, std::vector<int>> stack3;
  EXPECT_TRUE(stack3.empty());
  EXPECT_EQ(stack3.size(), 0);

  lib::stack<std::string, std::list<std::string>> stack4;
  EXPECT_TRUE(stack4.empty());
  EXPECT_EQ(stack4.size(), 0);
}

TEST(SpecialFunctions, InitializerListConstructor) {
  std::initializer_list<int> ilist1{1, 2, 3, 4};
  lib::stack<int> stack1(ilist1);
  ASSERT_EQ(stack1.size(), ilist1.size());
  int counter{5};
  while (!stack1.empty()) {
    ASSERT_EQ(stack1.top(), --counter);
    stack1.pop();
  }

  lib::stack<int, std::list<int>> stack2(ilist1);
  ASSERT_EQ(stack2.size(), ilist1.size());
  counter = 5;
  while (!stack2.empty()) {
    ASSERT_EQ(stack2.top(), --counter);
    stack2.pop();
  }

  lib::stack<int> stack3{};
  ASSERT_TRUE(stack3.empty());

  lib::stack<int, std::vector<int>> stack4{};
  ASSERT_TRUE(stack4.empty());
}

TEST(SpecialFunctions, CopyConstructor) {
  std::initializer_list<int> ilist1{1, 2, 3, 4, 5};
  lib::stack<int> stack1(ilist1);
  lib::stack<int> stack1_copy(stack1);
  ASSERT_EQ(stack1_copy.size(), 5);
  int counter{6};
  while (!stack1_copy.empty()) {
    ASSERT_EQ(stack1_copy.top(), --counter);
    stack1_copy.pop();
  }

  lib::stack<int, std::list<int>> stack2(ilist1);
  lib::stack<int, std::list<int>> stack2_copy(stack2);
  counter = 6;
  while (!stack2_copy.empty()) {
    ASSERT_EQ(stack2_copy.top(), --counter);
    stack2_copy.pop();
  }

  lib::stack<int, std::list<int>> stack3;
  lib::stack<int, std::list<int>> stack3_copy(stack3);
  EXPECT_TRUE(stack3_copy.empty());
}

TEST(SpecialFunctions, RangeConstructor) {
  int array[] = {1, 2, 3, 4, 5, 6};
  sc::stack<int> stack1(array, array + 6);
  EXPECT_EQ(stack1.size(), 6);
  int counter{7};
  while (!stack1.empty()) {
    EXPECT_EQ(stack1.top(), --counter);
    stack1.pop();
  }

  sc::stack<int, std::list<int>> stack2(array, array + 6);
  counter = 7;
  while (!stack2.empty()) {
    EXPECT_EQ(stack2.top(), --counter);
    stack2.pop();
  }

  sc::stack<int> stack3(array, array);
  EXPECT_TRUE(stack3.empty());
}

TEST(SpecialFunctions, AssignOperator) {
  std::initializer_list<int> ilist1{1, 2, 3, 4, 5};
  lib::stack<int> stack1(ilist1);
  lib::stack<int> stack1_copy = stack1;
  EXPECT_EQ(stack1_copy.size(), stack1.size());
  for (int counter{5}; counter > 0; --counter) {
    EXPECT_EQ(stack1_copy.top(), counter);
    stack1_copy.pop();
  }

  lib::stack<int, std::list<int>> stack2(ilist1);
  lib::stack<int, std::list<int>> stack2_copy = stack2;
  EXPECT_EQ(stack2_copy.size(), stack2.size());
  for (int counter{5}; counter > 0; --counter) {
    EXPECT_EQ(stack2_copy.top(), counter);
    stack2_copy.pop();
  }

  lib::stack<int> stack3;
  lib::stack<int> stack3_copy = stack3;
  EXPECT_EQ(stack3_copy.size(), stack3.size());
}

TEST(ElementAccess, top) {
  std::initializer_list<int> ilist1{1, 2, 3, 4};
  lib::stack<int> stack1(ilist1);

  lib::stack<int, std::vector<int>> stack2(ilist1);
  for (int counter{4}; counter > 0; --counter) {
    EXPECT_EQ(stack2.top(), counter);
    stack2.pop();
  }
}

TEST(Capacity, empty) {
  std::initializer_list<int> ilist1{1, 2, 3, 4, 5};
  lib::stack<int> stack1(ilist1);
  EXPECT_FALSE(stack1.empty());

  lib::stack<int, std::list<int>> stack2(ilist1);
  EXPECT_FALSE(stack2.empty());

  lib::stack<int> stack3;
  EXPECT_TRUE(stack3.empty());
}

TEST(Capacity, size) {
  std::initializer_list<int> ilist1{1, 2, 3, 4, 5};
  lib::stack<int> stack1(ilist1);
  EXPECT_EQ(stack1.size(), 5);

  lib::stack<int, std::list<int>> stack2(ilist1);
  EXPECT_EQ(stack1.size(), 5);

  lib::stack<int> stack3;
  EXPECT_EQ(stack3.size(), 0);
}

TEST(Modifiers, push) {
  lib::stack<int> stack1;
  for (int counter{0}; counter < 9; ++counter) {
    stack1.push(counter);
    EXPECT_EQ(stack1.top(), counter);
  }

  lib::stack<int, std::list<int>> stack2;
  for (int counter{0}; counter < 9; ++counter) {
    stack2.push(counter);
    EXPECT_EQ(stack2.top(), counter);
  }
}

TEST(Modifiers, pop) {
  std::initializer_list<int> ilist1{1, 2, 3, 4, 5};
  lib::stack<int> stack1(ilist1);
  int counter{6};
  while (!stack1.empty()) {
    EXPECT_EQ(stack1.top(), --counter);
    stack1.pop();
  }

  lib::stack<int, std::list<int>> stack2(ilist1);
  counter = 6;
  while (!stack1.empty()) {
    EXPECT_EQ(stack1.top(), --counter);
    stack1.pop();
  }
}

int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
