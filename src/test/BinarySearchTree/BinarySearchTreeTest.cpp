#include "BinarySearchTree/BinarySearchTree.h"
#include "gtest/gtest.h"
#include <initializer_list>
#include <limits> // numeric_limits
#include <string>
#include <vector>

TEST(SpecialFunctions, DefaultConstructor) {
  tree::BST<int> btree1;
  EXPECT_EQ(btree1.size(), 0);
  EXPECT_TRUE(btree1.empty());

  tree::BST<char> btree2;
  EXPECT_EQ(btree2.size(), 0);
  EXPECT_TRUE(btree2.empty());

  tree::BST<std::string> btree3;
  EXPECT_EQ(btree3.size(), 0);
  EXPECT_TRUE(btree3.empty());
}

TEST(SpecialFunctions, InitializerListConstructor) {
  tree::BST<int> btree1({1, 2, 3, 4});
  EXPECT_EQ(btree1.size(), 4);
  int counter{0};
  for (int value : btree1) {
    EXPECT_EQ(value, ++counter);
  }

  tree::BST<int> btree2({4, 1, 5, 6, 3, 7, 2, 2});
  EXPECT_EQ(btree2.size(), 8);
  int last = *btree2.begin();
  for (int value : btree2) {
    EXPECT_TRUE(value >= last);
    last = value;
  }

  tree::BST<int> btree3({});
  EXPECT_EQ(btree3.size(), 0);
  EXPECT_TRUE(btree3.empty());
  EXPECT_EQ(btree3.begin(), btree3.end());
}

TEST(SpecialFunctions, RangeConstructor) {
  int array[] = {9, 1, 4, 2, 5, 99, 3, 7};

  tree::BST<int> btree1(array, array);
  EXPECT_TRUE(btree1.empty());
  EXPECT_EQ(btree1.begin(), btree1.end());

  tree::BST<int> btree2(array, array + 4);
  EXPECT_EQ(btree2.size(), 4);
  auto it = btree2.begin();
  EXPECT_EQ(*(it++), 1);
  EXPECT_EQ(*(it++), 2);
  EXPECT_EQ(*(it++), 4);
  EXPECT_EQ(*(it++), 9);

  tree::BST<int> btree3(array, array + 8);
  EXPECT_EQ(btree3.size(), 8);
  int last = *btree3.begin();
  for (int value : btree3) {
    EXPECT_TRUE(last <= value);
    last = value;
  }
}

TEST(SpecialFunctions, CopyConstructor) {
  tree::BST<int> btree1({1, 2, 3, 4});
  tree::BST<int> btree1_copy(btree1);
  EXPECT_EQ(btree1.size(), btree1_copy.size());
  for (auto it = btree1.begin(), it_copy = btree1_copy.begin();
       it != btree1.end(); ++it, ++it_copy) {
    EXPECT_EQ(*it, *it_copy);
  }

  tree::BST<int> btree2{4, 1, 5, 6, 3, 7, 2, 2};
  tree::BST<int> btree2_copy(btree2);
  EXPECT_EQ(btree2.size(), btree2_copy.size());
  for (auto it = btree2.begin(), it_copy = btree2_copy.begin();
       it != btree2.end(); ++it, ++it_copy) {
    EXPECT_EQ(*it, *it_copy);
  }

  tree::BST<int> btree3({});
  tree::BST<int> btree3_copy(btree3);
  EXPECT_EQ(btree3.size(), btree3_copy.size());
  for (auto it = btree3.begin(), it_copy = btree3_copy.begin();
       it != btree3.end(); ++it, ++it_copy) {
    EXPECT_EQ(*it, *it_copy);
  }
}

TEST(SpecialFunctions, MoveConstructor) {
  tree::BST<int> btree1{1, 2, 3, 4};
  tree::BST<int> btree1_copy(std::move(btree1));
  EXPECT_EQ(btree1.size(), 0);
  EXPECT_EQ(btree1_copy.size(), 4);
  int last = *btree1_copy.begin();
  for (int value : btree1_copy) {
    EXPECT_TRUE(last <= value);
  }

  tree::BST<int> btree2{4, 1, 5, 6, 3, 7, 2, 2};
  tree::BST<int> btree2_copy(std::move(btree2));
  EXPECT_EQ(btree2.size(), 0);
  EXPECT_EQ(btree2_copy.size(), 8);
  last = *btree2_copy.begin();
  for (int value : btree2_copy) {
    EXPECT_TRUE(last <= value);
  }

  tree::BST<int> btree3;
  tree::BST<int> btree3_copy(std::move(btree3));
  EXPECT_EQ(btree3.size(), 0);
  EXPECT_EQ(btree3_copy.size(), 0);
  EXPECT_EQ(btree3_copy.begin(), btree3_copy.end());
}

