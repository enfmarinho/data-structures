#include "Trie/Trie.h"
#include "gtest/gtest.h"
#include <algorithm> // is_sorted, sort
#include <string>
#include <utility> // move
#include <vector>

class TrieTest : public testing::Test {
protected:
  void SetUp() override {
    for (std::string word : words_list1) {
      trie1.insert(word);
    }
    for (std::string word : words_list2) {
      trie2.insert(word);
    }
    for (std::string word : words_list3) {
      trie3.insert(word);
    }
  }

  std::vector<std::string> words_list1{
      "apple", "apple", "banana", "grape", "grape", "pineapple", "watermelon"};
  std::vector<std::string> words_list2{
      "car", "car", "car", "truck", "motorcycle", "van", "van", "van", "", ""};
  std::vector<std::string> words_list3{
      "",      "",        "",   "Hello, World!", "Hello, World!",
      "Hello", "Hello, ", "Hi", "Hi again"};
  tree::Trie trie1;
  tree::Trie trie2;
  tree::Trie trie3;
  tree::Trie trie_empty;
};

TEST(SpecialFunctions, WordsListConstructor) {
  std::initializer_list<std::string> words_list1{
      "apple", "banana", "grape", "pineapple", "watermelon", "apple", "grape"};
  tree::Trie trie1(words_list1);
  EXPECT_EQ(trie1.size(), words_list1.size());
  for (std::string word : words_list1) {
    EXPECT_TRUE(trie1.is_there(word));
  }
  EXPECT_EQ(trie1.count("apple"), 2);
  EXPECT_EQ(trie1.count("grape"), 2);
  EXPECT_EQ(trie1.count("banana"), 1);
  EXPECT_FALSE(trie1.is_there("asdf"));
}

TEST(SpecialFunctions, WordConstructor) {
  tree::Trie trie1("try", 3);
  EXPECT_EQ(trie1.count("try"), 3);
  EXPECT_FALSE(trie1.is_there(""));
  EXPECT_FALSE(trie1.is_there("as"));
  EXPECT_FALSE(trie1.is_there("tr"));
  EXPECT_FALSE(trie1.is_there("try "));

  tree::Trie trie2("", 0);
  EXPECT_FALSE(trie2.is_there(""));
  EXPECT_FALSE(trie2.is_there("try"));
  EXPECT_FALSE(trie2.is_there("asdf"));

  tree::Trie trie3("Hello", 1);
  EXPECT_EQ(trie3.is_there("Hello"), 1);
  EXPECT_FALSE(trie3.is_there("hello"));
  EXPECT_FALSE(trie3.is_there("hel"));
  EXPECT_FALSE(trie3.is_there(""));
  EXPECT_FALSE(trie3.is_there("Hello "));
}

TEST_F(TrieTest, CopyConstructor) {
  tree::Trie trie1_copy(trie1);
  EXPECT_EQ(trie1_copy.size(), words_list1.size());
  EXPECT_EQ(trie1_copy.size(), trie1.size());
  for (std::string word : words_list1) {
    EXPECT_TRUE(trie1_copy.is_there(word));
    EXPECT_TRUE(trie1.is_there(word));
  }
  EXPECT_FALSE(trie1_copy.is_there("app"));
  EXPECT_FALSE(trie1_copy.is_there(""));
  EXPECT_FALSE(trie1.is_there("app"));
  EXPECT_FALSE(trie1.is_there(""));

  tree::Trie trie2_copy(trie2);
  EXPECT_EQ(trie2_copy.size(), words_list2.size());
  EXPECT_EQ(trie2_copy.size(), trie2.size());
  for (std::string word : words_list2) {
    EXPECT_TRUE(trie2_copy.is_there(word));
    EXPECT_TRUE(trie2.is_there(word));
  }
  EXPECT_FALSE(trie2_copy.is_there("ca"));
  EXPECT_FALSE(trie2_copy.is_there("tru"));
  EXPECT_FALSE(trie2.is_there("ca"));
  EXPECT_FALSE(trie2.is_there("tru"));

  tree::Trie trie3_copy(trie3);
  EXPECT_EQ(trie3_copy.size(), words_list3.size());
  EXPECT_EQ(trie3_copy.size(), trie3.size());
  for (std::string word : words_list3) {
    EXPECT_TRUE(trie3_copy.is_there(word));
    EXPECT_TRUE(trie3.is_there(word));
  }
  EXPECT_FALSE(trie3_copy.is_there("Hell"));
  EXPECT_FALSE(trie3_copy.is_there("H"));
  EXPECT_FALSE(trie3.is_there("Hell"));
  EXPECT_FALSE(trie3.is_there("H"));

  tree::Trie trie_empty_copy(trie_empty);
  EXPECT_TRUE(trie_empty_copy.empty());
  EXPECT_TRUE(trie_empty.empty());
}

