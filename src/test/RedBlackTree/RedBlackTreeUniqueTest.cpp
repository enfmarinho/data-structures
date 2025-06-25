#include "RedBlackTree/RedBlackTreeUnique.h"
#include "gtest/gtest.h"
#include <limits> // numeric_limits
#include <random> // mt19937, random_device, uniform_int_distribution
#include <string> // std::string
#include <vector> // std::vector

TEST(SpecialFunctions, DefaultConstructor) {
  tree::RedBlackTreeUnique<int> rbt1;
  EXPECT_EQ(rbt1.size(), 0);
  EXPECT_TRUE(rbt1.balanced());
  EXPECT_TRUE(rbt1.empty());

  tree::RedBlackTreeUnique<char> rbt2;
  EXPECT_EQ(rbt2.size(), 0);
  EXPECT_TRUE(rbt2.empty());
  EXPECT_TRUE(rbt2.balanced());

  tree::RedBlackTreeUnique<std::string> rbt3;
  EXPECT_EQ(rbt3.size(), 0);
  EXPECT_TRUE(rbt3.empty());
  EXPECT_TRUE(rbt3.balanced());
}

TEST(SpecialFunctions, InitializerListConstructor) {
  std::initializer_list<int> list1{1, 2, 3, 4}, list2{4, 1, 5, 6, 3, 7, 2},
      list3{};

  tree::RedBlackTreeUnique<int> rbt1(list1);
  EXPECT_EQ(rbt1.size(), list1.size());
  EXPECT_TRUE(rbt1.balanced());
  int counter{0};
  for (int value : rbt1) {
    EXPECT_EQ(value, ++counter);
  }
  EXPECT_TRUE(rbt1.balanced());

  tree::RedBlackTreeUnique<int> rbt2(list2);
  EXPECT_EQ(rbt2.size(), list2.size());
  EXPECT_TRUE(rbt2.balanced());
  counter = 0;
  for (int value : rbt2) {
    EXPECT_EQ(value, ++counter);
  }
  EXPECT_TRUE(rbt2.balanced());

  tree::RedBlackTreeUnique<int> rbt3(list3);
  EXPECT_EQ(rbt3.size(), list3.size());
  EXPECT_TRUE(rbt3.empty());
  EXPECT_TRUE(rbt3.balanced());
  EXPECT_EQ(rbt3.begin(), rbt3.end());
}

TEST(SpecialFunctions, RangeConstructor) {
  int array[] = {9, 1, 4, 2, 5, 99, 3, 7};

  tree::RedBlackTreeUnique<int> rbt1(array, array);
  EXPECT_TRUE(rbt1.empty());
  EXPECT_EQ(rbt1.begin(), rbt1.end());
  EXPECT_TRUE(rbt1.balanced());

  tree::RedBlackTreeUnique<int> rbt2(array, array + 4);
  EXPECT_EQ(rbt2.size(), 4);
  auto it = rbt2.begin();
  EXPECT_EQ(*(it++), 1);
  EXPECT_EQ(*(it++), 2);
  EXPECT_EQ(*(it++), 4);
  EXPECT_EQ(*(it++), 9);
  EXPECT_TRUE(rbt2.balanced());

  tree::RedBlackTreeUnique<int> rbt3(array, array + 8);
  EXPECT_EQ(rbt3.size(), 8);
  EXPECT_TRUE(rbt3.balanced());
  int last = *rbt3.begin();
  for (int value : rbt3) {
    EXPECT_TRUE(last <= value);
    last = value;
  }
}

