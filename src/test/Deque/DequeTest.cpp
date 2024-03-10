#include "Deque/Deque.h"
#include "gtest/gtest.h"
#include <deque>
#include <string>

/*
 * The "lib" macro can be used to select which lib to test. To ensure the
 * integrity of the tests, the "std" namespace must be used, to effectively test
 * the deque developed the "sc" namespace must be used. Note: when trying to
 * test changing the size of the deque's block, the "sc" namespace is used
 * directly since the "std" library is not compatible with that, also the tests
 * for the "InsertCopy" function uses the namespace "sc" directly, since the
 * functions signatures are different.
 */
#define lib sc
// #define lib std

TEST(SpecialFunctions, DefaultConstructor) {
  lib::deque<int> deque1;
  EXPECT_TRUE(deque1.empty());
  EXPECT_EQ(deque1.size(), 0);

  lib::deque<char> deque2;
  EXPECT_TRUE(deque2.empty());
  EXPECT_EQ(deque2.size(), 0);

  lib::deque<std::string> deque3;
  EXPECT_TRUE(deque3.empty());
  EXPECT_EQ(deque3.size(), 0);
}

TEST(SpecialFunctions, InitializerListConstructor) {
  std::initializer_list<int> ilist1{1, 2, 3, 4, 5, 6, 7}, ilist2{},
      ilist3{3, 4, 9, 1, 0, 3, 6, 2, 2};

  lib::deque<int> deque1(ilist1);
  EXPECT_FALSE(deque1.empty());
  EXPECT_EQ(deque1.size(), ilist1.size());
  int counter{0};
  for (int value : deque1) {
    EXPECT_EQ(value, ++counter);
  }

  lib::deque<int> deque2(ilist2);
  EXPECT_TRUE(deque2.empty());
  EXPECT_EQ(deque2.size(), 0);

  lib::deque<int> deque3(ilist3);
  EXPECT_EQ(deque3.size(), ilist3.size());
  auto list_it = ilist3.begin();
  for (int value : deque3) {
    EXPECT_EQ(value, *list_it++);
  }
}

TEST(SpecialFunctions, RangeConstructor) {
  int array[] = {9, 3, 5, 9, 1, 8, 3, 6};
  lib::deque<int> deque1(array, array + 4);
  EXPECT_EQ(deque1.size(), 4);
  for (int index{0}; index < deque1.size(); ++index) {
    EXPECT_EQ(deque1[index], array[index]);
  }

  lib::deque<int> deque2(array, array);
  EXPECT_TRUE(deque2.empty());

  lib::deque<int> deque3(array + 1, array + 2);
  EXPECT_EQ(deque3.size(), 1);
  EXPECT_EQ(*deque3.begin(), 3);

  lib::deque<int> deque4(array, array + 8);
  EXPECT_EQ(deque4.size(), 8);
  for (int index{0}; index < deque4.size(); ++index) {
    EXPECT_EQ(deque4[index], array[index]);
  }

  lib::deque<int> deque5(array, array + 5);
  EXPECT_EQ(deque5.size(), 5);
  for (int index = 0; index < deque5.size(); ++index) {
    EXPECT_EQ(deque5[index], array[index]);
  }
}

TEST(SpecialFunctions, CopyConstructor) {
  std::initializer_list<int> ilist1{1, 2, 3, 4, 5, 6, 7}, ilist2{},
      ilist3{3, 4, 9, 1, 0, 3, 6, 2, 2};

  lib::deque<int> deque1(ilist1);
  lib::deque<int> deque1_copy(deque1);
  EXPECT_FALSE(deque1.empty());
  EXPECT_FALSE(deque1_copy.empty());
  EXPECT_EQ(deque1.size(), ilist1.size());
  EXPECT_EQ(deque1_copy.size(), ilist1.size());
  int counter{0};
  for (int index{0}; index < deque1_copy.size(); ++index) {
    EXPECT_EQ(deque1_copy[index], ++counter);
    EXPECT_EQ(deque1_copy[index], deque1[index]);
  }

  lib::deque<int> deque2(ilist2);
  lib::deque<int> deque2_copy(deque2);
  EXPECT_TRUE(deque2.empty());
  EXPECT_TRUE(deque2_copy.empty());
  EXPECT_EQ(deque2.size(), 0);
  EXPECT_EQ(deque2_copy.size(), 0);

  lib::deque<int> deque3(ilist3);
  lib::deque<int> deque3_copy(deque3);
  EXPECT_EQ(deque3.size(), ilist3.size());
  EXPECT_EQ(deque3_copy.size(), ilist3.size());
  auto list_it = ilist3.begin();
  for (int index{0}; index < deque3_copy.size(); ++index) {
    EXPECT_EQ(deque3_copy[index], *list_it++);
    EXPECT_EQ(deque3_copy[index], deque3[index]);
  }
}