TEST_F(TrieTest, MoveConstructor) {
  tree::Trie trie1_move(std::move(trie1));
  EXPECT_EQ(trie1_move.size(), words_list1.size());
  EXPECT_TRUE(trie1.empty());
  for (std::string word : words_list1) {
    EXPECT_TRUE(trie1_move.is_there(word));
    EXPECT_FALSE(trie1.is_there(word));
  }
  EXPECT_FALSE(trie1_move.is_there("app"));
  EXPECT_FALSE(trie1_move.is_there(""));
  EXPECT_FALSE(trie1.is_there("app"));
  EXPECT_FALSE(trie1.is_there(""));

  tree::Trie trie2_move(std::move(trie2));
  EXPECT_EQ(trie2_move.size(), words_list2.size());
  EXPECT_TRUE(trie2.empty());
  for (std::string word : words_list2) {
    EXPECT_TRUE(trie2_move.is_there(word));
    EXPECT_FALSE(trie2.is_there(word));
  }
  EXPECT_FALSE(trie2_move.is_there("ca"));
  EXPECT_FALSE(trie2_move.is_there("tru"));
  EXPECT_FALSE(trie2.is_there("ca"));
  EXPECT_FALSE(trie2.is_there("tru"));

  tree::Trie trie3_move(std::move(trie3));
  EXPECT_EQ(trie3_move.size(), words_list3.size());
  EXPECT_TRUE(trie3.empty());
  for (std::string word : words_list3) {
    EXPECT_TRUE(trie3_move.is_there(word));
    EXPECT_FALSE(trie3.is_there(word));
  }
  EXPECT_FALSE(trie3_move.is_there("Hell"));
  EXPECT_FALSE(trie3_move.is_there("H"));
  EXPECT_FALSE(trie3.is_there("Hell"));
  EXPECT_FALSE(trie3.is_there("H"));

  tree::Trie trie_empty_move(std::move(trie_empty));
  EXPECT_TRUE(trie_empty_move.empty());
  EXPECT_TRUE(trie_empty.empty());
}

TEST_F(TrieTest, consult_words) {
  std::vector<std::string> list = trie1.consult_words();
  EXPECT_TRUE(std::is_sorted(list.begin(), list.end()));
  sort(words_list1.begin(), words_list1.end());
  EXPECT_TRUE(list == words_list1);

  list = trie2.consult_words();
  EXPECT_TRUE(std::is_sorted(list.begin(), list.end()));
  sort(words_list2.begin(), words_list2.end());
  EXPECT_TRUE(list == words_list2);

  list = trie3.consult_words();
  EXPECT_TRUE(std::is_sorted(list.begin(), list.end()));
  sort(words_list3.begin(), words_list3.end());
  EXPECT_TRUE(list == words_list3);

  list = trie_empty.consult_words();
  EXPECT_TRUE(list.empty());
}