TEST(SpecialFunctions, CopyConstructor) {
  std::initializer_list<int> list1{1, 2, 3, 4}, list2{4, 1, 5, 6, 3, 7, 2},
      list3{};

  tree::RedBlackTreeUnique<int> rbt1(list1);
  tree::RedBlackTreeUnique<int> rbt1_copy(rbt1);
  EXPECT_TRUE(rbt1.balanced());
  EXPECT_EQ(rbt1.size(), rbt1_copy.size());
  for (auto it = rbt1.begin(), it_copy = rbt1_copy.begin(); it != rbt1.end();
       ++it, ++it_copy) {
    EXPECT_EQ(*it, *it_copy);
  }

  tree::RedBlackTreeUnique<int> rbt2(list2);
  tree::RedBlackTreeUnique<int> rbt2_copy(rbt2);
  EXPECT_TRUE(rbt2.balanced());
  EXPECT_EQ(rbt2.size(), rbt2_copy.size());
  for (auto it = rbt2.begin(), it_copy = rbt2_copy.begin(); it != rbt2.end();
       ++it, ++it_copy) {
    EXPECT_EQ(*it, *it_copy);
  }

  tree::RedBlackTreeUnique<int> rbt3(list3);
  tree::RedBlackTreeUnique<int> rbt3_copy(rbt3);
  EXPECT_EQ(rbt3.size(), rbt3_copy.size());
  EXPECT_TRUE(rbt3.balanced());
  for (auto it = rbt3.begin(), it_copy = rbt3_copy.begin(); it != rbt3.end();
       ++it, ++it_copy) {
    EXPECT_EQ(*it, *it_copy);
  }
}

TEST(SpecialFunctions, MoveConstructor) {
  std::initializer_list<int> list1{1, 2, 3, 4}, list2{4, 1, 5, 6, 3, 7, 2},
      list3{};

  tree::RedBlackTreeUnique<int> rbt1(list1);
  tree::RedBlackTreeUnique<int> rbt1_copy(std::move(rbt1));
  EXPECT_EQ(rbt1.size(), 0);
  EXPECT_EQ(rbt1_copy.size(), list1.size());
  EXPECT_TRUE(rbt1.balanced());
  EXPECT_TRUE(rbt1_copy.balanced());
  int last = *rbt1_copy.begin();
  for (int value : rbt1_copy) {
    EXPECT_TRUE(last <= value);
  }

  tree::RedBlackTreeUnique<int> rbt2(list2);
  tree::RedBlackTreeUnique<int> rbt2_copy(std::move(rbt2));
  EXPECT_EQ(rbt2.size(), 0);
  EXPECT_EQ(rbt2_copy.size(), list2.size());
  EXPECT_TRUE(rbt2.balanced());
  EXPECT_TRUE(rbt2_copy.balanced());
  last = *rbt2_copy.begin();
  for (int value : rbt2_copy) {
    EXPECT_TRUE(last <= value);
  }

  tree::RedBlackTreeUnique<int> rbt3(list3);
  tree::RedBlackTreeUnique<int> rbt3_copy(std::move(rbt3));
  EXPECT_TRUE(rbt3.balanced());
  EXPECT_TRUE(rbt3_copy.balanced());
  EXPECT_EQ(rbt3.size(), 0);
  EXPECT_EQ(rbt3_copy.size(), list3.size());
  EXPECT_EQ(rbt3_copy.begin(), rbt3_copy.end());
}

TEST(Iterators, IteratorOperations) {
  tree::RedBlackTreeUnique<int> rbt1;
  EXPECT_TRUE(rbt1.begin() == rbt1.end());
  EXPECT_FALSE(rbt1.begin() != rbt1.end());
  EXPECT_TRUE(rbt1.begin() == rbt1.end() - rbt1.size());

  tree::RedBlackTreeUnique<int> rbt2({std::numeric_limits<int>::max(), 0});
  EXPECT_EQ(*rbt2.begin(), 0);
  EXPECT_EQ(*(rbt2.end() - 2), 0);
  EXPECT_EQ(*((++rbt2.begin()) - 1), 0);
  EXPECT_EQ(*(++rbt2.begin()), std::numeric_limits<int>::max());
  EXPECT_EQ(*(--rbt2.end()), std::numeric_limits<int>::max());
  EXPECT_FALSE(--rbt2.end() == rbt2.end());
  EXPECT_TRUE(rbt2.begin() != rbt2.end());
  EXPECT_TRUE(rbt2.begin() + rbt2.size() == rbt2.end());
  EXPECT_TRUE(rbt2.begin() == rbt2.end() - rbt2.size());

  tree::RedBlackTreeUnique<int> rbt3({3, 1, 8, 6, 4});
  EXPECT_EQ(*rbt3.begin(), 1);
  EXPECT_EQ(*(--rbt3.end()), 8);
  EXPECT_EQ(*rbt3.end(), std::numeric_limits<int>::max());
  EXPECT_FALSE(rbt3.begin() == rbt3.end());
  EXPECT_TRUE(rbt3.begin() + rbt3.size() == rbt3.end());
  EXPECT_TRUE(rbt3.begin() == rbt3.end() - rbt3.size());

  tree::RedBlackTreeUnique<std::vector<int>> rbt4;
  rbt4.insert(std::vector<int>({1, 2, 3}));
  EXPECT_EQ(rbt4.size(), 1);
  EXPECT_EQ(rbt4.begin()->size(), 3);
  rbt4.begin()->push_back(5);
  EXPECT_EQ(rbt4.begin()->size(), 4);
}