TEST(SpecialFunctions, MoveConstructor) {
  std::initializer_list<int> ilist1{1, 2, 3, 4, 5, 6, 7}, ilist2{},
      ilist3{3, 4, 9, 1, 0, 3, 6, 2, 2};
  lib::deque<int> deque1(ilist1);
  lib::deque<int> deque2(ilist2);
  lib::deque<int> deque3(ilist3);

  lib::deque<int> deque1_copy(std::move(deque1));
  EXPECT_TRUE(deque1.empty());
  EXPECT_FALSE(deque1_copy.empty());
  EXPECT_EQ(deque1_copy.size(), ilist1.size());
  int counter{0};
  for (int index{0}; index < deque1_copy.size(); ++index) {
    EXPECT_EQ(deque1_copy[index], ++counter);
  }

  lib::deque<int> deque2_copy(std::move(deque2));
  EXPECT_TRUE(deque2.empty());
  EXPECT_TRUE(deque2_copy.empty());
  EXPECT_EQ(deque2.size(), 0);
  EXPECT_EQ(deque2_copy.size(), 0);

  lib::deque<int> deque3_copy(std::move(deque3));
  EXPECT_TRUE(deque3.empty());
  EXPECT_FALSE(deque3_copy.empty());
  EXPECT_EQ(deque3_copy.size(), ilist3.size());
  auto list_it = ilist3.begin();
  for (int index{0}; index < deque3_copy.size(); ++index) {
    EXPECT_EQ(deque3_copy[index], *list_it++);
  }
}

TEST(ElementAccess, at) {
  lib::deque<int> deque1{1, 2, 3, 4, 5, 6, 7, 8, 9};
  for (int idx{0}; idx < deque1.size(); ++idx) {
    EXPECT_EQ(deque1[idx], idx + 1);
  }
  deque1[8] = 3;
  EXPECT_EQ(deque1[8], 3);
  deque1.push_back(99);
  EXPECT_EQ(deque1[9], 99);

  lib::deque<int> deque2;
  for (int value{11}; value < 100; value += 11) {
    deque2.push_back(value);
  }
  int counter{0};
  for (int idx{0}; idx < deque2.size(); ++idx, ++counter) {
    EXPECT_EQ(deque2[idx], 11 * idx + 11);
  }
  EXPECT_EQ(counter, deque2.size());
  EXPECT_EQ(deque2[0], 11);
}

TEST(ElementAccess, front) {
  lib::deque<int> deque1{9, 3, 6, 1, 4};
  EXPECT_EQ(deque1.front(), 9);
  deque1.pop_front();
  EXPECT_EQ(deque1.front(), 3);
  deque1.pop_front();
  EXPECT_EQ(deque1.front(), 6);
  deque1.pop_front();
  EXPECT_EQ(deque1.front(), 1);
  deque1.push_front(99);
  EXPECT_EQ(deque1.front(), 99);
  deque1.pop_front();
  deque1.pop_front();
  EXPECT_EQ(deque1.front(), 4);

  lib::deque<int> deque2;
  for (int value = 0; value < 10; ++value) {
    deque2.push_front(value);
  }
  for (int value = 9; value >= 0; --value) {
    EXPECT_EQ(deque2.front(), value);
    deque2.pop_front();
  }
}

TEST(ElementAccess, back) {
  lib::deque<int> deque1{9, 3, 6, 1, 4};
  EXPECT_EQ(deque1.back(), 4);
  deque1.pop_back();
  EXPECT_EQ(deque1.back(), 1);
  deque1.pop_back();
  EXPECT_EQ(deque1.back(), 6);
  deque1.pop_back();
  EXPECT_EQ(deque1.back(), 3);
  deque1.push_back(99);
  EXPECT_EQ(deque1.back(), 99);
  deque1.pop_back();
  deque1.pop_back();
  EXPECT_EQ(deque1.back(), 9);

  lib::deque<int> deque2;
  for (int value = 0; value < 10; ++value) {
    deque2.push_back(value);
  }
  for (int value = 9; value >= 0; --value) {
    EXPECT_EQ(deque2.back(), value);
    deque2.pop_back();
  }
}

