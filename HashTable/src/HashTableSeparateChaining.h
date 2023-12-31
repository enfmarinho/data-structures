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
  HashTable(size_type bucket_count = DEFAULT_SIZE) {
    m_table.resize(find_next_prime(bucket_count));
  }
  template <typename InputIt>
  HashTable(InputIt first, InputIt last,
            size_type bucket_count = DEFAULT_SIZE) {
    m_table.resize(find_next_prime(bucket_count));
    reserve(std::distance(first, last));
    while (first != last) {
      insert(*first);
      ++first;
    }
  }
  HashTable(const HashTable &other) { *this = other; }
  HashTable(HashTable &&other) { *this = other; }
  HashTable(std::initializer_list<value_type> ilist,
            size_type bucket_count = DEFAULT_SIZE) {
    m_table.resize(find_next_prime(bucket_count));
    *this = ilist;
  }
  ~HashTable() = default;
  HashTable &operator=(const HashTable &other) {
    m_max_load_factor = other.m_max_load_factor;
    clear();
    reserve(other.size());
    for (value_type element : other) {
      insert(element);
    }
    return *this;
  }
  HashTable &operator=(HashTable &&other) {
    m_table = std::move(other.m_table);
    m_max_load_factor = other.m_max_load_factor;
    m_size = other.m_size;
    other.m_size = 0;
    return *this;
  }
  HashTable &operator=(std::initializer_list<value_type> ilist) {
    clear();
    reserve(ilist.size());
    for (value_type element : ilist) {
      insert(element);
    }
    return *this;
  }

  ///=== [II] Iterators.
  iterator begin() { return iterator(m_table.begin(), m_table[0].begin()); }
  iterator end() {
    return iterator(--m_table.end(), m_table[m_table.size() - 1].end());
  }
  const_iterator cbegin() {
    return const_iterator(m_table.cbegin(), m_table[0].cbegin());
  }
  const_iterator cend() {
    return const_iterator(--m_table.cend(), m_table[m_table.size()].cend());
  }

  ///=== [III] Capacity.
  bool empty() const { return m_size == 0; }
  size_type size() const { return m_size; }

  ///=== [IV] Modifiers.
  /// Empties the content of the container, i.e. removes all its elements.
  void clear() {
    for (list_type list : m_table) {
      list.clear();
    }
    m_size = 0;
  }
  /*!
   * Inserts the element "value" in the container.
   * \param value element to insert.
   * \return iterator to the inserted element.
   */
  iterator insert(const_reference value) {
    ++m_size;
    if (load_factor() > m_max_load_factor) {
      rehash(find_next_prime(m_size));
    }
    size_type index = hash(value);
    m_table[index].push_front(value);
    return iterator(m_table.begin() + index, m_table[index].begin());
  }
  /*!
   * Inserts all elements in the range [first, last) in the container.
   * \param first beginning of the range.
   * \param last end of the range (not included).
   */
  template <typename InputIt> void insert(InputIt first, InputIt last) {
    reserve(m_size + std::distance(first, last));
    for (; first != last; ++first) {
      insert(*first);
    }
  }
  /*!
   * Inserts all elements in the initializer_list "ilist" in the container.
   * \param ilist initializer_list containing the elements to insert.
   */
  void insert(std::initializer_list<value_type> ilist) {
    reserve(m_size + ilist.size());
    for (value_type element : ilist) {
      insert(element);
    }
  }
  /*!
   * Removes the element being pointed by the iterator "pos".
   * \param pos iterator pointing to the element to remove.
   * \return iterator pointing to the element following the removed one.
   */
  iterator erase(iterator pos) {
    iterator following_removed = pos + 1;
    size_type index = hash(*pos);
    m_table[index].erase(pos.m_bucket);
    return following_removed;
  }
  // template must be iterator or constant iterator
  template <typename InputIt> iterator erase(InputIt first, InputIt last) {
    while (first != last) {
      InputIt copy = first + 1;
      size_type index = hash(*first);
      m_table[index].erase(first.m_bucket);
      first = copy;
    }
    return last;
  }
  size_type erase(const_reference key) {
    size_type index = hash(key);
    size_type counter{0};
    for (auto runner = m_table[index].begin(); runner != m_table[index].end();
         ++runner) {
      if (key_equal{}(*runner, key)) {
        m_table[index].erase(runner);
        ++counter;
      }
    }
    return counter;
  }
  void merge(HashTable &other) {
    for (value_type element : other) {
      insert(element);
    }
  }

  ///=== [V] Lookup.
  /*!
   * Counts the number of elements that compares equal to "key".
   * \param key element to look for.
   * \return number of elements equal to "key".
   */
  size_type count(const_reference key) {
    size_type index = hash(key);
    size_type counter{0};
    for (value_type element : m_table[index]) {
      if (key_equal{}(element, key)) {
        ++counter;
      }
    }
    return counter;
  }
  /*!
   * Finds the first occurrence of a element equal to "key", if there is one.
   * \param key element to look for.
   * \return iterator to the element, if it exists. Otherwise end().
   */
  iterator find(const_reference key) {
    size_type index = hash(key);
    for (auto runner = m_table[index].begin(); runner != m_table[index].end();
         ++runner) {
      if (key_equal{}(*runner, key)) {
        return iterator(m_table.begin() + index, runner);
      }
    }
    return end();
  }
  /*!
   * Finds the first occurrence of a element equal to "key", if there is one.
   * \param key element to look for.
   * \return const_iterator to the element, if it exists. Otherwise cend().
   */
  const_iterator find(const_reference key) const {
    size_type index = hash(key);
    for (auto runner = m_table[index].cbegin(); runner != m_table[index].cend();
         ++runner) {
      if (key_equal{}(*runner, key)) {
        return const_iterator(m_table.cbegin() + index, runner);
      }
    }
    return cend();
  }
  /*!
   * Checks if the container has a element equivalent to "key".
   * \param key element to look for.
   * \return flag that indicates whether the container has a element equivalent
   *         to "key".
   */
  bool contains(const_reference key) const {
    size_type index = hash(key);
    for (value_type element : m_table[index]) {
      if (key_equal{}(element, key)) {
        return true;
      }
    }
    return false;
  }

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
  /// Calculates the average number of elements per bucket
  float load_factor() const { return size() / (float)bucket_count(); }
  /// Returns the current max load factor.
  float max_load_factor() const { return m_max_load_factor; }
  /// Sets the maximum load factor to "load_factor".
  void max_load_factor(float load_factor) { m_max_load_factor = load_factor; }
  /*!
   * Changes the number of buckets to a value n that is not less than count and
   * satisfies n >= size() / max_load_factor(), then rehashes the container,
   * i.e. puts the elements into appropriate buckets.
   * \param count lower bound for the new number of buckets.
   */
  void rehash(size_type count) {
    size_type new_size = find_next_prime(count);
    while (new_size < size() / max_load_factor()) {
      new_size = find_next_prime(new_size);
    }
    HashTable new_hash_table(new_size);
    for (iterator runner = begin(); runner != end(); ++runner) {
      new_hash_table.insert(*runner);
    }
    m_table = std::move(new_hash_table);
  }
  /*!
   * Sets the number of buckets to the number needed to accommodate at least
   * count elements without exceeding maximum load factor and rehashes the
   * container, i.e. puts the elements into appropriate buckets
   * \param count lower bound for the new capacity of the container.
   */
  void reserve(size_type count) {
    rehash(std::ceil(count / max_load_factor()));
  }

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
