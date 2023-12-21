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

TEST(SpecialFunctions, DefaultConstructor) {
  lib::list<int> list1;
  ASSERT_TRUE(list1.empty());
  ASSERT_EQ(list1.size(), 0);

  lib::list<char> list2(4);
  ASSERT_FALSE(list2.empty());
  ASSERT_EQ(list2.size(), 4);
  for (auto v : list2) {
    ASSERT_EQ(v, char());
  }

  lib::list<std::string> list3(9, "exemple");
  ASSERT_FALSE(list3.empty());
  ASSERT_EQ(list3.size(), 9);
  for (auto v : list3) {
    EXPECT_EQ(v, "exemple");
  }
}

TEST(SpecialFunctions, CopyAssign) {
  lib::list<int> list1{1, 2, 3}, list2, list3;
  list2 = list1;
  ASSERT_EQ(list2.size(), list1.size());
  int counter{0};
  for (auto v : list2) {
    ASSERT_EQ(v, ++counter);
  }

  list2 = list3;
  ASSERT_EQ(list2.size(), list3.size());

  list2 = list3 = list1;
  counter = 0;
  ASSERT_EQ(list2.size(), 3);
  for (auto v : list2) {
    ASSERT_EQ(v, ++counter);
  }

  ASSERT_EQ(list3.size(), 3);
  counter = 0;
  for (auto v : list3) {
    ASSERT_EQ(v, ++counter);
  }
}

TEST(SpecialFunctions, InitializerListAssign) {
  std::initializer_list<int> ilist{1, 2, 3, 4};
  lib::list<int> list1;
  list1 = ilist;
  ASSERT_EQ(list1.size(), ilist.size());
  int counter{0};
  for (auto v : list1) {
    ASSERT_EQ(v, ++counter);
  }

  std::initializer_list<int> ilist2{};
  list1 = ilist2;
  ASSERT_EQ(list1.size(), 0);
  ASSERT_TRUE(list1.empty());
}

TEST(SpecialFunctions, InitializerListConstructor) {
  lib::list<int> list1{1, 2, 3, 4, 5, 6};
  int counter{1};
  ASSERT_EQ(list1.size(), 6);
  for (auto v : list1) {
    ASSERT_EQ(v, counter++);
  }

  lib::list<std::string> list2{"first", "second", "third"};
  ASSERT_EQ(list2.size(), 3);
  auto iterator = list2.begin();
  ASSERT_EQ(*iterator++, "first");
  ASSERT_EQ(*iterator++, "second");
  ASSERT_EQ(*iterator++, "third");
}

TEST(SpecialFunctions, MoveAssing) {
  lib::list<int> list1{1, 2, 3, 4, 5, 6};
  lib::list<int> list2 = std::move(list1);
  ASSERT_EQ(list1.size(), 0);
  ASSERT_EQ(list2.size(), 6);
  int counter{1};
  for (auto it = list2.begin(); it != list2.end(); ++it) {
    ASSERT_EQ(*it, counter++);
  }
}

TEST(SpecialFunctions, RangeAssign) {
  int array[] = {1, 2, 3, 4, 5};
  lib::list<int> list1;
  list1.assign(array, array + 5);
  ASSERT_EQ(list1.size(), 5);
  int counter{0};
  for (auto v : list1) {
    ASSERT_EQ(v, ++counter);
  }

  list1.clear();
  list1.assign(array, array);
  ASSERT_EQ(list1.size(), 0);
}

TEST(SpecialFunctions, RangeConstructor) {
  int array[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
  lib::list<int> list1(array, array + 9);
  int counter{1};
  EXPECT_EQ(list1.size(), 9);
  for (auto v : list1) {
    EXPECT_EQ(v, counter++);
  }
}

TEST(SpecialFunctions, CopyConstructor) {
  lib::list<int> list1{1, 2, 3, 4};
  lib::list<int> list2(list1);
  EXPECT_EQ(list2.size(), 4);
  int counter{1};
  for (auto v : list2) {
    EXPECT_EQ(v, counter++);
  }
}

TEST(SpecialFunctions, MoveConstructor) {
  lib::list<int> list1{1, 2, 3, 4, 5, 6};
  lib::list<int> list2{std::move(list1)};
  EXPECT_EQ(list1.size(), 0);
  EXPECT_EQ(list2.size(), 6);
  int counter{1};
  for (auto v : list2) {
    EXPECT_EQ(v, counter++);
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

TEST(Capacity, empty) {
  lib::list<int> list1{1, 2, 3}, list2;
  EXPECT_FALSE(list1.empty());
  EXPECT_TRUE(list2.empty());
}

TEST(Capacity, size) {
  lib::list<int> list1{1, 2, 3}, list2;
  EXPECT_EQ(list1.size(), 3);
  EXPECT_EQ(list2.size(), 0);
}
int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