TEST(Capacity, Empty) {
  std::initializer_list<int> list1{1, 2, 3}, list2{5, 1, 9, 10, 2}, list3{1},
      list4{};

  tree::RedBlackTreeUnique<int> rbt1(list1);
  EXPECT_FALSE(rbt1.empty());

  tree::RedBlackTreeUnique<int> rbt2(list2);
  EXPECT_FALSE(rbt2.empty());

  tree::RedBlackTreeUnique<int> rbt3(list3);
  EXPECT_FALSE(rbt3.empty());

  tree::RedBlackTreeUnique<int> rbt4(list4);
  EXPECT_TRUE(rbt4.empty());
}

TEST(Capacity, Size) {
  std::initializer_list<int> list1{1, 2, 3}, list2{5, 1, 9, 10, 2}, list3{1},
      list4{};

  tree::RedBlackTreeUnique<int> rbt1(list1);
  EXPECT_EQ(rbt1.size(), list1.size());

  tree::RedBlackTreeUnique<int> rbt2(list2);
  EXPECT_EQ(rbt2.size(), list2.size());

  tree::RedBlackTreeUnique<int> rbt3(list3);
  EXPECT_EQ(rbt3.size(), list3.size());

  tree::RedBlackTreeUnique<int> rbt4(list4);
  EXPECT_EQ(rbt4.size(), list4.size());
}

TEST(Modifiers, Clear) {
  std::initializer_list<int> list1{1, 2, 3}, list2{5, 1, 9, 10, 2}, list3{1},
      list4{};

  tree::RedBlackTreeUnique<int> rbt1(list1);
  EXPECT_FALSE(rbt1.empty());
  EXPECT_TRUE(rbt1.balanced());
  rbt1.clear();
  EXPECT_TRUE(rbt1.balanced());
  EXPECT_TRUE(rbt1.empty());

  tree::RedBlackTreeUnique<int> rbt2(list2);
  EXPECT_FALSE(rbt2.empty());
  EXPECT_TRUE(rbt2.balanced());
  rbt2.clear();
  EXPECT_TRUE(rbt2.balanced());
  EXPECT_TRUE(rbt2.empty());

  tree::RedBlackTreeUnique<int> rbt3(list3);
  EXPECT_FALSE(rbt3.empty());
  EXPECT_TRUE(rbt3.balanced());
  rbt3.clear();
  EXPECT_TRUE(rbt3.balanced());
  EXPECT_TRUE(rbt3.empty());

  tree::RedBlackTreeUnique<int> rbt4(list4);
  EXPECT_TRUE(rbt4.empty());
  EXPECT_TRUE(rbt4.balanced());
  rbt4.clear();
  EXPECT_TRUE(rbt4.balanced());
  EXPECT_TRUE(rbt4.empty());
}