TEST(Iterator, IteratorOperations) {
  lib::deque<int> deque1{1, 2, 3, 4, 5};
  EXPECT_FALSE(deque1.begin() == deque1.end());
  EXPECT_TRUE(deque1.begin() != deque1.end());
  EXPECT_TRUE((deque1.begin() + deque1.size()) == deque1.end());
  EXPECT_TRUE(deque1.begin() == (deque1.end() - deque1.size()));
  EXPECT_EQ(*deque1.begin(), 1);
  EXPECT_EQ(*(--deque1.end()), 5);
  lib::deque<int>::iterator it = deque1.begin();
  EXPECT_FALSE(it == deque1.end());
  it = deque1.end();
  EXPECT_TRUE(it == deque1.end());
  lib::deque<int>::iterator it_cp = lib::deque<int>::iterator(it);
  EXPECT_TRUE(it_cp == it);

  sc::deque<int, 2> deque2{1, 2, 3, 4, 5};
  EXPECT_FALSE(deque2.begin() == deque2.end());
  EXPECT_TRUE(deque2.begin() != deque2.end());
  EXPECT_TRUE((deque2.begin() + deque2.size()) == deque2.end());
  EXPECT_TRUE(deque2.begin() == (deque2.end() - deque2.size()));
  EXPECT_EQ(*deque2.begin(), 1);
  EXPECT_EQ(*(--deque2.end()), 5);
  sc::deque<int, 2>::iterator it2 = deque2.begin();
  EXPECT_FALSE(it2 == deque2.end());
  it2 = deque2.end();
  EXPECT_TRUE(it2 == deque2.end());
  sc::deque<int, 2>::iterator it2_cp = sc::deque<int, 2>::iterator(it2);
  EXPECT_TRUE(it2_cp == it2);

  lib::deque<int> deque3;
  EXPECT_TRUE(deque3.begin() == deque3.end());
  deque3.push_front(34);
  EXPECT_TRUE(deque3.begin() != deque3.end());
  deque3.pop_front();
  EXPECT_TRUE(deque3.begin() == deque3.end());
}

TEST(ConstIterator, ConstIteratorOperations) {
  lib::deque<int> deque1{1, 2, 3, 4, 5};
  EXPECT_FALSE(deque1.cbegin() == deque1.cend());
  EXPECT_TRUE(deque1.cbegin() != deque1.cend());
  EXPECT_TRUE((deque1.cbegin() + deque1.size()) == deque1.cend());
  EXPECT_TRUE(deque1.cbegin() == (deque1.cend() - deque1.size()));
  EXPECT_EQ(*deque1.cbegin(), 1);
  EXPECT_EQ(*(--deque1.cend()), 5);
  lib::deque<int>::const_iterator it = deque1.cbegin();
  EXPECT_FALSE(it == deque1.cend());
  it = deque1.cend();
  EXPECT_TRUE(it == deque1.cend());
  lib::deque<int>::const_iterator it_cp = lib::deque<int>::const_iterator(it);
  EXPECT_TRUE(it_cp == it);

  sc::deque<int, 2> deque2{1, 2, 3, 4, 5};
  EXPECT_FALSE(deque2.cbegin() == deque2.cend());
  EXPECT_TRUE(deque2.cbegin() != deque2.cend());
  EXPECT_TRUE((deque2.cbegin() + deque2.size()) == deque2.cend());
  EXPECT_TRUE(deque2.cbegin() == (deque2.cend() - deque2.size()));
  EXPECT_EQ(*deque2.cbegin(), 1);
  EXPECT_EQ(*(--deque2.cend()), 5);
  sc::deque<int, 2>::const_iterator it2 = deque2.cbegin();
  EXPECT_FALSE(it2 == deque2.cend());
  it2 = deque2.cend();
  EXPECT_TRUE(it2 == deque2.cend());
  sc::deque<int, 2>::const_iterator it2_cp =
      sc::deque<int, 2>::const_iterator(it2);
  EXPECT_TRUE(it2_cp == it2);

  lib::deque<int> deque3;
  EXPECT_TRUE(deque3.cbegin() == deque3.cend());
  deque3.push_front(34);
  EXPECT_TRUE(deque3.cbegin() != deque3.cend());
  deque3.pop_front();
  EXPECT_TRUE(deque3.cbegin() == deque3.cend());
}

