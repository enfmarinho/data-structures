#ifndef HASH_TABLE_SEPARATE_CHAINING_H
#define HASH_TABLE_SEPARATE_CHAINING_H

#include <cstddef> // size_t, ptrdiff_t
#include <forward_list>
#include <functional> //equal_to
#include <initializer_list>
#include <vector>

// Namespace for associative containers(ac).
namespace ac {
/*!
 * TODO document.
 */
template <class KeyType, class KeyHash = std::hash<KeyType>,
          class KeyEqual = std::equal_to<KeyType>>
class HashTable {
public:
  // Forward declaration.
  template <class ListIt, class BucketIt> class HashIterator;
  //=== Aliases.
  using value_type = KeyType;
  using list_type = std::forward_list<value_type>;
  using size_type = size_t;
  using difference_type = std::ptrdiff_t;
  using hasher = KeyHash;
  using key_equal = KeyEqual;
  using reference = KeyType &;
  using const_reference = const KeyType &;
  using iterator = HashIterator<typename std::vector<list_type>::iterator,
                                typename list_type::iterator>;
  using const_iterator =
      HashIterator<typename std::vector<list_type>::const_iterator,
                   typename list_type::const_iterator>;
  using local_iterator = typename list_type::iterator;
  using const_local_iterator = typename list_type::iterator;

  ///=== [I] Special Functions.
  HashTable(size_type bucket_count = DEFAULT_SIZE) {}
  template <typename InputIt>
  HashTable(InputIt first, InputIt last,
            size_type bucket_count = DEFAULT_SIZE) {}
  HashTable(const HashTable &other) {}
  HashTable(HashTable &&other) {}
  HashTable(std::initializer_list<value_type> ilist,
            size_type bucket_count = DEFAULT_SIZE) {}
  ~HashTable() = default;
  HashTable &operator=(const HashTable &other) {}
  HashTable &operator=(HashTable &&other) {}
  HashTable &operator=(std::initializer_list<value_type> ilist) {}

  ///=== [II] Iterators.
  iterator begin() {}
  iterator end() {}
  const_iterator cbegin() {}
  const_iterator cend() {}

  ///=== [III] Capacity.
  bool empty() const { return m_size == 0; }
  size_type size() const { return m_size; }

  ///=== [IV] Modifiers.
  void clear() {}
  iterator insert(const_reference value) {}
  template <typename InputIt> iterator insert(InputIt first, InputIt last) {}
  iterator insert(std::initializer_list<value_type> ilist) {}
  iterator erase(iterator pos) {}
  template <typename InputIt> iterator erase(InputIt first, InputIt last) {}
  size_type erase(const_reference key) {}
  void merge(HashTable &other) {}

  ///=== [V] Lookup.
  size_type count(const_reference key) {}
  iterator find(const_reference key) {}
  const_iterator find(const_reference key) const {}
  bool constains(const_reference key) const {}

  ///=== [VI] Bucket Interface.
  local_iterator begin(const size_type &index) {
    return m_table[index].begin();
  }
  const_local_iterator cbegin(const size_type &index) {
    return m_table[index].cbegin();
  }
  local_iterator end(const size_type &index) { return m_table[index].end(); }
  const_local_iterator cend(const size_type &index) {
    return m_table[index].cend();
  }
  size_type bucket_count() const { return m_table.size(); }
  size_type bucket_size(const size_type &index) const {
    return m_table[index].size();
  }
  size_type bucket(const_reference key) const { return hash(key); }

  ///=== [VII] Hash Policy.
  float load_factor() const {}
  float max_load_factor() const {}
  void max_load_factor(float load_factor) {}
  void rehash(size_type count) {}
  void reserve(size_type count) {}

  template <class ListIt, class BucketIt> class HashIterator {};

private:
  /// Returns the index at which the element should be stored.
  size_type hash(const_reference key) { return hasher{}(key) % m_table.size(); }
  /// Returns the first prime greater or equal to "number".
  size_type get_prime(size_type number) {}

  std::vector<list_type> m_table; //!< Scatter table.
  size_type m_size{0};            //!< Number of elements in the container.
  float m_max_load_factor{1.0};   //!< Max load factor.
  const static size_type DEFAULT_SIZE =
      11; //!< Minimum number of buckets to initialize the container.
};
} // namespace ac

#endif // HASH_TABLE_SEPARATE_CHAINING_H