TEST(Modifiers, Insert) {
  tree::RedBlackTreeUnique<int> rbt1({4, 1, 3});
  rbt1.insert(0);
  EXPECT_TRUE(rbt1.balanced());
  EXPECT_EQ(*rbt1.begin(), 0);
  rbt1.insert(-2);
  EXPECT_TRUE(rbt1.balanced());
  EXPECT_EQ(*rbt1.begin(), -2);
  EXPECT_TRUE(rbt1.balanced());
  EXPECT_EQ(rbt1.insert(4), rbt1.end());
  EXPECT_TRUE(rbt1.balanced());
  EXPECT_EQ(rbt1.size(), 5);
  EXPECT_TRUE(rbt1.balanced());
  int last = *rbt1.begin();
  for (int v : rbt1) {
    EXPECT_TRUE(last <= v);
  }

  tree::RedBlackTreeUnique<int> rbt2;
  rbt2.insert(-9);
  EXPECT_TRUE(rbt1.balanced());
  EXPECT_EQ(*rbt2.begin(), -9);
  EXPECT_EQ(*(--rbt2.end()), -9);
  rbt2.insert(0);
  EXPECT_TRUE(rbt1.balanced());
  EXPECT_EQ(*(--rbt2.end()), 0);
  EXPECT_EQ(rbt2.size(), 2);
  EXPECT_EQ(rbt2.insert(0), rbt2.end());
  EXPECT_TRUE(rbt1.balanced());
  EXPECT_EQ(rbt2.size(), 2);
  rbt2.insert(-99);
  EXPECT_TRUE(rbt1.balanced());
  EXPECT_EQ(*rbt2.begin(), -99);
  EXPECT_EQ(rbt2.size(), 3);
  rbt2.insert(std::numeric_limits<int>::max());
  EXPECT_TRUE(rbt1.balanced());
  EXPECT_EQ(rbt2.size(), 4);
  EXPECT_EQ(*(--rbt2.end()), std::numeric_limits<int>::max());
}

TEST(Modifiers, Erase) {
  tree::RedBlackTreeUnique<int> rbt1{1, 2, 3};
  EXPECT_EQ(rbt1.erase(9), rbt1.end());
  EXPECT_TRUE(rbt1.balanced());
  EXPECT_EQ(*rbt1.erase(2), 3);
  EXPECT_TRUE(rbt1.balanced());
  EXPECT_EQ(*(++rbt1.begin()), 3);
  EXPECT_EQ(*(rbt1.end() - 2), 1);
  EXPECT_EQ(rbt1.erase(3), rbt1.end());
  EXPECT_TRUE(rbt1.balanced());
  EXPECT_TRUE(++rbt1.begin() == rbt1.end());
  EXPECT_EQ(*(--rbt1.end()), 1);
  EXPECT_EQ(rbt1.erase(1), rbt1.end());
  EXPECT_TRUE(rbt1.balanced());
  EXPECT_TRUE(rbt1.begin() == rbt1.end());
  EXPECT_TRUE(rbt1.empty());

  tree::RedBlackTreeUnique<int> rbt2{5, 1, 9, 10, 2};
  EXPECT_EQ(rbt2.erase(99), rbt2.end());
  EXPECT_TRUE(rbt2.balanced());
  EXPECT_EQ(*rbt2.erase(10), rbt2.end());
  EXPECT_TRUE(rbt2.balanced());
  EXPECT_EQ(*rbt2.erase(1), 2);
  EXPECT_TRUE(rbt2.balanced());
  EXPECT_EQ(*rbt2.begin(), 2);
  EXPECT_EQ(*(--rbt2.end()), 9);
  EXPECT_EQ(*rbt2.end(), std::numeric_limits<int>::max());
  EXPECT_EQ(rbt2.size(), 3);
  int last = *rbt2.begin();
  for (int value : rbt2) {
    EXPECT_TRUE(value >= last);
    last = value;
  }

  tree::RedBlackTreeUnique<int> rbt3{};
  EXPECT_EQ(rbt3.erase(4), rbt3.end());
  EXPECT_TRUE(rbt3.balanced());
  EXPECT_TRUE(rbt3.erase(1));
  EXPECT_TRUE(rbt3.balanced());
  EXPECT_EQ(rbt3.size(), 0);
  EXPECT_TRUE(rbt3.begin() == rbt3.end());

  tree::RedBlackTreeUnique<int> rbt4{};
  EXPECT_EQ(rbt4.erase(4), rbt4.end());
  EXPECT_TRUE(rbt4.balanced());
  EXPECT_EQ(rbt4.erase(3), rbt4.end());
  EXPECT_TRUE(rbt4.balanced());
  EXPECT_EQ(*rbt4.erase(9), rbt4.end());
  EXPECT_TRUE(rbt4.balanced());
  EXPECT_TRUE(rbt4.empty());

  tree::RedBlackTreeUnique<int> rbt5{9, 4, 7, 1, 3};
  EXPECT_TRUE(rbt5.erase(rbt5.begin()));
  EXPECT_TRUE(rbt5.balanced());
  EXPECT_EQ(*rbt5.begin(), 3);
  EXPECT_EQ(rbt5.erase(rbt5.end()), rbt5.end());
  EXPECT_TRUE(rbt5.balanced());
  EXPECT_EQ(rbt5.size(), 4);
}