TEST(Capacity, empty) {
  lib::deque<int> deque1;
  EXPECT_TRUE(deque1.empty());

  lib::deque<char> deque2;
  EXPECT_TRUE(deque2.empty());

  lib::deque<char> deque3{'a', 'i'};
  EXPECT_FALSE(deque3.empty());

  lib::deque<int> deque4{3, 1, 9, 16};
  EXPECT_FALSE(deque4.empty());
}

TEST(Capacity, size) {
  lib::deque<int> deque1;
  EXPECT_EQ(deque1.size(), 0);

  lib::deque<char> deque2;
  EXPECT_EQ(deque2.size(), 0);

  lib::deque<char> deque3{'a', 'i'};
  EXPECT_EQ(deque3.size(), 2);

  lib::deque<int> deque4{3, 1, 9, 16};
  EXPECT_EQ(deque4.size(), 4);
}

TEST(Capacity, shrink_to_fit) {
  lib::deque<int> deque1{1, 2, 3};
  deque1.shrink_to_fit();
  for (int value{0}; value < 1000; ++value) {
    deque1.push_back(value);
  }
  deque1.clear();
  deque1.shrink_to_fit();
  deque1.shrink_to_fit();
  for (int value{0}; value < 1000; ++value) {
    deque1.push_back(value);
  }
  EXPECT_EQ(deque1.size(), 1000);
  deque1.shrink_to_fit();
  for (int value{0}; value < 10; ++value) {
    deque1.push_back(value);
  }
  EXPECT_EQ(deque1.size(), 1010);
  deque1.clear();
  deque1.shrink_to_fit();
  deque1.push_back(3);
}

TEST(Modifiers, clear) {
  lib::deque<int> deque1;
  EXPECT_TRUE(deque1.empty());
  deque1.clear();
  EXPECT_TRUE(deque1.empty());
  deque1.push_back(3);
  EXPECT_EQ(deque1.size(), 1);
  EXPECT_EQ(deque1.front(), 3);
  deque1.clear();
  EXPECT_TRUE(deque1.empty());

  lib::deque<int> deque2{9, 1, 3};
  EXPECT_FALSE(deque2.empty());
  deque2.clear();
  EXPECT_TRUE(deque2.empty());
}

TEST(Modifiers, InsertValue) {
  lib::deque<int> deque1{3};
  EXPECT_EQ(deque1.insert(deque1.begin(), 1), deque1.begin());
  EXPECT_EQ(deque1.insert(++deque1.begin(), 2), ++deque1.begin());
  EXPECT_EQ(deque1.back(), 3);
  EXPECT_EQ(deque1.size(), 3);
  int expected = 0;
  for (int value = 4; value < 30; ++value) {
    EXPECT_EQ(deque1.insert(deque1.end(), value), --deque1.end());
  }
  EXPECT_EQ(deque1.insert(deque1.end(), 32), --deque1.end());
  EXPECT_EQ(deque1.insert(--deque1.end(), 31), deque1.end() - 2);
  EXPECT_EQ(deque1.insert(deque1.end() - 2, 30), deque1.end() - 3);
  EXPECT_EQ(deque1.size(), 32);
  expected = 0;
  for (int value : deque1) {
    EXPECT_EQ(value, ++expected);
  }

  lib::deque<int> deque2;
  EXPECT_EQ(deque2.insert(deque2.begin(), -22), deque2.begin());
  EXPECT_EQ(deque2.front(), -22);
  EXPECT_EQ(deque2.front(), deque2.back());
  EXPECT_EQ(deque2.insert(deque2.end(), 20), deque2.end() - 1);
  EXPECT_EQ(deque2.front(), -22);
  EXPECT_EQ(deque2.back(), 20);
  EXPECT_EQ(deque2.insert(--deque2.end(), 10), deque2.end() - 2);
  EXPECT_EQ(deque2.front(), -22);
  EXPECT_EQ(deque2.back(), 20);
  EXPECT_EQ(*(deque2.end() - 2), 10);
  EXPECT_EQ(deque2.size(), 3);

  sc::deque<int, 2> deque3;
  EXPECT_EQ(deque3.insert(deque3.begin(), -22), deque3.begin());
  EXPECT_EQ(deque3.front(), -22);
  EXPECT_EQ(deque3.front(), deque3.back());
  EXPECT_EQ(deque3.insert(deque3.end(), 20), deque3.end() - 1);
  EXPECT_EQ(deque3.front(), -22);
  EXPECT_EQ(deque3.back(), 20);
  EXPECT_EQ(deque3.insert(--deque3.end(), 10), deque3.end() - 2);
  EXPECT_EQ(deque3.front(), -22);
  EXPECT_EQ(deque3.back(), 20);
  EXPECT_EQ(*(deque3.end() - 2), 10);
  EXPECT_EQ(deque3.size(), 3);

  lib::deque<int> deque4;
  for (int value = 0; value < 1000; ++value) {
    deque4.push_back(value);
  }
  for (int expected = 0; expected < 1000; ++expected) {
    EXPECT_EQ(deque4.front(), expected);
    deque4.pop_front();
  }
}

