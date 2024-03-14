#include "LinkedList/DoubleLinkedList.h"
#include "gtest/gtest.h"
#include <cstddef>    // size_t
#include <functional> // function<>, greater<>, less<>
#include <initializer_list>
#include <list>

/*
 * The "lib" macro can be used to select which lib to test. To ensure the
 * integrity of the tests, the "std" namespace must be used, to effectively test
 * the double linked list developed the "sc" namespace must be used. Some tests
 * do not use this macro because the functions they test are not defined in the
 * standard library, so the namespace "sc" is used directly instead.
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

TEST(ElementAccess, front) {
  lib::list<int> list1{1, 2, 3}, list2{11};
  EXPECT_EQ(list1.front(), 1);
  EXPECT_EQ(list2.front(), 11);
  list1.front() = 99;
  EXPECT_EQ(list1.front(), 99);
  const lib::list<int> list3{10, 11};
  EXPECT_EQ(list3.front(), 10);
}

TEST(ElementAccess, back) {
  lib::list<int> list1{1, 2, 3}, list2{11};
  EXPECT_EQ(list1.back(), 3);
  EXPECT_EQ(list2.back(), 11);
  list1.back() = 99;
  EXPECT_EQ(list1.back(), 99);
  const lib::list<int> list3{10, 11};
  EXPECT_EQ(list3.back(), 11);
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

TEST(Modifiers, clear) {
  lib::list<int> list1{1, 23, 45}, list2;
  list1.clear();
  EXPECT_TRUE(list1.empty());

  list2.clear();
  EXPECT_TRUE(list2.empty());
}

TEST(Modifiers, InsertValue) {
  lib::list<int> list1{1, 4, 5};
  list1.insert(list1.begin(), 0);
  ASSERT_EQ(list1.front(), 0);

  auto it = list1.insert(++(++list1.begin()), 3);
  EXPECT_TRUE(it == ++(++list1.begin()));

  it = list1.insert(it, 2);
  EXPECT_TRUE(it == ++(++list1.begin()));

  it = list1.insert(list1.end(), 6);
  EXPECT_TRUE(it == --list1.end());

  int counter{-1};
  for (auto v : list1) {
    ASSERT_EQ(v, ++counter);
  }
  ASSERT_EQ(list1.size(), 7);
}

TEST(Modifiers, InsertRange) {
  int array[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
  lib::list<int> list1;
  auto it = list1.insert(list1.end(), array, array + 9);
  EXPECT_EQ(list1.size(), 9);
  EXPECT_TRUE(it == list1.begin());
  int counter{0};
  for (auto v : list1) {
    EXPECT_EQ(v, ++counter);
  }

  list1.clear();
  list1.insert(list1.begin(), array, array + 5);
  list1.insert(list1.end(), array + 5, array + 9);

  EXPECT_EQ(list1.size(), 9);
  counter = 0;
  for (auto v : list1) {
    EXPECT_EQ(v, ++counter);
  }
}

TEST(Modifiers, InsertInitializerList) {
  std::initializer_list<int> ilist1{1, 2}, ilist2{3, 4, 5, 6, 7}, ilist3{8, 9};
  lib::list<int> list1;
  list1.insert(list1.begin(), ilist1);
  EXPECT_EQ(list1.size(), 2);
  int index{0};
  for (auto v : list1) {
    EXPECT_EQ(v, ++index);
  }

  list1.insert(list1.end(), ilist3);
  EXPECT_EQ(list1.size(), 4);
  index = 0;
  for (auto v : list1) {
    if (++index < 3) {
      EXPECT_EQ(v, index);
    } else {
      EXPECT_EQ(v, index + 5);
    }
  }

  list1.insert(++(++list1.begin()), ilist2);
  EXPECT_EQ(list1.size(), 9);
  index = 0;
  for (auto v : list1) {
    EXPECT_EQ(v, ++index);
  }
}

TEST(Modifiers, InsertCopies) {
  lib::list<int> list1;
  list1.insert(list1.begin(), static_cast<size_t>(2), 99);
  EXPECT_EQ(list1.size(), 2);
  for (auto v : list1) {
    EXPECT_EQ(v, 99);
  }

  list1.insert(list1.end(), static_cast<size_t>(6), 44);
  EXPECT_EQ(list1.size(), 8);
  int index{0};
  for (auto v : list1) {
    if (++index < 3) {
      EXPECT_EQ(v, 99);
    } else {
      EXPECT_EQ(v, 44);
    }
  }

  list1.insert(++(++list1.begin()), static_cast<size_t>(3), 22);
  EXPECT_EQ(list1.size(), 11);
  index = 0;
  for (auto v : list1) {
    if (++index < 3) {
      EXPECT_EQ(v, 99);
    } else if (index < 6) {
      EXPECT_EQ(v, 22);
    } else {
      EXPECT_EQ(v, 44);
    }
  }
}

TEST(Modifiers, EraseValue) {
  lib::list<int> list1{1, 2, 3, 4, 5, 6};

  list1.erase(list1.begin());
  EXPECT_EQ(list1.front(), 2);
  EXPECT_EQ(list1.size(), 5);

  list1.erase(--list1.end());
  EXPECT_EQ(list1.back(), 5);
  EXPECT_EQ(list1.size(), 4);

  for (auto runner = list1.begin(); runner != list1.end();) {
    runner = list1.erase(runner);
  }
  EXPECT_TRUE(list1.empty());
}

TEST(Modifiers, EraseRange) {
  lib::list<int> list1{1, 2, 3, 4, 5, 6, 7, 8, 9};
  auto returned = list1.erase(list1.begin(), list1.begin());
  EXPECT_EQ(list1.size(), 9);
  EXPECT_TRUE(returned == list1.begin());

  returned = list1.erase(list1.begin(), ++list1.begin());
  EXPECT_TRUE(returned == list1.begin());
  EXPECT_EQ(list1.size(), 8);

  auto it = ++(++list1.begin());
  returned = list1.erase(list1.begin(), it);
  EXPECT_TRUE(returned == it);
  EXPECT_EQ(list1.size(), 6);

  returned = list1.erase(list1.begin(), list1.end());
  EXPECT_TRUE(returned == list1.end());
  EXPECT_TRUE(list1.empty());
}

TEST(Modifiers, push_back) {
  lib::list<int> list1{1, 2}, list2;
  for (int counter{0}; counter < 5; ++counter) {
    list1.push_back(counter);
    EXPECT_EQ(list1.size(), counter + 3);
    EXPECT_EQ(list1.back(), counter);
  }

  list2.push_back(4);
  EXPECT_EQ(list2.size(), 1);
  EXPECT_EQ(list2.back(), 4);
}

TEST(Modifiers, push_front) {
  lib::list<int> list1{1, 2}, list2;
  for (int counter{0}; counter < 5; ++counter) {
    list1.push_front(counter);
    EXPECT_EQ(list1.size(), counter + 3);
    EXPECT_EQ(list1.front(), counter);
  }

  list2.push_front(4);
  EXPECT_EQ(list2.size(), 1);
  EXPECT_EQ(list2.front(), 4);
}

TEST(Modifiers, pop_back) {
  lib::list<int> list1{1, 2, 3, 4, 5};
  for (int counter{5}; counter > 0; --counter) {
    EXPECT_EQ(list1.back(), counter);
    list1.pop_back();
  }
  EXPECT_TRUE(list1.empty());
}

TEST(Modifiers, pop_front) {
  lib::list<int> list1{1, 2, 3, 4, 5};
  for (int counter{1}; counter <= 5; ++counter) {
    EXPECT_EQ(list1.front(), counter);
    list1.pop_front();
  }
  EXPECT_TRUE(list1.empty());
}

TEST(Modifiers, resize) {
  lib::list<int> list1{1, 2, 3, 4, 5, 6, 7, 8, 9};
  list1.resize(14);
  EXPECT_EQ(list1.size(), 14);
  int index{0};
  for (auto v : list1) {
    if (index < 9) {
      EXPECT_EQ(v, ++index);
    } else {
      EXPECT_EQ(v, int());
    }
  }

  list1.resize(4);
  EXPECT_EQ(list1.size(), 4);
  index = 0;
  for (auto v : list1) {
    EXPECT_EQ(v, ++index);
  }

  list1.resize(0);
  EXPECT_EQ(list1.size(), 0);
}

TEST(Operations, merge) {
  lib::list<int> list1{1, 3, 5, 7, 9}, list2{2, 4, 6, 8};
  list1.merge(std::move(list2));
  EXPECT_EQ(list1.size(), 9);
  int counter{0};
  for (auto v : list1) {
    EXPECT_EQ(v, ++counter);
  }

  lib::list<int> list3, list4{-2, -1, 0};
  list1.merge(std::move(list3));
  EXPECT_EQ(list1.size(), 9);
  list1.merge(std::move(list4));
  counter = -3;
  for (auto v : list1) {
    EXPECT_EQ(v, ++counter);
  }
}

TEST(Operations, splice) {
  lib::list<int> list1{2, 3, 7, 8}, list2{4, 5, 6}, list3{0, 1}, list4{9, 10};
  list1.splice(++(++list1.begin()), std::move(list2));
  EXPECT_EQ(list1.size(), 7);
  EXPECT_TRUE(list2.empty());

  list1.splice(list1.begin(), std::move(list3));
  EXPECT_EQ(list1.size(), 9);
  EXPECT_TRUE(list3.empty());

  list1.splice(list1.end(), std::move(list4));
  EXPECT_EQ(list1.size(), 11);
  EXPECT_TRUE(list4.empty());

  int counter{-1};
  for (auto v : list1) {
    EXPECT_EQ(v, ++counter);
  }

  lib::list<int> list5;
  list5.splice(list5.end(), std::move(list1));
  EXPECT_EQ(list5.size(), 11);
  EXPECT_TRUE(list1.empty());

  counter = -1;
  for (auto v : list5) {
    EXPECT_EQ(v, ++counter);
  }
}

TEST(Operations, remove) {
  sc::list<int> list1{1, 2, 3, 4, 5, 6, 7}, list2{2, 2, 2, 2, 2, 2};
  EXPECT_EQ(list1.remove(7), 1);
  EXPECT_EQ(list1.back(), 6);

  EXPECT_EQ(list2.remove(2), 6);
  EXPECT_TRUE(list2.empty());

  sc::list<int> list3{3, 3, 3, 4, 3, 3};
  EXPECT_EQ(list3.remove(3), 5);
  EXPECT_EQ(list3.size(), 1);
  EXPECT_EQ(list3.front(), 4);
}

TEST(Operations, remove_if) {
  sc::list<int> list1{1, 2, 3, 4, 5, 6, 7, 8, 9}, list2{1, 3, 5, 7, 9};
  struct function {
    static bool even(int input) { return input % 2 == 0; }
    static bool positive(int input) { return input > 0; }
  } functions;

  EXPECT_EQ(list1.remove_if(functions.even), 4);
  for (auto v : list1) {
    EXPECT_FALSE(functions.even(v));
  }

  EXPECT_EQ(list1.remove_if(functions.positive), 5);
  EXPECT_TRUE(list1.empty());

  sc::list<int> list3{-3, 3, 9, 1, -9, -12, 99, 17, 8, -8, 0};
  EXPECT_EQ(list3.remove_if(functions.positive), 6);
  for (auto v : list3) {
    EXPECT_LE(v, 0);
  }
}

TEST(Operations, reverse) {
  lib::list<int> list1{1, 2, 3, 4, 5, 6, 7, 8, 9};
  list1.reverse();
  int counter{10};
  for (auto v : list1) {
    EXPECT_EQ(v, --counter);
  }

  list1.reverse();
  counter = 0;
  for (auto v : list1) {
    EXPECT_EQ(v, ++counter);
  }

  lib::list<int> list2{1};
  list2.reverse();
  EXPECT_EQ(list2.size(), 1);
  EXPECT_EQ(list2.front(), 1);

  lib::list<int> list3;
  list3.reverse();
  EXPECT_TRUE(list3.empty());
}

TEST(Operations, unique) {
  sc::list<int> list1{1, 2, 3, 4, 5, 6}, list_r1{1, 2, 3, 4, 5, 6};
  auto it_begin = list1.begin();
  auto it_end = list1.end();
  EXPECT_EQ(list1.unique(), 0);
  EXPECT_TRUE(list1 == list_r1);
  *it_begin = 40;
  *(--it_end) = 90;
  EXPECT_EQ(list1.front(), 40);
  EXPECT_EQ(list1.back(), 90);

  sc::list<int> list2{1, 1, 1, 2, 2, 3, 7, 7, 9}, list_r2{1, 2, 3, 7, 9};
  EXPECT_EQ(list2.unique(), 4);
  EXPECT_TRUE(list2 == list_r2);

  sc::list<int> list3{1, 1, 7, 7, 3, 2, 2}, list_r3{1, 7, 3, 2};
  EXPECT_EQ(list3.unique(), 3);
  EXPECT_TRUE(list3 == list_r3);
}

TEST(Operations, sort) {
  auto check = [&](lib::list<int> input,
                   std::function<bool(int, int)> comp) -> bool {
    for (auto fast = ++input.begin(), slow = input.begin(); fast != input.end();
         ++fast) {
      if (comp(*fast, *slow)) {
        return false;
      }
      slow = fast;
    }
    return true;
  };

  lib::list<int> list1{1, 2, 3, 4, 5, 6};
  auto it_begin = list1.begin();
  auto it_end = list1.end();
  list1.sort();
  EXPECT_TRUE(check(list1, std::less()));
  *it_begin = 90;
  *(--it_end) = 30;
  EXPECT_EQ(list1.front(), 90);
  EXPECT_EQ(list1.back(), 30);

  lib::list<int> list2{9, 8, 7, 6, 5, 4, 3, 2, 1};
  list2.sort();
  EXPECT_TRUE(check(list2, std::less()));
  list2.sort(std::greater());
  EXPECT_TRUE(check(list2, std::greater()));

  lib::list<int> list3{1, 2, 3, 9, 8, 4, 1, 3, 2, 3, 4};
  list3.sort(std::greater());
  EXPECT_TRUE(check(list3, std::greater()));
  list3.sort(std::less());
  EXPECT_TRUE(check(list3, std::less()));
}

TEST(Operations, EquivalentOperator) {
  lib::list<int> list1{1, 2, 3}, list2{1, 2, 3}, list3{1, 4, 5}, list4{1, 2};
  EXPECT_TRUE(list1 == list1);
  EXPECT_FALSE(list1 == list3);
  EXPECT_FALSE(list1 == list4);
}

int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