TEST(LookUp, balenced) {
  tree::RedBlackTreeUnique<int> rbt_increasing, rbt_decreasing, rbt_random;
  for (int value{0}; value < 1000; ++value) {
    rbt_increasing.insert(value);
    EXPECT_TRUE(rbt_increasing.balanced());

    rbt_decreasing.insert(1000 - value);
    EXPECT_TRUE(rbt_decreasing.balanced());

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> generator(
        std::numeric_limits<int>::min(), std::numeric_limits<int>::max());
    rbt_random.insert(generator(gen));
    EXPECT_TRUE(rbt_random.balanced());
  }
  auto it_increasing = rbt_increasing.begin(),
       it_decreasing = rbt_decreasing.begin(), it_random = rbt_random.begin();
  for (int value{1000}; value >= 0; --value) {
    it_increasing = rbt_increasing.erase(it_increasing);
    EXPECT_TRUE(rbt_increasing.balanced());

    it_decreasing = rbt_decreasing.erase(it_decreasing);
    EXPECT_TRUE(rbt_increasing.balanced());

    it_random = rbt_random.erase(it_random);
    EXPECT_TRUE(rbt_increasing.balanced());
  }
}

TEST(LookUp, find) {
  tree::RedBlackTreeUnique<int> rbt1{9, 2, 5, 3, 1};
  tree::RedBlackTreeUnique<int>::iterator it = rbt1.find(1);
  EXPECT_EQ(it, rbt1.begin());
  EXPECT_EQ(*it, 1);
  it = rbt1.find(9);
  EXPECT_EQ(*it, 9);
  EXPECT_EQ(it, --rbt1.end());
  it = rbt1.find(3);
  EXPECT_EQ(*it, 3);
  it = rbt1.find(11);
  EXPECT_EQ(it, rbt1.end());

  tree::RedBlackTreeUnique<int> rbt2{};
  EXPECT_EQ(rbt2.find(1), rbt2.end());
}

TEST(LookUp, lower_bound) {
  tree::RedBlackTreeUnique<int> rbt1{9, 2, 6, 1, 4, 11};
  tree::RedBlackTreeUnique<int>::iterator it = rbt1.lower_bound(1);
  EXPECT_EQ(it, rbt1.begin());
  EXPECT_EQ(*it, 1);
  it = rbt1.lower_bound(10);
  EXPECT_EQ(*it, 11);
  EXPECT_EQ(it, --rbt1.end());
  it = rbt1.lower_bound(5);
  EXPECT_EQ(*it, 6);
  it = rbt1.lower_bound(6);
  EXPECT_EQ(*it, 6);
  it = rbt1.lower_bound(12);
  EXPECT_EQ(it, rbt1.end());

  tree::RedBlackTreeUnique<int> rbt2;
  it = rbt2.lower_bound(1);
  EXPECT_EQ(it, rbt2.end());
}

TEST(LookUp, upper_bound) {
  tree::RedBlackTreeUnique<int> rbt1{9, 2, 6, 1, 4, 11};
  tree::RedBlackTreeUnique<int>::iterator it = rbt1.upper_bound(0);
  EXPECT_EQ(it, rbt1.begin());
  EXPECT_EQ(*it, 1);
  it = rbt1.upper_bound(10);
  EXPECT_EQ(*it, 11);
  EXPECT_EQ(it, --rbt1.end());
  it = rbt1.upper_bound(5);
  EXPECT_EQ(*it, 6);
  it = rbt1.upper_bound(9);
  EXPECT_EQ(*it, 11);
  it = rbt1.upper_bound(12);
  EXPECT_EQ(it, rbt1.end());

  tree::RedBlackTreeUnique<int> rbt2;
  it = rbt2.upper_bound(1);
  EXPECT_EQ(it, rbt2.end());
}

int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
