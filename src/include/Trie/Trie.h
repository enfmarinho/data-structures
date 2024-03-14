#ifndef SRC_INCLUDE_TRIE_TRIE_H_
#define SRC_INCLUDE_TRIE_TRIE_H_

#include <cstddef> // size_t, ptrdiff_t
#include <map>
#include <stack>
#include <string>
#include <utility> // swap
#include <vector>

// Namespace for tree data structures.
namespace tree {
/*!
 * Trie data structure. Allows efficient storage of strings with common
 * prefixes, making it ideal for a lot of string-searching algorithms, such as
 * predictive text, approximate string matching, and spell checking.
 *
 * \author Eduardo Marinho (eduardo.nestor.marinho228@gmail.com)
 */
class Trie {
public:
  //=== Aliases.
  using size_type = size_t;

  struct Node {
    std::map<char, Node *> table; //!< Nodes from this node.
    size_type count{0};           //!< Number of words that end in this node.
    Node() = default;
    ~Node() = default;
  };

  ///=== [I] Special Functions.
  /// Constructs an empty trie.
  Trie() = default;
  /*!
   * Constructs a trie containing the strings in the initializer_list "words".
   * \param words initializer_list with the strings to insert.
   */
  Trie(const std::initializer_list<std::string> &words) {
    for (const std::string &word : words) {
      insert(word);
    }
  }
  /*!
   * Constructs a trie with "count" instances of the string "word".
   * \param word string to be inserted.
   * \param count number of times to insert the string.
   */
  explicit Trie(const std::string &word, size_type count = 1) {
    insert(word, count);
  }
  /*!
   * Constructs a clone of the trie "other".
   * \param other trie to be cloned.
   */
  Trie(const Trie &other) {
    // TODO optimize this, it can be easily done be going node by node.
    std::vector<std::string> words = other.consult_words();
    for (const std::string &word : words) {
      insert(word);
    }
  }
  /*!
   * Constructs a trie that takes ownership of "other" memory.
   * \param other trie to take memory from.
   */
  Trie(Trie &&other) {
    std::swap(m_root, other.m_root);
    std::swap(m_size, other.m_size);
  }
  /// Destructs the trie, deallocating its memory.
  ~Trie() {
    clear();
    delete m_root;
  }
  ///=== [II] Element Access.
  /*!
   * Returns all the strings in the container in alphabetical order.
   * \return vector of strings containing the strings in the container in
   *        alphabetical order.
   */
  [[nodiscard]] std::vector<std::string> consult_words() const {
    std::vector<std::string> words;
    std::string word;
    traverse(words, word, m_root);
    return words;
  }
  /// Consults whether "word" is in the container.
  [[nodiscard]] bool is_there(const std::string &word) const {
    return count(word) > 0;
  }
  /// Consults the number of times "word" has been inserted into the trie.
  [[nodiscard]] size_type count(const std::string &word) const {
    Node *runner = m_root;
    for (char letter : word) {
      auto it = runner->table.find(letter);
      if (it != runner->table.end()) {
        runner = it->second;
      } else {
        return 0;
      }
    }
    return runner->count;
  }

  ///=== [III] Capacity.
  /// Consults whether the container is empty.
  [[nodiscard]] bool empty() const { return m_size == 0; }
  /// Consults the number of strings in the container.
  [[nodiscard]] size_type size() const { return m_size; }

  ///=== [IV] Modifiers.
  /// Clears the contents of the container, deallocating its memory.
  void clear() {
    for (std::pair<char, Node *> element : m_root->table) {
      clear(element.second);
    }
    m_root->table.clear();
    m_size = 0;
  }
  /*!
   * Inserts "count" times the string "word" in the container.
   * \param word string to insert in the container.
   * \param count number of times to insert the string "word" in the trie.
   */
  void insert(const std::string &word, size_type count = 1) {
    m_size += count;
    if (word.empty()) {
      m_root->count += count;
      return;
    }
    Node *runner = m_root;
    for (char letter : word) {
      auto it = runner->table.find(letter);
      if (it != runner->table.end()) {
        runner = it->second;
      } else {
        Node *pointer = new Node;
        runner->table.insert({letter, pointer});
        runner = pointer;
      }
    }
    runner->count += count;
  }
  /*!
   * Removes the string "word" "count" times from the container, if is in it,
   * otherwise does nothing, also, if there are less then "count" instances of
   * the string "word" in the container, remove all of them.
   * \param word string to be removed from the container.
   * \param count number of times to erase the string "word".
   * \return boolean representing whether or not the string was removed.
   */
  bool erase(const std::string &word, size_type count = 1) {
    Node *runner = m_root;
    for (char letter : word) {
      auto it = runner->table.find(letter);
      if (it != runner->table.end()) {
        runner = it->second;
      } else {
        return false;
      }
    }
    if (runner->count > count) {
      runner->count -= count;
      m_size -= count;
      return true;
    } else if (runner->count > 0) {
      m_size -= runner->count;
      runner->count = 0;
      return true;
    }
    return false;
  }
  /*!
   * Removes the string "word" "count" times from the container, if is in it,
   * otherwise does nothing, also, if there are less then "count" instances of
   * the string "word" in the container, remove all of them. After removing it,
   * deallocates nodes that are not required anymore.
   * \param word string to be removed from the container.
   * \param count number of times to erase the string "word".
   * \return boolean representing whether or not the string was removed.
   */
  bool erase_clearing(const std::string &word, size_type count = 1) {
    std::stack<std::pair<char, Node *>> pointers;
    Node *runner = m_root;
    for (char letter : word) {
      auto it = runner->table.find(letter);
      if (it == runner->table.end()) {
        return false;
      }
      runner = it->second;
      pointers.push({letter, runner});
    }
    if (runner->count == 0) {
      return false;
    }
    if (runner->count > count) {
      runner->count -= count;
      m_size -= count;
      return true;
    }
    m_size -= runner->count;
    runner->count = 0;
    if (!runner->table.empty()) {
      return true;
    }
    char last_letter;
    while (!pointers.empty() && pointers.top().second->count <= 1 &&
           !pointers.top().second->table.empty()) {
      delete pointers.top().second;
      last_letter = pointers.top().first;
      pointers.pop();
    }
    if (pointers.empty()) {
      m_root->table.erase(last_letter);
    } else {
      pointers.top().second->table.erase(last_letter);
    }
    return true;
  }

private:
  /// Helper function to deallocate trie memory.
  void clear(Node *pointer) {
    for (std::pair<char, Node *> letter : pointer->table) {
      clear(letter.second);
    }
    delete pointer;
  }
  /// Helper function to traverse the trie saving its string in the vector.
  void traverse(std::vector<std::string> &words, std::string &word,
                Node *current) const {
    for (unsigned int counter{0}; counter < current->count; ++counter) {
      words.push_back(word);
    }
    for (std::pair<char, Node *> pair : current->table) {
      word.push_back(pair.first);
      traverse(words, word, pair.second);
      word.pop_back();
    }
  }

  Node *m_root{new Node}; // Pointer to the root node.
  size_type m_size{0};    // Number of words stored.
};
} // namespace tree

#endif // SRC_INCLUDE_TRIE_TRIE_H_
