#ifndef SRC_INCLUDE_REDBLACKTREE_REDBLACKTREEFATNODES_H_
#define SRC_INCLUDE_REDBLACKTREE_REDBLACKTREEFATNODES_H_

#include <cstddef> // size_t, ptrdiff_t
#include <initializer_list>
#include <utility> // swap, move

// Namespace for tree data-structures.
namespace tree {
/*!
 * Red black tree, i.e. a self-balancing binary search tree. Important: allows
 * duplicate elements by the use of fat nodes.
 * \tparam T data type to store.
 *
 * \author Eduardo Marinho (eduardo.nestor.marinho228@gmail.com)
 */
template <typename T> class RedBlackTree {
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
  using node_pointer = Node *;
  using const_node_pointer = const Node *;

  struct Node {
    Node(value_type data, bool black, Node *parent = nullptr,
         Node *right_child = nullptr, Node *left_child = nullptr)
        : data(data), black(black), parent(parent), right_child(right_child),
          left_child(left_child), counter(1) {}

    value_type data;
    Node *right_child;
    Node *left_child;
    Node *parent;
    bool black;
    size_type counter;
  };

  ///=== [I] Special Functions.
  /// Default constructor.
  RedBlackTree() = default;
  /*!
   * Construct a red black tree containing the elements presents in "ilist".
   * \param ilist initializer_list with elements to insert in the tree.
   */
  RedBlackTree(const std::initializer_list<value_type> &ilist) {
    for (value_type value : ilist) {
      insert(value);
    }
  }
  /*!
   * Construct a red black tree containing the elements presents in the range
   * [begin, end).
   * \param begin beginning of the range to insert.
   * \param end end of the range to insert(not included).
   */
  template <typename InputIt> RedBlackTree(InputIt begin, InputIt end) {
    for (; begin != end; ++begin) {
      insert(*begin);
    }
  }
  /*!
   * Construct a clone of the red black tree "other".
   * \param clone red black tree to be cloned.
   */
  RedBlackTree(RedBlackTree &other) {
    for (value_type value : other) {
      insert(value);
    }
  }
  /*!
   * Construct a red black tree that takes ownership of memory from "source".
   * \param source red black tree to take memory from.
   */
  RedBlackTree(RedBlackTree &&other) {
    std::swap(m_size, other.m_size);
    std::swap(m_root, other.m_root);
    std::swap(m_smallest, other.m_smallest);
    std::swap(m_end, other.m_end);
  }
  /// Destructs the container, deallocating its memory.
  ~RedBlackTree() { clear(); }

  ///=== [II] Iterators.
  /// Returns a iterator to the beginning of the container.
  iterator begin() { return iterator(m_smallest); }
  /// Returns a iterator to the end of the container.
  iterator end() { return iterator(m_end); }

  ///=== [III] Capacity.
  /// Consults whether or not the container is empty.
  bool empty() const { return m_size == 0; }
  /// Consults the number of elements in the container.
  size_type size() const { return m_size; }

  ///=== [IV] Modifiers.
  /// Removes all elements in the container, making it empty.
  void clear() {
    if (m_root == nullptr) {
      return;
    }
    clear_helper(&m_root);
    m_root = nullptr;
    m_smallest = nullptr;
    m_end = nullptr;
    m_size = 0;
  }
  /*!
   * Inserts the element "value" in the container.
   * \param value data to insert.
   * \return iterator pointing to inserted data.
   */
  iterator insert(value_type value) {
    iterator it = find(value);
    if (it != end()) {
      ++(&it)->counter;
      return it;
    }
    // TODO
    iterator runner = m_root;
    Node *parent = nullptr;
    while (runner != nullptr) {
      parent = (&runner)->parent;
      runner.next(value);
    }
    return m_root; // TODO delete this, just a stub;
  }
  /*!
   * Removes the first occurrence of a element equivalent to "key", if there are
   * any, otherwise does nothing. Uses the operator==().
   * \param key element to remove.
   * \return boolean representing whether or not a element was removed.
   */
  bool erase(value_type key) {
    iterator it = find(key);
    if (it == end()) {
      return false;
    }
    return erase(it);
  }
  /*!
   * Removes the element being pointed by the iterator "it".
   * \param it iterator pointing to the element to remove.
   * \return boolean representing whether or not a element was removed.
   */
  bool erase(iterator it) {
    // TODO
    if (it == nullptr or it == m_end) {
      return false;
    }

    --m_size;
    if (it.count() > 1) {
      --(&it)->counter;
      return true;
    }

    if (it == m_smallest) {
      if ((&m_smallest)->right_child != nullptr) {
        m_smallest = (&m_smallest)->right_child;
        while ((&m_smallest)->left_child != nullptr) {
          m_smallest = (&m_smallest)->left_child;
        }
      } else {
        m_smallest = it.get_parent();
      }
    }

    Node *parent = &it.get_parent();
    Node *substitute = (&it)->right_child;
    Node *save = nullptr;
    if (substitute != nullptr and substitute->left_child == nullptr) {
      save = substitute;
      substitute->parent = parent;
      substitute->left_child = (&it)->left_child;
      if ((&it)->left_child != nullptr) {
        (&it)->left_child->parent = substitute;
      }
    } else if (substitute != nullptr) {
      while (substitute->left_child != nullptr) {
        substitute = substitute->left_child;
      }
      if (substitute->right_child != nullptr) {
        substitute->right_child->parent = substitute->parent;
        substitute->parent->left_child = substitute->right_child;
      }
      substitute->right_child = (&it)->right_child;
      substitute->left_child = (&it)->left_child;
      if ((&it)->right_child != nullptr) {
        (&it)->right_child->parent = substitute;
      }
      if ((&it)->left_child != nullptr) {
        (&it)->left_child->parent = substitute;
      }
      save = substitute;
    } else if (parent == nullptr) {
      m_root = (&it)->left_child;
      if ((&it)->left_child != nullptr) {
        (&it)->left_child->parent = nullptr;
      } else {
        m_smallest = m_end;
      }
    } else if ((&it)->left_child != nullptr) {
      save = (&it)->left_child;
      (&it)->left_child->parent = parent;
    }

    if (parent != nullptr) {
      if (*it < parent->data) {
        parent->left_child = save;
      } else {
        parent->right_child = save;
      }
    }
    if (m_size == 0) {
      m_smallest = m_end;
      m_root = nullptr;
    }
    delete &it;
    return true;
  }

  ///=== [VI] Lookup.
  /*!
   * Counts the number of elements matching "key". Uses operator==(),
   * operator<() and operator>().
   * \param key element to look for.
   * \return number of elements that matches "key".
   */
  size_type count(const_reference key) {
    iterator it = find(key);
    if (it == end()) {
      return 0;
    } else {
      return it.count();
    }
  }
  /*!
   * Returns an iterator pointing to the first occurrence of element equivalent
   * to "key" or end() if there is none. Uses operator==(), operator<() and
   * operator>().
   * \param key element to look for.
   * \return iterator pointing to element or end(), if not found.
   */
  iterator find(const_reference key) {
    iterator runner(m_root);
    while (runner != nullptr) {
      if (*runner < key) {
        runner = (&runner)->right_child;
      } else if (*runner > key) {
        runner = (&runner)->left_child;
      } else {
        return runner;
      }
    }
    return end();
  }
  /*!
   * Returns an iterator pointing to the first element not less than "key".
   * \param key value to compare elements to.
   * \return iterator pointing to the first element not less than "key" or
   *         end(), if there is none.
   */
  iterator lower_bound(const_reference key) {
    iterator candidate = end();
    iterator runner(m_root);
    while (runner != nullptr) {
      if (*runner >= key) {
        candidate = runner;
        runner = (&runner)->left_child;
      } else {
        runner = (&runner)->right_child;
      }
    }
    return candidate;
  }
  /*!
   * Returns an iterator pointing to the first element greater than "key".
   * \param key value to compare elements to.
   * \return iterator pointing to the first element greater than "key" or end(),
   *         if there is none.
   */
  iterator upper_bound(const_reference key) {
    iterator candidate = end();
    iterator runner(m_root);
    while (runner != nullptr) {
      if (*runner > key) {
        candidate = runner;
        runner = (&runner)->left_child;
      } else {
        runner = (&runner)->right_child;
      }
    }
    return candidate;
  }

  class iterator {
  public:
    /// Default constructor.
    iterator(node_pointer ptr = nullptr) { this->m_pointer = ptr; }
    /// Copy constructor.
    iterator(const iterator &copy) { this->m_pointer = copy.m_pointer; }
    /// Destructor.
    ~iterator() = default;
    /// Makes this iterator points to the same element that "it" does.
    iterator &operator=(const iterator &it) = default;
    /// Makes this iterator points to the element at the address "pointer".
    iterator &operator=(Node *pointer) {
      m_pointer = pointer;
      return *this;
    }
    /// Constant dereference operator.
    const_reference operator*() const { return m_pointer->data; }
    /// Consults the pointer to the node.
    node_pointer operator&() {
      // TODO this function should be removed as it breaks the encapsulation
      return m_pointer;
    }
    /// Consults the pointer to the node.
    const_node_pointer operator&() const { return m_pointer; }
    /// Arrow operator.
    pointer operator->() { return &(m_pointer->data); }
    /// Constant arrow operator.
    const_pointer operator->() const { return &(m_pointer->data); }
    /// Consults node parent.
    iterator get_parent() { return iterator(m_pointer->parent); }
    size_type count() const { return m_pointer->counter; }
    /// Goes in the direction of the key.
    iterator next(const_reference value) {
      if (value < m_pointer->data) {
        m_pointer = m_pointer->left_child;
      } else {
        m_pointer = m_pointer->right_child;
      }
      return iterator(m_pointer);
    }
    /// Goes to next node. If there is no next node, does nothing.
    iterator operator++() {
      if (m_counter < m_pointer->counter) {
        ++m_counter;
        return iterator(m_pointer);
      }
      if (m_pointer->right_child != nullptr) {
        m_pointer = m_pointer->right_child;
        while (m_pointer->left_child != nullptr) {
          m_pointer = m_pointer->left_child;
        }
      } else {
        Node *parent = m_pointer->parent;
        Node *runner = m_pointer;
        while (parent != nullptr) {
          if (runner == parent->left_child) {
            m_pointer = parent;
            break;
          }
          runner = runner->parent;
          parent = runner->parent;
        }
      }
      return iterator(m_pointer);
    }
    /// Goes to the next node.
    iterator operator++(int) {
      Node *copy = m_pointer;
      ++(*this);
      return iterator(copy);
    }
    /// Goes to previous node. If there is no previous node, does nothing.
    iterator operator--() {
      if (m_counter > 1) {
        --m_counter;
        return iterator(m_pointer);
      }
      if (m_pointer->left_child != nullptr) {
        m_pointer = m_pointer->left_child;
        while (m_pointer->right_child != nullptr) {
          m_pointer = m_pointer->right_child;
        }
      } else {
        Node *parent = m_pointer->parent;
        Node *runner = m_pointer;
        while (parent != nullptr) {
          if (runner == parent->right_child) {
            m_pointer = parent;
            break;
          }
          runner = runner->parent;
          parent = runner->parent;
        }
      }
      return iterator(m_pointer);
    }
    /// Goes to previous node and return a copy of this before operation.
    iterator operator--(int) {
      auto copy = m_pointer;
      ++(*this);
      return iterator(copy);
    }
    /// Checks whether two iterators are equivalent.
    friend bool operator==(const iterator &lhs, const iterator &rhs) {
      return lhs.m_pointer == rhs.m_pointer;
    }
    friend bool operator==(const iterator &lhs, node_pointer rhs) {
      return lhs.m_pointer == rhs;
    }
    /// Checks whether two iterators are different.
    friend bool operator!=(const iterator &lhs, const iterator &rhs) {
      return !(lhs == rhs);
    }
    friend bool operator!=(iterator lhs, node_pointer pointer) {
      return !(lhs == pointer);
    }
    friend iterator operator+(iterator it, difference_type increment) {
      for (difference_type counter{0}; counter < increment; ++counter) {
        ++it;
      }
      for (difference_type counter{0}; counter > increment; --counter) {
        --it;
      }
      return it;
    }
    friend iterator operator+(difference_type increment, const iterator &it) {
      return it + increment;
    }
    friend iterator operator-(const iterator &it, difference_type decrement) {
      return it + (-decrement);
    }

  protected:
    Node *m_pointer;        //!< Pointer to the node.
    size_type m_counter{1}; //!< Counter of the node.
  };

private:
  void clear_helper(node_pointer node) {
    if (node->left_child != nullptr) {
      clear_helper(node->left_child);
    }
    if (node->right_child != nullptr) {
      clear_helper(node->right_child);
    }
    delete node;
  }
  void rotate() {
    // TODO
  }

  size_type m_size{0}; //!< Number of elements in the tree.
  iterator m_root;     //!< Iterator to the tree root.
  iterator m_smallest; //!< Iterator to smallest element in the tree.
  iterator m_end;      //!< Iterator to the end of the tree.
};
} // namespace tree

#endif // SRC_INCLUDE_REDBLACKTREE_REDBLACKTREEFATNODES_H_