TEST_F(TrieTest, is_there) {
  for (std::string word : words_list1) {
    EXPECT_TRUE(trie1.is_there(word));
  }
  EXPECT_FALSE(trie1.is_there("asdf"));
  EXPECT_FALSE(trie1.is_there("app"));
  EXPECT_FALSE(trie1.is_there(""));

  for (std::string word : words_list2) {
    EXPECT_TRUE(trie2.is_there(word));
  }
  EXPECT_FALSE(trie1.is_there("asdf"));
  EXPECT_FALSE(trie1.is_there("tru"));
  EXPECT_FALSE(trie1.is_there(""));

  for (std::string word : words_list3) {
    EXPECT_TRUE(trie3.is_there(word));
  }
  EXPECT_FALSE(trie1.is_there("asdf"));
  EXPECT_FALSE(trie1.is_there("H"));
  EXPECT_FALSE(trie1.is_there(""));

  EXPECT_FALSE(trie1.is_there("asdf"));
  EXPECT_FALSE(trie1.is_there("app"));
  EXPECT_FALSE(trie1.is_there(""));
}

TEST_F(TrieTest, count) {
  EXPECT_EQ(trie1.count("apple"), 2);
  EXPECT_EQ(trie1.count("banana"), 1);
  EXPECT_EQ(trie1.count("grape"), 2);
  EXPECT_EQ(trie1.count("pineapple"), 1);
  EXPECT_EQ(trie1.count("watermelon"), 1);
  EXPECT_EQ(trie1.count("appl"), 0);
  EXPECT_EQ(trie1.count(""), 0);

  EXPECT_EQ(trie2.count("car"), 3);
  EXPECT_EQ(trie2.count("van"), 3);
  EXPECT_EQ(trie2.count("truck"), 1);
  EXPECT_EQ(trie2.count("motorcycle"), 1);
  EXPECT_EQ(trie2.count(""), 2);
  EXPECT_EQ(trie2.count("apple"), 0);
  EXPECT_EQ(trie2.count("ca"), 0);

  EXPECT_EQ(trie3.count("Hello, World!"), 2);
  EXPECT_EQ(trie3.count("Hello"), 1);
  EXPECT_EQ(trie3.count("Hello, "), 1);
  EXPECT_EQ(trie3.count("Hi"), 1);
  EXPECT_EQ(trie3.count(""), 3);
  EXPECT_EQ(trie3.count("apple"), 0);
  EXPECT_EQ(trie3.count("car"), 0);
  EXPECT_EQ(trie3.count("another"), 0);

  EXPECT_EQ(trie_empty.count("anohr"), 0);
  EXPECT_EQ(trie_empty.count("as"), 0);
  EXPECT_EQ(trie_empty.count("car"), 0);
  EXPECT_EQ(trie_empty.count(""), 0);
}

TEST_F(TrieTest, empty) {
  EXPECT_FALSE(trie1.empty());

  EXPECT_FALSE(trie2.empty());

  EXPECT_FALSE(trie3.empty());

  EXPECT_TRUE(trie_empty.empty());
}

TEST_F(TrieTest, size) {
  EXPECT_EQ(trie1.size(), words_list1.size());

  EXPECT_EQ(trie2.size(), words_list2.size());

  EXPECT_EQ(trie3.size(), words_list3.size());

  EXPECT_EQ(trie_empty.size(), 0);
}

TEST_F(TrieTest, clear) {
  trie1.clear();
  EXPECT_TRUE(trie1.empty());

  trie2.clear();
  EXPECT_TRUE(trie2.empty());

  trie3.clear();
  EXPECT_TRUE(trie3.empty());

  trie_empty.clear();
  EXPECT_TRUE(trie_empty.empty());
}