TEST(Iterators, IteratorOperations) {
  tree::BST<int> btree1;
  EXPECT_TRUE(btree1.begin() == btree1.end());
  EXPECT_FALSE(btree1.begin() != btree1.end());
  EXPECT_TRUE(btree1.begin() == btree1.end() - btree1.size());

  tree::BST<int> btree2({std::numeric_limits<int>::max(), 0});
  EXPECT_EQ(*btree2.begin(), 0);
  EXPECT_EQ(*(btree2.end() - 2), 0);
  EXPECT_EQ((&btree2.begin())->key, 0);
  EXPECT_EQ(*((++btree2.begin()) - 1), 0);
  EXPECT_EQ(*(++btree2.begin()), std::numeric_limits<int>::max());
  EXPECT_EQ(*(--btree2.end()), std::numeric_limits<int>::max());
  EXPECT_FALSE(--btree2.end() == btree2.end());
  EXPECT_TRUE(btree2.begin() != btree2.end());
  EXPECT_TRUE(btree2.begin() + btree2.size() == btree2.end());
  EXPECT_TRUE(btree2.begin() == btree2.end() - btree2.size());

  tree::BST<int> btree3({3, 1, 8, 6, 4});
  EXPECT_EQ(*btree3.begin(), 1);
  EXPECT_EQ(*(--btree3.end()), 8);
  EXPECT_EQ((&btree3.begin())->key, 1);
  EXPECT_EQ(*btree3.end(), std::numeric_limits<int>::max());
  EXPECT_FALSE(btree3.begin() == btree3.end());
  EXPECT_TRUE(btree3.begin() + btree3.size() == btree3.end());
  EXPECT_TRUE(btree3.begin() == btree3.end() - btree3.size());

  tree::BST<std::vector<int>> btree4;
  btree4.insert(std::vector<int>({1, 2, 3}));
  EXPECT_EQ(btree4.size(), 1);
  EXPECT_EQ(btree4.begin()->size(), 3);
  btree4.begin()->push_back(5);
  EXPECT_EQ(btree4.begin()->size(), 4);
}

TEST(Capacity, Empty) {
  tree::BST<int> btree1({1, 2, 3});
  EXPECT_FALSE(btree1.empty());

  tree::BST<int> btree2({5, 1, 9, 10, 2});
  EXPECT_FALSE(btree2.empty());

  tree::BST<int> btree3({1});
  EXPECT_FALSE(btree3.empty());

  tree::BST<int> btree4({});
  EXPECT_TRUE(btree4.empty());
}

TEST(Capacity, Size) {
  tree::BST<int> btree1({1, 2, 3});
  EXPECT_EQ(btree1.size(), 3);

  tree::BST<int> btree2({5, 1, 9, 10, 2});
  EXPECT_EQ(btree2.size(), 5);

  tree::BST<int> btree3({1});
  EXPECT_EQ(btree3.size(), 1);

  tree::BST<int> btree4({});
  EXPECT_EQ(btree4.size(), 0);
}

TEST(Modifiers, Clear) {
  tree::BST<int> btree1({1, 2, 3});
  EXPECT_FALSE(btree1.empty());
  btree1.clear();
  EXPECT_TRUE(btree1.empty());

  tree::BST<int> btree2({5, 1, 9, 10, 2});
  EXPECT_FALSE(btree2.empty());
  btree2.clear();
  EXPECT_TRUE(btree2.empty());

  tree::BST<int> btree3({1});
  EXPECT_FALSE(btree3.empty());
  btree3.clear();
  EXPECT_TRUE(btree3.empty());

  tree::BST<int> btree4({});
  EXPECT_TRUE(btree4.empty());
  btree4.clear();
  EXPECT_TRUE(btree4.empty());
}

TEST(Modifiers, Insert) {
  tree::BST<int> btree1({4, 1, 3});
  btree1.insert(0);
  EXPECT_EQ(*btree1.begin(), 0);
  btree1.insert(-2);
  EXPECT_EQ(*btree1.begin(), -2);
  btree1.insert(4);
  EXPECT_EQ(btree1.size(), 6);
  int last = *btree1.begin();
  for (int v : btree1) {
    EXPECT_TRUE(last <= v);
  }

  tree::BST<int> btree2;
  btree2.insert(-9);
  EXPECT_EQ(*btree2.begin(), -9);
  EXPECT_EQ(*(--btree2.end()), -9);
  btree2.insert(0);
  EXPECT_EQ(*(--btree2.end()), 0);
  btree2.insert(0);
  EXPECT_EQ(*(--btree2.end()), 0);
  EXPECT_EQ(*(btree2.end() - 2), 0);
  btree2.insert(-99);
  EXPECT_EQ(*btree2.begin(), -99);
  btree2.insert(std::numeric_limits<int>::max());
  EXPECT_EQ(*(--btree2.end()), std::numeric_limits<int>::max());
}

TEST(Modifiers, Erase) {
  tree::BST<int> btree1{1, 2, 3};
  EXPECT_TRUE(btree1.erase(2));
  EXPECT_EQ(*(++btree1.begin()), 3);
  EXPECT_EQ(*(btree1.end() - 2), 1);
  EXPECT_TRUE(btree1.erase(3));
  EXPECT_TRUE(++btree1.begin() == btree1.end());
  EXPECT_EQ(*(--btree1.end()), 1);
  EXPECT_TRUE(btree1.erase(1));
  EXPECT_TRUE(btree1.begin() == btree1.end());
  EXPECT_TRUE(btree1.empty());

  tree::BST<int> btree2({5, 1, 9, 10, 2, 2});
  EXPECT_TRUE(btree2.erase(10));
  EXPECT_TRUE(btree2.erase(1));
  EXPECT_EQ(*btree2.begin(), 2);
  EXPECT_EQ(*(--btree2.end()), 9);
  EXPECT_EQ(*btree2.end(), std::numeric_limits<int>::max());
  EXPECT_EQ(btree2.size(), 4);
  int last = *btree2.begin();
  for (int value : btree2) {
    EXPECT_TRUE(value >= last);
    last = value;
  }

  tree::BST<int> btree3({1});
  EXPECT_FALSE(btree3.erase(4));
  EXPECT_TRUE(btree3.erase(1));
  EXPECT_EQ(btree3.size(), 0);
  EXPECT_TRUE(btree3.begin() == btree3.end());

  tree::BST<int> btree4;
  EXPECT_FALSE(btree4.erase(4));
  EXPECT_FALSE(btree4.erase(3));
  EXPECT_FALSE(btree4.erase(9));
  EXPECT_TRUE(btree4.empty());
}

int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
