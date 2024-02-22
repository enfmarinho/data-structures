#include "HashTable/HashTableSeparateChaining.h"
#include "gtest/gtest.h"
#include <initializer_list>
#include <set>
#include <string>
#include <unordered_set>
#include <vector>

/*
 * The "HashTable" macro can be used to select which class to test. To ensure
 * the integrity of the tests, the std::unordered_multiset must be used, to
 * effectively test the Hash Table developed the ac::HashTable must be used.
 */
// #define HashTable std::unordered_multiset
#define HashTable ac::HashTable

TEST(SpecialFunction, DefaultConstructor) {
  HashTable<int> hash1;
  EXPECT_TRUE(hash1.empty());
  EXPECT_EQ(hash1.size(), 0);

  HashTable<char> hash2;
  EXPECT_TRUE(hash2.empty());
  EXPECT_EQ(hash2.size(), 0);

  HashTable<std::string> hash3;
  EXPECT_TRUE(hash3.empty());
  EXPECT_EQ(hash3.size(), 0);

  HashTable<int> hash4(20);
  EXPECT_TRUE(hash3.empty());
  EXPECT_EQ(hash3.size(), 0);
}

TEST(SpecialFunction, RangeConstructor) {
  int array[] = {1, 2, 3, 4, 5, 6};
  HashTable<int> hash1(array, array + 6);
  EXPECT_FALSE(hash1.empty());
  EXPECT_EQ(hash1.size(), 6);
  std::vector<bool> appered(6, false);
  for (int value : hash1) {
    EXPECT_FALSE(appered[value]);
    appered[value] = true;
  }

  HashTable<int> hash2(array, array);
  EXPECT_TRUE(hash2.empty());
}

TEST(SpecialFunction, CopyConstructor) {
  HashTable<int> hash1({1, 2, 3, 4});
  EXPECT_FALSE(hash1.empty());
  EXPECT_EQ(hash1.size(), 4);

  HashTable<int> hash2(hash1);
  EXPECT_FALSE(hash2.empty());
  EXPECT_EQ(hash2.size(), hash1.size());

  std::set<int> appered;
  for (int value : hash1) {
    appered.insert(value);
  }
  for (int value : hash2) {
    EXPECT_TRUE(appered.find(value) != appered.end());
    appered.erase(value);
  }
  EXPECT_TRUE(appered.empty());
}

TEST(SpecialFunction, InitializerListConstructor) {
  std::initializer_list<int> ilist1{1, 2, 3, 4, 5, 6, 7}, ilist2,
      ilist3{1, 2, 3};

  HashTable<int> hash1(ilist1);
  EXPECT_FALSE(hash1.empty());
  EXPECT_EQ(hash1.size(), 7);
  std::vector<bool> appeared(7, false);
  for (int value : hash1) {
    EXPECT_FALSE(appeared[value - 1]);
    appeared[value - 1] = true;
  }
  for (bool value : appeared) {
    EXPECT_TRUE(value);
  }

  HashTable<int> hash2(ilist2);
  EXPECT_TRUE(hash2.empty());
  EXPECT_EQ(hash2.size(), 0);

  HashTable<int> hash3(ilist3);
  EXPECT_FALSE(hash3.empty());
  EXPECT_EQ(hash3.size(), 3);
  std::vector<bool> appeared2(3, false);
  for (int value : hash3) {
    EXPECT_FALSE(appeared2[value - 1]);
    appeared2[value - 1] = true;
  }
  for (bool value : appeared2) {
    EXPECT_TRUE(value);
  }
}

TEST(SpecialFunction, MoveConstructor) {}

TEST(Iterators, Copy) {}

TEST(Iterators, begin) {}

TEST(Iterators, end) {}

TEST(Iterators, cbegin) {}

TEST(Iterators, cend) {}

TEST(Iterators, Increment) {}

TEST(Iterators, Decrement) {}

TEST(Iterators, EqualityOperator) {}

TEST(Iterators, InequalityOperator) {}

TEST(Iterator, ReferenceOperator) {}

TEST(Iterator, DereferenceOperator) {}

TEST(Iterator, ArrowOperator) {}

TEST(Capacity, empty) {
  HashTable<int> hash1, hash2{1, 2};
  EXPECT_TRUE(hash1.empty());
  EXPECT_FALSE(hash2.empty());
  hash2.clear();
  EXPECT_TRUE(hash2.empty());
}

TEST(Capacity, size) {
  HashTable<int> hash1;
  EXPECT_EQ(hash1.size(), 0);
  hash1.insert(1);
  EXPECT_EQ(hash1.size(), 1);
  hash1.insert(44);
  EXPECT_EQ(hash1.size(), 2);
  hash1.clear();
  EXPECT_TRUE(hash1.empty());
}

TEST(Modifiers, clear) {
  HashTable<int> hash1{1, 2, 3, 4};
  EXPECT_FALSE(hash1.empty());
  hash1.clear();
  EXPECT_TRUE(hash1.empty());
  hash1.clear();
  EXPECT_TRUE(hash1.empty());
}

TEST(Modifiers, InsertValue) {}

TEST(Modifiers, InsertRange) {}

TEST(Modifiers, InsertInitializerList) {}

TEST(Modifiers, ErasePos) {}

TEST(Modifiers, EraseRange) {}

TEST(Modifiers, EraseValue) {}

TEST(Modifiers, merge) {}

TEST(Lookup, count) {}

TEST(Lookup, find) {}

TEST(Lookup, contains) {}

TEST(BucketInterface, begin) {}

TEST(BucketInterface, end) {}

TEST(BucketInterface, cbegin) {}

TEST(BucketInterface, cend) {}

TEST(BucketInterface, bucket_count) {}

TEST(BucketInterface, bucket_size) {}

TEST(BucketInterface, bucket) {}

TEST(HashPolicy, load_factor) {}

TEST(HashPolicy, max_load_factor) {}

TEST(HashPolicy, rehash) {}

TEST(HashPolicy, reserve) {}

int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
