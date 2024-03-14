#include "Vector/Vector.h"
#include "gtest/gtest.h"
#include <initializer_list>
#include <vector>

/*
 * The "lib" macro can be used to select which lib to test. To ensure the
 * integrity of the tests, the "std" namespace must be used, to effectively test
 * the vector developed the "sc" namespace must be used. Note: the test for the
 * function "push_front" does not use the "lib" macro, because it is not defined
 * in "std", instead the namespace "sc" is used directly.
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

TEST(ElementAccess, front) {
  lib::vector<int> list1{100, 200, 300, 400};
  EXPECT_EQ(list1.front(), 100);
  list1.front() = 900;
  EXPECT_EQ(list1.front(), 900);

  const lib::vector<int> list2{1, 2, 4, 8, 16};
  EXPECT_EQ(list2.front(), 1);
}

TEST(ElementAccess, back) {
  lib::vector<int> list1{1, 2, 3, 4, 5};
  EXPECT_EQ(list1.back(), 5);
  list1.back() = 10;
  EXPECT_EQ(list1.back(), 10);

  const lib::vector<int> list2{1, 2, 3, 4, 5};
  EXPECT_EQ(list2.back(), 5);
}

TEST(ElementAccess, data) {
  lib::vector<int> list1{1, 2, 3, 4, 5};
  EXPECT_EQ(list1.front(), 1);
  *list1.data() = 100;
  EXPECT_EQ(list1.front(), 100);
}

TEST(ElementAccess, at) {
  lib::vector<int> list1{1, 2, 3, 4, 5};
  for (int index{0}; index < 5; ++index) {
    EXPECT_EQ(list1.at(index), index + 1);
  }
  for (int index{0}; index < 5; ++index) {
    EXPECT_EQ(list1[index], index + 1);
  }
  list1.at(0) = 99;
  EXPECT_EQ(list1.at(0), 99);
  list1[1] = 100;
  EXPECT_EQ(list1[1], 100);

  const lib::vector<int> list2{1, 2, 3, 4, 5};
  EXPECT_EQ(list2[1], 2);
}

TEST(Iterators, begin) {
  lib::vector<int> list1{1, 2, 3, 4, 5, 6};
  EXPECT_EQ(*list1.begin(), 1);
  *list1.begin() = 99;
  EXPECT_EQ(*list1.begin(), 99);
}

TEST(Iterators, end) {
  lib::vector<int> list1{1, 2, 3, 4, 5, 6};
  EXPECT_EQ(*(list1.end() - 1), 6);
  int expected{1};
  for (auto value : list1) {
    EXPECT_EQ(value, expected++);
  }
}

TEST(Iterators, Increment) {
  lib::vector<int> list1{1, 2, 3, 4, 5};
  int expected{1};
  for (auto it = list1.begin(); it != list1.end(); ++it, ++expected) {
    EXPECT_EQ(*it, expected);
  }
  expected = 1;
  for (auto it = list1.begin(); it != list1.end(); it++, ++expected) {
    EXPECT_EQ(*it, expected);
  }
  EXPECT_EQ(*(list1.begin() + 3), 4);
  EXPECT_EQ(*(2 + list1.begin()), 3);
  *(2 + list1.begin()) = 100;
  EXPECT_EQ(*(2 + list1.begin()), 100);
}

TEST(Iterators, Decrement) {
  lib::vector<int> list1{1, 2, 3, 4, 5};
  int expected{5};
  for (auto it = list1.end() - 1; it != list1.begin(); --it, --expected) {
    EXPECT_EQ(*it, expected);
  }
  expected = 5;
  for (auto it = list1.end() - 1; it != list1.begin(); it--, --expected) {
    EXPECT_EQ(*it, expected);
  }
  EXPECT_EQ(*(list1.end() - 2), 4);
  *(list1.end() - 2) = 99;
  EXPECT_EQ(*(list1.end() - 2), 99);
}

TEST(Iterators, Difference) {
  lib::vector<int> list1{1, 2, 3};
  EXPECT_EQ((list1.end() - list1.begin()), list1.size());
}

TEST(Iterators, ArrowOperator) {
  lib::vector<std::vector<int>> list1;
  list1.push_back(std::vector<int>());
  EXPECT_FALSE(list1.empty());
  EXPECT_TRUE(list1.begin()->empty());
  list1.begin()->push_back(5);
  EXPECT_EQ(list1.begin()->front(), 5);
}

TEST(Iterators, EqualOperator) {
  lib::vector<int> list1{1, 2, 3};
  auto it = list1.begin();
  EXPECT_EQ(*it, 1);
  auto it2 = it;
  *it2 = 99;
  EXPECT_EQ(*it2, 99);
}

TEST(Iterators, EquivalentOperator) {
  lib::vector<int> list1{100, 200, 300};
  EXPECT_TRUE(list1.begin() == list1.begin());
  EXPECT_TRUE(list1.end() == list1.end());
  EXPECT_FALSE(list1.begin() == list1.end());
  EXPECT_FALSE(list1.begin() == list1.begin() + 1);
}

TEST(Iterators, DifferenceOperator) {
  lib::vector<int> list1{1, 2, 3};
  EXPECT_FALSE(list1.begin() != list1.begin());
  EXPECT_FALSE(list1.end() != list1.end());
  EXPECT_TRUE(list1.begin() != list1.end());
  EXPECT_TRUE(list1.begin() != list1.begin() + 1);
  EXPECT_TRUE(list1.end() != list1.end() + 1);
}

TEST(Capacity, empty) {
  lib::vector<int> list1{1, 2, 3};
  EXPECT_FALSE(list1.empty());
  list1.clear();
  EXPECT_TRUE(list1.empty());
  lib::vector<int> list2;
  EXPECT_TRUE(list2.empty());
}

TEST(Capacity, size) {
  lib::vector<int> list1;
  EXPECT_EQ(list1.size(), 0);
}

TEST(Capacity, reserve) {
  lib::vector<int> list1;
  EXPECT_EQ(list1.capacity(), 0);
  for (int counter{1}; counter <= 8; counter *= 2) {
    list1.reserve(counter);
    EXPECT_EQ(list1.capacity(), counter);
  }
  list1.reserve(0);
  EXPECT_EQ(list1.capacity(), 8);
}

TEST(Capacity, capacity) {
  lib::vector<int> list1;
  EXPECT_EQ(list1.capacity(), 0);
  list1.reserve(6);
  EXPECT_EQ(list1.capacity(), 6);
  list1.reserve(3);
  EXPECT_EQ(list1.capacity(), 6);
  list1.reserve(8);
  EXPECT_EQ(list1.capacity(), 8);
}

TEST(Capacity, shrink_to_fit) {
  lib::vector<int> list1{1, 2, 3, 4, 5, 6, 7, 8, 9};
  EXPECT_EQ(list1.capacity(), 9);
  list1.shrink_to_fit();
  EXPECT_EQ(list1.capacity(), 9);
  list1.resize(6);
  list1.shrink_to_fit();
  EXPECT_EQ(list1.capacity(), 6);
}

TEST(Modifiers, clear) {
  lib::vector<int> list1{1, 2, 3};
  EXPECT_FALSE(list1.begin() == list1.end());
  EXPECT_EQ(list1.size(), 3);
  list1.clear();
  EXPECT_TRUE(list1.begin() == list1.end());
  EXPECT_EQ(list1.size(), 0);
}

TEST(Modifiers, InsertValue) {
  lib::vector<int> list1;
  for (int counter{0}; counter < 10; ++counter) {
    EXPECT_EQ(list1.size(), counter);
    list1.insert(list1.end(), counter);
    EXPECT_EQ(list1.back(), counter);
  }
  EXPECT_EQ(list1.size(), 10);
  list1.insert(list1.begin(), 99);
  EXPECT_EQ(list1.front(), 99);
  EXPECT_EQ(list1.size(), 11);
}

TEST(Modifiers, InsertCopies) {
  lib::vector<int> list1;
  list1.insert(list1.begin(), static_cast<size_t>(5), 99);
  EXPECT_EQ(list1.size(), 5);
  for (int v : list1) {
    EXPECT_EQ(v, 99);
  }
  list1.insert(list1.end(), static_cast<size_t>(6), 22);
  EXPECT_EQ(list1.size(), 11);
  for (unsigned int index{0}; index < list1.size(); ++index) {
    if (index < 5) {
      EXPECT_EQ(list1[index], 99);
    } else {
      EXPECT_EQ(list1[index], 22);
    }
  }
  list1.insert(list1.begin(), static_cast<size_t>(6), 11);
  EXPECT_EQ(list1.size(), 17);
  for (unsigned int index{0}; index < list1.size(); ++index) {
    if (index < 6) {
      EXPECT_EQ(list1[index], 11);
    } else if (index < 11) {
      EXPECT_EQ(list1[index], 99);
    } else {
      EXPECT_EQ(list1[index], 22);
    }
  }
}

TEST(Modifiers, InsertRange) {
  int array[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
  lib::vector<int> list1;
  list1.insert(list1.end(), array, array + 9);
  EXPECT_EQ(list1.size(), 9);
  for (unsigned int index{0}; index < list1.size(); ++index) {
    EXPECT_EQ(list1[index], index + 1);
  }
  list1.clear();
  list1.insert(list1.end(), array + 3, array + 6);
  EXPECT_EQ(list1.size(), 3);
  for (unsigned int index{0}; index < list1.size(); ++index) {
    EXPECT_EQ(list1[index], index + 4);
  }
  list1.insert(list1.begin(), array, array + 3);
  EXPECT_EQ(list1.size(), 6);
  for (unsigned int index{0}; index < list1.size(); ++index) {
    EXPECT_EQ(list1[index], index + 1);
  }
  list1.insert(list1.end(), array + 6, array + 9);
  EXPECT_EQ(list1.size(), 9);
  for (unsigned int index{0}; index < list1.size(); ++index) {
    EXPECT_EQ(list1[index], index + 1);
  }
}

TEST(Modifiers, InsertInitializerList) {
  std::initializer_list<int> ilist1 = {1, 2, 3};
  std::initializer_list<int> ilist2 = {4, 5, 6};
  std::initializer_list<int> ilist3 = {7, 8, 9};
  lib::vector<int> list1;
  list1.insert(list1.end(), ilist2);
  EXPECT_EQ(list1.size(), 3);
  for (unsigned int index{0}; index < list1.size(); ++index) {
    EXPECT_EQ(list1[index], index + 4);
  }
  list1.insert(list1.end(), ilist3);
  EXPECT_EQ(list1.size(), 6);
  for (unsigned int index{0}; index < list1.size(); ++index) {
    EXPECT_EQ(list1[index], index + 4);
  }
  list1.insert(list1.begin(), ilist1);
  EXPECT_EQ(list1.size(), 9);
  for (unsigned int index{0}; index < list1.size(); ++index) {
    EXPECT_EQ(list1[index], index + 1);
  }
}

TEST(Modifiers, EraseValue) {
  lib::vector<int> list1{1, 2, 3, 4, 5};
  int counter{5}, expected{1};
  while (list1.size() > 0) {
    EXPECT_EQ(list1.size(), counter--);
    EXPECT_EQ(*list1.begin(), expected++);
    list1.erase(list1.begin());
  }
}

TEST(Modifiers, EraseRange) {
  lib::vector<int> list1{1, 2, 3};
  EXPECT_EQ(list1.size(), 3);
  list1.erase(list1.begin(), list1.end());
  EXPECT_EQ(list1.size(), 0);

  lib::vector<int> list2{1, 2, 3, 4, 5, 6, 7, 8, 9};
  EXPECT_EQ(list2.size(), 9);
  list2.erase(list2.begin(), list2.begin() + 3);
  EXPECT_EQ(list2.size(), 6);
}

TEST(Modifiers, resize) {
  lib::vector<int> list1;
  EXPECT_EQ(list1.size(), 0);
  for (int counter{1}; counter < 9; counter *= 2) {
    list1.resize(counter);
    EXPECT_EQ(list1.size(), counter);
  }
  list1.resize(3);
  EXPECT_EQ(list1.size(), 3);
}

TEST(Modifiers, push_back) {
  lib::vector<int> list1;
  for (int counter{0}; counter < 5; ++counter) {
    EXPECT_EQ(list1.size(), counter);
    list1.push_back(counter);
    EXPECT_EQ(*(--list1.end()), counter);
  }
}

TEST(Modifiers, push_front) {
  sc::vector<int> list1;
  for (int counter{0}; counter < 5; ++counter) {
    EXPECT_EQ(list1.size(), counter);
    list1.push_front(counter);
    EXPECT_EQ(*(list1.begin()), counter);
  }
}

int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