TEST(Modifiers, InsertCopies) {
  sc::deque<int> deque1;
  EXPECT_EQ(deque1.insert(5, 99, deque1.begin()), deque1.begin());
  EXPECT_EQ(deque1.size(), 5);
  int counter{0};
  for (int value : deque1) {
    EXPECT_EQ(value, 99);
    ++counter;
  }
  EXPECT_EQ(counter, 5);
  EXPECT_EQ(deque1.insert(9, 1, deque1.begin()), deque1.begin());
  EXPECT_EQ(deque1.size(), 14);
  for (int index{0}; index < deque1.size(); ++index) {
    if (index < 9) {
      EXPECT_EQ(deque1[index], 1);
    } else {
      EXPECT_EQ(deque1[index], 99);
    }
  }
  EXPECT_EQ(deque1.insert(0, 0, deque1.begin()), deque1.begin());
  EXPECT_EQ(deque1.insert(0, 0, deque1.end()), deque1.end());

  sc::deque<int, 7> deque2;
  EXPECT_EQ(deque2.insert(0, 2, deque2.end()), deque2.end());
  EXPECT_EQ(deque2.insert(0, 2, deque2.begin()), deque2.end());
  EXPECT_EQ(deque2.size(), 0);
  EXPECT_EQ(deque2.insert(1, 2, deque2.begin()), deque2.begin());
  EXPECT_EQ(deque2.size(), 1);
  EXPECT_EQ(deque2.insert(2, 9, deque2.end()), ++deque2.begin());
  EXPECT_EQ(deque2.size(), 3);
  for (int index{0}; index < deque2.size(); ++index) {
    if (index < 1) {
      EXPECT_EQ(deque2[index], 2);
    } else {
      EXPECT_EQ(deque2[index], 9);
    }
  }
  EXPECT_EQ(deque2.insert(0, 0, deque2.begin()), deque2.begin());
  EXPECT_EQ(deque2.insert(0, 0, deque2.end()), deque2.end());

  sc::deque<char, 4> deque3;
  EXPECT_EQ(deque3.insert(100, 'c', deque3.end()), deque3.begin());
  EXPECT_EQ(deque3.size(), 100);
  for (char element : deque3) {
    EXPECT_EQ(element, 'c');
  }
}

TEST(Modifiers, InsertRange) {
  int array[] = {5, 4, 3, 2, 1, 6, 7, 8, 9, 10};
  int array2[] = {1, 2, 3, 4, 5, 10, 9, 8, 7, 6};

  lib::deque<int> deque1;
  EXPECT_EQ(deque1.insert(deque1.begin(), array, array + 5), deque1.begin());
  EXPECT_EQ(deque1.size(), 5);
  int expected{6};
  for (int value : deque1) {
    EXPECT_EQ(value, --expected);
  }
  EXPECT_EQ(deque1.insert(deque1.begin(), array2 + 5, array2 + 10),
            deque1.begin());
  EXPECT_EQ(deque1.size(), 10);
  expected = 11;
  for (int value : deque1) {
    EXPECT_EQ(value, --expected);
  }

  lib::deque<int> deque2;
  EXPECT_EQ(deque2.insert(deque2.end(), array2, array2 + 4), deque2.begin());
  EXPECT_EQ(deque2.size(), 4);
  EXPECT_EQ(deque2.insert(deque2.end(), array2 + 4, array2 + 5),
            --deque2.end());
  EXPECT_EQ(deque2.insert(deque2.end(), array + 5, array + 10),
            deque2.end() - 5);
  EXPECT_EQ(deque2.size(), 10);
  expected = 0;
  for (int value : deque2) {
    EXPECT_EQ(value, ++expected);
  }
}

