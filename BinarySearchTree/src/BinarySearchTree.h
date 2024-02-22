#ifndef BST_H
#define BST_H

#include <cstddef>    // size_t, ptrdiff_t
#include <functional> // greater<>, greater_equal<>
#include <initializer_list>
#include <utility> // swap()

// Namespace for tree data-structures.
namespace tree {
/*!
 * TODO document.
 */
template <typename T> class BST {
public:
  // Forward declaration.
  class iterator;
  struct Node;
  //=== Aliases.
  using value_type = T;
  using pointer = value_type *;
  using const_pointer = const value_type *;
  using reference = value_type &;
  using const_reference = const value_type &;
  using size_type = size_t;
  using difference_type = std::ptrdiff_t;
  using compare_function = std::function<bool(value_type, value_type)>;
  using node_pointer = Node *;
  using const_node_pointer = const Node *;

  struct Node {
    T key;             //!< Data stored
    Node *left_child;  //!< Pointer to the left child.
    Node *right_child; //!< Pointer to the right child.
    Node *parent;      //!< Pointer to the parent node.
    explicit Node(T key, Node *parent = nullptr, Node *left_child = nullptr,
                  Node *right_child = nullptr) {
      this->key = key;
      this->left_child = left_child;
      this->right_child = right_child;
      this->parent = parent;
    }
  };

public:
  //=== [I] Special Functions.
  BST() = default;
  BST(std::initializer_list<value_type> ilist) { assing(ilist); }
  template <typename Itr> BST(Itr begin, Itr end) { assing(begin, end); }
  BST(const BST &clone) {
    for (auto runner = clone.begin(); runner != clone.end(); ++runner) {
      insert(*runner);
    }
  }
  BST(BST &&source) noexcept {
    m_size = source.size();
    std::swap(m_root, source.m_root);
    std::swap(m_greatest, source.m_greatest);
    std::swap(m_smallest, source.m_smallest);
  }
  ~BST() { clear(); }
  template <typename Itr> void assing(Itr begin, Itr end) {
    clear();
    for (; begin != end; ++begin) {
      insert(*begin);
    }
  }

  void assing(std::initializer_list<value_type> ilist) {
    clear();
    for (auto value : ilist) {
      insert(value);
    }
  }

  //=== [II] ITERATORS.
  iterator begin() { return iterator(m_smallest); }
  iterator end() { return iterator(m_greatest); }

  //=== [III] Capacity.
  [[nodiscard]] bool empty() const { return m_size == 0; }
  [[nodiscard]] size_type size() const { return m_size; }

  //=== [IV] MODIFIERS
  /// Removes all elements in the container.
  // TODO
  void clear() {
    if (m_root == nullptr) {
      return;
    }
    iterator runner = begin();
    iterator end = end();
    while (runner != end) {
      iterator copy = runner + 1;
      delete &runner;
      runner = copy;
    }
    m_root = nullptr;
    m_size = 0;
  }
  /*!
   * Inserts a node with the "value" data in the container.
   * \param value data to insert.
   * \return iterator pointing to inserted data.
   */
  // TODO check border cases.
  iterator insert(const_reference value) {
    iterator runner(m_root);
    node_pointer parent = nullptr;
    while (runner != nullptr) {
      parent = runner.get_parent();
      runner.next(value);
    }
    node_pointer new_node = new Node(value, parent);
    if (parent == nullptr) { // tree empty
      m_root = new_node;
    } else if (value < *parent) {
      parent->left_child = new_node;
    } else {
      parent->right_child = new_node;
    }

    if (m_greatest != nullptr and value > *m_greatest) {
      m_greatest = new_node;
    } else if (m_smallest != nullptr and value < *m_smallest) {
      m_smallest = new_node;
    }
    return iterator(new_node);
  }
  /*!
   * Removes all elements in the container that are equivalent to "key".
   * \param key element to remove.
   */
  void erase(const_reference key) {
    iterator it{m_root};
    while (it != nullptr and it->key != key) {
      it.next(key);
    }
    if (it != nullptr) {
      iterator parent = it.get_parent();
      while (it != nullptr and it == key) {
      }
      // Guarantee that all nodes equivalent to key are deleted. Not only one.
    }
  }

  //=== [V] Lookup.
  size_type count(const_reference key) {}
  /*!
   * Finds the first occurrence of the elements "key" in the container.
   * \param key element to look for.
   * \return iterator pointing to the element, if it was found or end().
   */
  iterator find(const_reference key) {}
  iterator search(const_reference key, compare_function compare,
                  std::function<void(iterator &)> traverse_method) {
    iterator it(m_root);
    while (it != nullptr) {
      if (compare(it, key)) {
        return iterator(it);
      }
      traverse_method(it);
    }
    return end();
  }

  iterator upper_bound(const T &key) {
    return search(key, std::greater<T>(), iterator::goto_right());
  }
  iterator lower_bound(const T &key) {
    return search(key, std::greater_equal<T>(), iterator::goto_right());
  }
  iterator first_smaller(const_reference key) {
    return search(key, std::less<T>(), iterator::goto_left());
  }

  class iterator {
  public:
    //=== Aliases.
    using iterator_reference = iterator &;

    /// Default constructor.
    iterator(Node *ptr = nullptr) { this->m_pointer = ptr; }
    /// Copy constructor.
    iterator(const iterator &copy) { this->m_pointer = copy.m_pointer; }
    /// Destructor.
    ~iterator() = default;
    /// Dereference operator.
    reference operator*() { return m_pointer->key; }
    /// Constant dereference operator.
    const_reference operator*() const { return m_pointer->key; }
    /// Consults the pointer to the node.
    node_pointer operator&() { return m_pointer; }
    /// Consults the pointer to the node.
    const_node_pointer operator&() const { return m_pointer; }
    /// Arrow operator.
    pointer operator->() { return &(m_pointer->key); }
    /// Constant arrow operator.
    const_pointer operator->() const { return &(m_pointer->key); }
    /// Consults node parent.
    node_pointer *get_parent() { return m_pointer->parent; }
    /// Goes to right node.
    iterator goto_right() {
      m_pointer = m_pointer->right_child;
      return iterator(m_pointer);
    }
    /// Goes to left node.
    iterator goto_left() {
      m_pointer = m_pointer->left_child;
      return iterator(m_pointer);
    }
    /// Goes to the parent node.
    iterator goto_parent() {
      m_pointer = m_pointer->parent;
      return iterator(m_pointer);
    }
    /// Goes to the right node.
    friend void goto_right(iterator &it) { return it.goto_right(); }
    /// Goes to left node.
    friend void goto_left(iterator &it) { return it.goto_left(); }
    /// Goes in the direction of the key.
    iterator next(const_reference key) {
      if (key < m_pointer->key) {
        goto_left();
      } else {
        goto_right();
      }
    }
    /// Goes to next node.
    iterator next() {
      if (m_pointer->right_child != nullptr) {
        goto_right();
      } else {
        node_pointer parrent = get_parent();
        while (parrent != nullptr) {
          if (m_pointer == parrent->left_child) {
            goto_parent();
            break;
          }
          goto_parent();
        }
      }
    }
    /// Goes to previous node.
    iterator previous() {}
    /// Goes to the next node.
    iterator operator++() { return next(); }
    /// Goes to the next node.
    iterator operator++(int) { return next(); }
    /// Goes to previous node.
    iterator operator--() { return previous(); }
    /// Goes to previous node and return a copy of this before operation.
    iterator operator--(int) {
      auto copy = m_pointer;
      previous();
      return iterator(copy);
    }
    /// Checks whether two iterators are different.
    bool operator!=(iterator_reference other) {
      return m_pointer != other.m_pointer;
    }
    /// Checks whether two iterators are equivalent.
    bool operator==(iterator_reference other) {
      return m_pointer == other.m_pointer;
    }

  private:
    Node *m_pointer; //!< Pointer to a node.
  };

private:
  size_type m_size{0}; //!< Number of elements in the tree.
  iterator m_root;     //!< Iterator to the tree root.
  iterator m_smallest; //!< Iterator to smallest element in the tree.
  iterator m_greatest; //!< Iterator to greatest element in the tree.
};
} // namespace tree

#endif // BST_H