TEST_F(TrieTest, insert) {
  trie1.insert("appleapple");
  trie1.insert("another fruit!");
  trie1.insert("apple");
  EXPECT_EQ(trie1.count("apple"), 3);
  EXPECT_EQ(trie1.count("banana"), 1);
  EXPECT_EQ(trie1.count("grape"), 2);
  EXPECT_EQ(trie1.count("pineapple"), 1);
  EXPECT_EQ(trie1.count("watermelon"), 1);
  EXPECT_EQ(trie1.count("appl"), 0);
  EXPECT_EQ(trie1.count(""), 0);
  EXPECT_EQ(trie1.count("another fruit!"), 1);
  EXPECT_EQ(trie1.count("appleapple"), 1);
  EXPECT_EQ(trie1.size(), words_list1.size() + 3);

  trie2.insert("carcar");
  trie2.insert("truuck");
  trie2.insert("motorcycle!!");
  trie2.insert("truck");
  EXPECT_EQ(trie2.count("car"), 3);
  EXPECT_EQ(trie2.count("van"), 3);
  EXPECT_EQ(trie2.count("truck"), 2);
  EXPECT_EQ(trie2.count("motorcycle"), 1);
  EXPECT_EQ(trie2.count(""), 2);
  EXPECT_EQ(trie2.count("apple"), 0);
  EXPECT_EQ(trie2.count("ca"), 0);
  EXPECT_EQ(trie2.count("carcar"), 1);
  EXPECT_EQ(trie2.count("motorcycle!!"), 1);
  EXPECT_EQ(trie2.count("truuck"), 1);
  EXPECT_EQ(trie2.size(), words_list2.size() + 4);

  trie3.insert("Hello, World!", 3);
  trie3.insert("Hi!");
  trie3.insert("Goodbye, World!");
  EXPECT_EQ(trie3.count("Hello, World!"), 5);
  EXPECT_EQ(trie3.count("Hello"), 1);
  EXPECT_EQ(trie3.count("Hello, "), 1);
  EXPECT_EQ(trie3.count("Hi"), 1);
  EXPECT_EQ(trie3.count(""), 3);
  EXPECT_EQ(trie3.count("apple"), 0);
  EXPECT_EQ(trie3.count("car"), 0);
  EXPECT_EQ(trie3.count("Hi!"), 1);
  EXPECT_EQ(trie3.count("Goodbye, World!"), 1);
  EXPECT_EQ(trie3.count("another"), 0);
  EXPECT_EQ(trie3.size(), words_list3.size() + 5);

  trie_empty.insert("that is not empty anymore!", 3);
  trie_empty.insert("empty", 2);
  EXPECT_EQ(trie_empty.count("anohr"), 0);
  EXPECT_EQ(trie_empty.count("as"), 0);
  EXPECT_EQ(trie_empty.count("car"), 0);
  EXPECT_EQ(trie_empty.count(""), 0);
  EXPECT_EQ(trie_empty.count("that is not empty anymore!"), 3);
  EXPECT_EQ(trie_empty.count("empty"), 2);
  EXPECT_EQ(trie_empty.size(), 5);
}

TEST_F(TrieTest, erase) {
  EXPECT_TRUE(trie1.erase("apple"));
  EXPECT_EQ(trie1.count("apple"), 1);
  EXPECT_TRUE(trie1.erase("apple"));
  EXPECT_EQ(trie1.count("apple"), 0);
  EXPECT_TRUE(trie1.erase("banana"));
  EXPECT_TRUE(trie1.erase("grape", 2));
  EXPECT_FALSE(trie1.erase("grape"));
  EXPECT_TRUE(trie1.erase("pineapple"));
  EXPECT_TRUE(trie1.erase("watermelon"));
  EXPECT_FALSE(trie1.erase(""));
  EXPECT_FALSE(trie1.erase("app"));
  EXPECT_FALSE(trie1.erase("again"));
  EXPECT_TRUE(trie1.empty());
  for (std::string word : words_list1) {
    EXPECT_FALSE(trie1.is_there(word));
  }

  EXPECT_TRUE(trie2.erase("car", 3));
  EXPECT_EQ(trie2.count("car"), 0);
  EXPECT_FALSE(trie2.erase("again"));
  EXPECT_FALSE(trie2.erase("amazing"));
  EXPECT_TRUE(trie2.erase("truck"));
  EXPECT_TRUE(trie2.erase("motorcycle"));
  EXPECT_TRUE(trie2.erase("van"));
  EXPECT_TRUE(trie2.erase("van"));
  EXPECT_TRUE(trie2.erase("van"));
  EXPECT_TRUE(trie2.erase(""));
  EXPECT_EQ(trie2.count(""), 1);
  EXPECT_TRUE(trie2.erase(""));
  EXPECT_TRUE(trie2.empty());
  for (std::string word : words_list2) {
    EXPECT_FALSE(trie2.is_there(word));
  }

  EXPECT_TRUE(trie3.erase("", 3));
  EXPECT_FALSE(trie3.erase(""));
  EXPECT_EQ(trie3.count(""), 0);
  EXPECT_FALSE(trie3.erase("nothing"));
  EXPECT_TRUE(trie3.erase("Hi"));
  EXPECT_TRUE(trie3.erase("Hello"));
  EXPECT_EQ(trie3.size(), words_list3.size() - 5);
  EXPECT_EQ(trie3.count("Hello, World!"), 2);
  EXPECT_TRUE(trie3.is_there("Hello, "));
  EXPECT_TRUE(trie3.is_there("Hi again"));

  EXPECT_FALSE(trie_empty.erase("count"));
  EXPECT_FALSE(trie_empty.erase("another"));
  EXPECT_FALSE(trie_empty.erase("don't know"));
  EXPECT_FALSE(trie_empty.erase("other word"));
}