TEST(Modifiers, InsertInitializerList) {
  lib::deque<int> deque1;
  EXPECT_EQ(deque1.insert(deque1.begin(), {}), deque1.end());
  EXPECT_EQ(deque1.insert(deque1.end(), {}), deque1.end());
  EXPECT_EQ(deque1.size(), 0);
  EXPECT_EQ(deque1.begin(), deque1.end());
  EXPECT_EQ(deque1.insert(deque1.begin(), {1, 2, 3}), deque1.begin());
  EXPECT_EQ(deque1.insert(deque1.begin() + 1, {}), deque1.begin() + 1);
  EXPECT_EQ(deque1.insert(deque1.end(), {4, 5, 6}), deque1.begin() + 3);
  EXPECT_EQ(deque1.size(), 6);
  int expected = 0;
  for (int value : deque1) {
    EXPECT_EQ(value, ++expected);
  }
  EXPECT_EQ(expected, 6);

  sc::deque<int, 2> deque2;
  EXPECT_EQ(deque2.insert(deque2.end(), {7, 8, 9}), deque2.begin());
  EXPECT_EQ(deque2.insert(deque2.begin(), {1, 2, 3}), deque2.begin());
  EXPECT_EQ(deque2.insert(deque2.begin() + 3, {4, 5, 6}), deque2.begin() + 3);
  EXPECT_EQ(deque2.insert(deque2.end(), {10, 11, 12}), deque2.end() - 3);
  expected = 0;
  for (int value : deque2) {
    EXPECT_EQ(value, ++expected);
  }
  EXPECT_EQ(expected, 12);
}

TEST(Modifiers, Erase) {
  lib::deque<int> deque1{-22, 1, 2, 9, 3, 4, 5, 10};
  EXPECT_EQ(deque1.erase(deque1.begin()), deque1.begin());
  EXPECT_EQ(*deque1.begin(), 1);
  EXPECT_EQ(deque1.erase(deque1.end() - 1), deque1.end());
  auto it = deque1.erase(deque1.begin() + 2);
  EXPECT_EQ(it, deque1.begin() + 2);
  EXPECT_EQ(*it, 3);
  int expected = 0;
  for (int value : deque1) {
    EXPECT_EQ(value, ++expected);
  }
  EXPECT_EQ(deque1.size(), 5);
  EXPECT_EQ(expected, 5);

  sc::deque<int, 7> deque2{-22, 1, 2, 9, 3, 4, 5, 10};
  EXPECT_EQ(deque2.erase(deque2.begin()), deque2.begin());
  EXPECT_EQ(*deque2.begin(), 1);
  EXPECT_EQ(deque2.erase(deque2.end() - 1), deque2.end());
  auto it2 = deque2.erase(deque2.begin() + 2);
  EXPECT_EQ(it2, deque2.begin() + 2);
  EXPECT_EQ(*it2, 3);
  expected = 0;
  for (int value : deque2) {
    EXPECT_EQ(value, ++expected);
  }
  EXPECT_EQ(deque2.size(), 5);
  EXPECT_EQ(expected, 5);
}

TEST(Modifiers, EraseRange) {
  lib::deque<int> deque1{100, 9, 77, 1, 2, 3, 4, 5, 10, 1, 12};
  EXPECT_EQ(deque1.erase(deque1.begin() + 3, deque1.begin() + 3),
            deque1.begin() + 3);
  auto it = deque1.erase(deque1.begin(), deque1.begin() + 3);
  EXPECT_EQ(it, deque1.begin());
  EXPECT_EQ(*it, 1);
  it = deque1.erase(deque1.end() - 3, deque1.end());
  EXPECT_EQ(it, deque1.end());
  EXPECT_EQ(deque1.size(), 5);
  int expected = 0;
  for (int value : deque1) {
    EXPECT_EQ(value, ++expected);
  }
  EXPECT_EQ(expected, 5);

  sc::deque<int, 5> deque2{100, 9, 77, 1, 2, 3, 4, 5, 10, 1, 12};
  EXPECT_EQ(deque2.erase(deque2.begin() + 3, deque2.begin() + 3),
            deque2.begin() + 3);
  auto it2 = deque2.erase(deque2.begin(), deque2.begin() + 3);
  EXPECT_EQ(it2, deque2.begin());
  EXPECT_EQ(*it2, 1);
  it2 = deque2.erase(deque2.end() - 3, deque2.end());
  EXPECT_EQ(it2, deque2.end());
  EXPECT_EQ(deque2.size(), 5);
  expected = 0;
  for (int value : deque2) {
    EXPECT_EQ(value, ++expected);
  }
  EXPECT_EQ(expected, 5);
}

