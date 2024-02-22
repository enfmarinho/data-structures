#include "Heap/Heap.h"
#include "gtest/gtest.h"
#include <deque>
#include <functional> // less, greater
#include <initializer_list>
#include <queue>
#include <utility> // move
#include <vector>

/*
 * The heap macro can be used to select which lib to test. To ensure the
 * integrity of the tests, the std::priority_queue must be used, to effectively
 * test the heap developed the tree::Heap must be used. Note: the test
 * "InitializerListConstructor" does not use the heap macro, because it is not
 * defined in std::priority_queue, instead tree::heap is used directly.
 */
#define heap tree::Heap
// #define heap std::priority_queue

TEST(SpecialFunctions, DefaultConstructor) {
  heap<int> heap1;
  EXPECT_EQ(heap1.size(), 0);
  EXPECT_TRUE(heap1.empty());

  heap<char> heap2;
  EXPECT_EQ(heap2.size(), 0);
  EXPECT_TRUE(heap2.empty());

  heap<std::string> heap3;
  EXPECT_EQ(heap3.size(), 0);
  EXPECT_TRUE(heap3.empty());
}

TEST(SpecialFunctions, RangeConstructor) {
  int array[] = {111, 39, 23, 119, 139, 12, 8, 43, 71, 98};

  heap<int> heap1(array, array + 5);
  int last = heap1.top();
  while (!heap1.empty()) {
    heap1.pop();
    EXPECT_TRUE(last >= heap1.top());
    last = heap1.top();
  }

  heap<int> heap2(array, array + 6);
  last = heap2.top();
  while (!heap2.empty()) {
    heap2.pop();
    EXPECT_TRUE(last >= heap2.top());
    last = heap2.top();
  }

  heap<int, std::deque<int>, std::greater<int>> heap3(array, array + 9);
  last = heap3.top();
  while (!heap3.empty()) {
    heap3.pop();
    EXPECT_TRUE(last <= heap3.top());
    last = heap3.top();
  }

  heap<int> heap4(array, array);
  EXPECT_TRUE(heap4.empty());
}

TEST(SpecialFunctions, InitializerListConstructor) {
  std::initializer_list<int> ilist1{1, 2, 3, 4, 5}, ilist2{},
      ilist3{8, 7, 6, 5, 4, 3, 2, 1}, ilist4{8, 2, 9, 5, 3, 1, 4, 6, 7};

  tree::Heap<int> heap1(ilist1);
  EXPECT_FALSE(heap1.empty());
  EXPECT_EQ(heap1.size(), ilist1.size());
  int expected = 6;
  while (!heap1.empty()) {
    EXPECT_EQ(heap1.top(), --expected);
    heap1.pop();
  }

  tree::Heap<int> heap2(ilist2);
  EXPECT_TRUE(heap2.empty());

  tree::Heap<int, std::vector<int>, std::greater<int>> heap3(ilist3);
  EXPECT_FALSE(heap3.empty());
  EXPECT_EQ(heap3.size(), ilist3.size());
  expected = 0;
  while (!heap3.empty()) {
    EXPECT_EQ(heap3.top(), ++expected);
    heap3.pop();
  }

  tree::Heap<int, std::deque<int>, std::greater<int>> heap4(ilist4);
  EXPECT_FALSE(heap4.empty());
  EXPECT_EQ(heap4.size(), ilist4.size());
  expected = 0;
  while (!heap4.empty()) {
    EXPECT_EQ(heap4.top(), ++expected);
    heap4.pop();
  }
}

TEST(SpecialFunctions, CopyConstructor) {
  heap<int> heap1;
  for (int value{0}; value < 51; value += 2) {
    heap1.push(value);
  }
  heap<int> heap1_copy(heap1);
  EXPECT_EQ(heap1.size(), heap1_copy.size());
  for (int value{50}; value >= 0; value -= 2) {
    EXPECT_EQ(heap1.top(), value);
    EXPECT_EQ(heap1_copy.top(), value);
    heap1.pop();
    heap1_copy.pop();
  }

  heap<int> heap2;
  heap<int> heap2_copy(heap2);
  EXPECT_TRUE(heap2.empty());
  EXPECT_TRUE(heap2_copy.empty());
}

TEST(SpecialFunctions, MoveConstructor) {
  heap<int> heap1;
  for (int value{0}; value < 51; value += 2) {
    heap1.push(value);
  }
  int expected_size = heap1.size();
  heap<int> heap1_copy(std::move(heap1));
  EXPECT_TRUE(heap1.empty());
  EXPECT_EQ(heap1_copy.size(), expected_size);
  for (int value{50}; value >= 0; value -= 2) {
    EXPECT_EQ(heap1_copy.top(), value);
    heap1_copy.pop();
  }

  heap<int> heap2;
  heap<int> heap2_copy(std::move(heap2));
  EXPECT_TRUE(heap2.empty());
  EXPECT_TRUE(heap2_copy.empty());

  heap<int> heap3;
  heap3.push(3);
  heap<int> heap3_copy(std::move(heap3));
  EXPECT_TRUE(heap3.empty());
  EXPECT_FALSE(heap3_copy.empty());
}

TEST(Capacity, empty) {
  heap<int> heap1;
  EXPECT_TRUE(heap1.empty());
  heap1.push(34);
  EXPECT_FALSE(heap1.empty());
}

TEST(Capacity, size) {
  heap<int> heap1;
  EXPECT_EQ(heap1.size(), 0);
  heap1.push(2);
  EXPECT_EQ(heap1.size(), 1);
  heap1.pop();
  EXPECT_EQ(heap1.size(), 0);
}

TEST(Modifiers, PushPopTop) {
  heap<int> heap1;
  for (int value = 0; value < 100; value += 10) {
    heap1.push(value);
  }
  int last = heap1.top();
  while (!heap1.empty()) {
    heap1.pop();
    EXPECT_TRUE(last >= heap1.top());
    last = heap1.top();
  }

  heap<int> heap2;
  for (int value = 0; value < 100; value += 10) {
    heap2.push(value);
  }
  last = heap2.top();
  while (!heap2.empty()) {
    heap2.pop();
    EXPECT_TRUE(last >= heap2.top());
    last = heap2.top();
  }

  heap<int> heap3;
  for (int value = 1; value < 6; ++value) {
    heap3.push(value);
  }
  EXPECT_FALSE(heap3.empty());
  EXPECT_EQ(heap3.size(), 5);
  int expected = 6;
  while (!heap3.empty()) {
    EXPECT_EQ(heap3.top(), --expected);
    heap3.pop();
  }

  heap<int> heap4;
  heap<int, std::vector<int>, std::greater<int>> heap5;
  for (int value = 5; value < 31; ++value) {
    heap4.push(value);
    heap4.push(value);
    heap5.push(value);
    heap5.push(value);
  }
  int heap4_last = heap4.top();
  int heap5_last = heap5.top();
  while (!heap4.empty() and !heap5.empty()) {
    heap4.pop();
    heap5.pop();
    EXPECT_TRUE(heap4_last >= heap4.top());
    heap4_last = heap4.top();
    EXPECT_TRUE(heap5_last <= heap5.top());
    heap5_last = heap5.top();
  }
}

int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