TEST_F(TrieTest, erase_clearing) {
  EXPECT_TRUE(trie1.erase_clearing("apple"));
  EXPECT_EQ(trie1.count("apple"), 1);
  EXPECT_TRUE(trie1.erase_clearing("apple"));
  EXPECT_TRUE(trie1.erase_clearing("banana"));
  EXPECT_EQ(trie1.count("apple"), 0);
  EXPECT_TRUE(trie1.erase_clearing("grape", 2));
  EXPECT_TRUE(trie1.erase_clearing("pineapple"));
  EXPECT_TRUE(trie1.erase_clearing("watermelon"));
  EXPECT_FALSE(trie1.erase_clearing(""));
  EXPECT_FALSE(trie1.erase_clearing("app"));
  EXPECT_FALSE(trie1.erase_clearing("again"));
  EXPECT_TRUE(trie1.empty());
  for (std::string word : words_list1) {
    EXPECT_FALSE(trie1.is_there(word));
  }

  EXPECT_TRUE(trie2.erase_clearing("car", 3));
  EXPECT_EQ(trie2.count("car"), 0);
  EXPECT_FALSE(trie2.erase_clearing("again"));
  EXPECT_FALSE(trie2.erase_clearing("amazing"));
  EXPECT_TRUE(trie2.erase_clearing("truck"));
  EXPECT_TRUE(trie2.erase_clearing("motorcycle"));
  EXPECT_TRUE(trie2.erase_clearing("van", 3));
  EXPECT_FALSE(trie2.erase_clearing("van"));
  EXPECT_TRUE(trie2.erase_clearing(""));
  EXPECT_EQ(trie2.count(""), 1);
  EXPECT_TRUE(trie2.erase_clearing(""));
  EXPECT_TRUE(trie2.empty());
  for (std::string word : words_list2) {
    EXPECT_FALSE(trie2.is_there(word));
  }

  EXPECT_TRUE(trie3.erase_clearing("", 5));
  EXPECT_FALSE(trie3.erase_clearing(""));
  EXPECT_FALSE(trie3.erase_clearing("nothing"));
  EXPECT_EQ(trie3.count(""), 0);
  EXPECT_TRUE(trie3.erase_clearing("Hi"));
  EXPECT_TRUE(trie3.erase_clearing("Hello"));
  EXPECT_EQ(trie3.size(), words_list3.size() - 5);
  EXPECT_EQ(trie3.count("Hello, World!"), 2);
  EXPECT_TRUE(trie3.is_there("Hello, "));
  EXPECT_TRUE(trie3.is_there("Hi again"));

  EXPECT_FALSE(trie_empty.erase_clearing("count"));
  EXPECT_FALSE(trie_empty.erase_clearing("another"));
  EXPECT_FALSE(trie_empty.erase_clearing("don't know"));
  EXPECT_FALSE(trie_empty.erase_clearing("other word"));
}

int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