TEST(Modifiers, push_back) {
  lib::deque<int> deque1{8, 1, 2};
  deque1.push_back(3);
  EXPECT_EQ(deque1.back(), 3);
  deque1.push_back(7);
  EXPECT_EQ(deque1.back(), 7);
  deque1.push_back(9);
  EXPECT_EQ(deque1.back(), 9);
  EXPECT_EQ(deque1.size(), 6);

  sc::deque<int, 2> deque2;
  EXPECT_TRUE(deque2.empty());
  deque2.push_back(2);
  EXPECT_FALSE(deque2.empty());
  EXPECT_EQ(deque2.back(), 2);
  deque2.push_back(9);
  EXPECT_EQ(deque2.back(), 9);
  EXPECT_EQ(deque2.size(), 2);
}

TEST(Modifiers, push_front) {
  lib::deque<int> deque1{8, 1, 2};
  deque1.push_front(3);
  EXPECT_EQ(deque1.front(), 3);
  deque1.push_front(7);
  EXPECT_EQ(deque1.front(), 7);
  deque1.push_front(9);
  EXPECT_EQ(deque1.front(), 9);
  EXPECT_EQ(deque1.size(), 6);

  sc::deque<int, 4> deque2;
  EXPECT_TRUE(deque2.empty());
  deque2.push_front(2);
  EXPECT_FALSE(deque2.empty());
  EXPECT_EQ(deque2.front(), 2);
  deque2.push_front(9);
  EXPECT_EQ(deque2.front(), 9);
  EXPECT_EQ(deque2.size(), 2);
}

TEST(Modifiers, pop_back) {
  lib::deque<int> deque{2, 4, 1, 8};
  deque.pop_back();
  EXPECT_EQ(deque.back(), 1);
  deque.pop_back();
  EXPECT_EQ(deque.back(), 4);
  deque.pop_back();
  EXPECT_EQ(deque.back(), 2);
  deque.pop_back();
  EXPECT_TRUE(deque.empty());
}

TEST(Modifiers, pop_front) {
  lib::deque<int> deque{1, 2, 3};
  deque.pop_front();
  EXPECT_EQ(deque.front(), 2);
  deque.pop_front();
  EXPECT_EQ(deque.front(), 3);
  deque.pop_front();
  EXPECT_TRUE(deque.empty());
}

TEST(Modifiers, resize) {
  lib::deque<int> deque1{1, 2, 3, 4, 5};
  deque1.resize(3);
  EXPECT_EQ(deque1.size(), 3);
  int expected{0};
  for (int value : deque1) {
    EXPECT_EQ(value, ++expected);
  }
  EXPECT_EQ(expected, 3);
  deque1.resize(9, 2);
  EXPECT_EQ(deque1.size(), 9);
  for (int index{0}; index < deque1.size(); ++index) {
    if (index < 3) {
      EXPECT_EQ(deque1[index], index + 1);
    } else {
      EXPECT_EQ(deque1[index], 2);
    }
  }

  lib::deque<int> deque2;
  deque2.resize(18);
  EXPECT_EQ(deque2.size(), 18);
  for (int value : deque2) {
    EXPECT_EQ(value, int());
  }
  deque2.resize(16, 5);
  EXPECT_EQ(deque2.size(), 16);
  for (int value : deque2) {
    EXPECT_EQ(value, int());
  }
  deque2.resize(18, 5);
  for (int index{0}; index < deque2.size(); ++index) {
    if (index < 16) {
      EXPECT_EQ(deque2[index], int());
    } else {
      EXPECT_EQ(deque2[index], 5);
    }
  }
}

int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
