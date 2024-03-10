#ifndef BST_H
#define BST_H

#include <cstddef> // size_t, ptrdiff_t
#include <initializer_list>
#include <limits>  // numeric_limits
#include <utility> // swap

// Namespace for tree data-structures.
namespace tree {
/*!
 * Binary search tree data structure. Important: not self balancing, just a
 * simple implementation.
 * \template T data type to store.
 *
 * \author Eduardo Marinho (eduardo.nestor.marinho228@gmail.com)
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
  /// Construct an empty binary search tree.
  BST() = default;
  /*!
   * Construct a binary search tree containing the elements presents in "ilist".
   * \param ilist initializer_list with elements to insert in the tree.
   */
  BST(std::initializer_list<value_type> ilist) {
    clear();
    for (value_type value : ilist) {
      insert(value);
    }
  }
  /*!
   * Construct a binary search tree containing the elements presents in the
   * range [begin, end).
   * \param begin beginning of the range to insert.
   * \param end end of the range to insert(not included).
   */
  template <typename Itr> BST(Itr begin, Itr end) {
    clear();
    for (; begin != end; ++begin) {
      insert(*begin);
    }
  }
  /*!
   * Construct a clone of the binary search tree "other".
   * \param clone binary search tree to be cloned.
   */
  BST(BST &clone) {
    for (iterator runner = clone.begin(); runner != clone.end(); ++runner) {
      insert(*runner);
    }
  }
  /*!
   * Construct a binary search tree that takes ownership of memory from
   * "source".
   * \param source binary search tree to take memory from.
   */
  BST(BST &&source) noexcept {
    std::swap(m_size, source.m_size);
    std::swap(m_root, source.m_root);
    std::swap(m_end, source.m_end);
    std::swap(m_smallest, source.m_smallest);
  }
  /// Destructs container, deallocating its memory.
  ~BST() { clear(); }

  //=== [II] ITERATORS.
  /// Returns a iterator to the beginning of the container.
  iterator begin() { return iterator(m_smallest); }
  /// Returns a iterator to the end of the container.
  iterator end() { return iterator(m_end); }

  //=== [III] Capacity.
  /// Consults whether or not the container is empty.
  [[nodiscard]] bool empty() const { return m_size == 0; }
  /// Consults the number of elements in the container.
  [[nodiscard]] size_type size() const { return m_size; }

  //=== [IV] MODIFIERS
  /// Removes all elements in the container, making it empty.
  void clear() {
    if (m_root == nullptr) {
      return;
    }
    clear_helper(&m_root);
    m_root = nullptr;
    m_end = nullptr;
    m_smallest = nullptr;
    m_size = 0;
  }
  /*!
   * Inserts the element "value" in the container.
   * \param value data to insert.
   * \return iterator pointing to inserted data.
   */
  iterator insert(const_reference value) {
    ++m_size;
    iterator runner(m_root);
    iterator parent;
    while (runner != nullptr) {
      parent = runner;
      runner.next(value);
    }
    node_pointer new_node = new Node(value, &parent);
    if (parent == nullptr) { // tree empty
      m_root = new_node;
      new_node->right_child =
          new Node(std::numeric_limits<value_type>::max(), new_node);
    } else if (value < *parent) {
      (&parent)->left_child = new_node;
    } else {
      (&parent)->right_child = new_node;
    }

    if (m_end == nullptr and m_smallest == nullptr) {
      m_end = new_node->right_child;
      m_smallest = new_node;
    } else if (value >= *m_end) {
      m_end = new_node;
    } else if (value <= *m_smallest) {
      m_smallest = new_node;
    }
    return iterator(new_node);
  }
  /*!
   * Removes the first occurrence of a element equivalent to "key", if there are
   * any, otherwise does nothing. Uses the operator==().
   * \param key element to remove.
   * \return boolean representing whether or not a element was removed.
   */
  bool erase(const_reference key) {
    iterator runner{m_root};
    while (runner != nullptr and (&runner)->key != key) {
      runner.next(key);
    }
    return erase(runner);
  }
  /*!
   * Removes the element being pointed by the iterator "it".
   * \param it iterator pointing to the element to remove.
   * \return boolean representing whether or not a element was removed.
   */
  bool erase(iterator it) {
    if (it == nullptr or it == m_end) {
      return false;
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
      if (*it < parent->key) {
        parent->left_child = save;
      } else {
        parent->right_child = save;
      }
    }
    --m_size;
    if (m_size == 0) {
      m_root = m_smallest = m_end;
    }
    delete &it;
    return true;
  }

  class iterator {
  public:
    //=== Aliases.
    using iterator_reference = iterator &;

    /// Make this iterator points to "ptr" or nullptr if no pointer is provided.
    iterator(node_pointer ptr = nullptr) { this->m_pointer = ptr; }
    /// Makes this iterator points to the same address "copy" does.
    iterator(const iterator &copy) { this->m_pointer = copy.m_pointer; }
    /// Default destructor.
    ~iterator() = default;
    /// Makes this iterator points to the same address of "it";
    iterator &operator=(const iterator &it) = default;
    /// Makes this iterator points to the address "pointer".
    iterator &operator=(Node *pointer) {
      m_pointer = pointer;
      return *this;
    }
    /// Constant dereference operator.
    const_reference operator*() const { return m_pointer->key; }
    /// Consults the pointer to the node.
    node_pointer operator&() {
      // TODO this function should be removed as it breaks the encapsulation
      return m_pointer;
    }
    /// Consults the pointer to the node.
    const_node_pointer operator&() const { return m_pointer; }
    /// Arrow operator.
    pointer operator->() { return &(m_pointer->key); }
    /// Constant arrow operator.
    const_pointer operator->() const { return &(m_pointer->key); }
    /// Returns iterator pointing to the node parent.
    iterator get_parent() { return iterator(m_pointer->parent); }
    /// Goes in the direction of the key.
    iterator next(const_reference key) {
      if (key < m_pointer->key) {
        m_pointer = m_pointer->left_child;
      } else {
        m_pointer = m_pointer->right_child;
      }
      return iterator(m_pointer);
    }
    /// Goes to next node. If there is no next node, does nothing.
    iterator operator++() {
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
    /// Goes to the next node. If there is no next node, does nothing.
    iterator operator++(int) {
      Node *copy = m_pointer;
      ++(*this);
      return iterator(copy);
    }
    /// Goes to previous node. If there is no previous node, does nothing.
    iterator operator--() {
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
    /// Checks whether the underlying pointer of the iterator "lhs" is
    /// equivalent to "rhs".
    friend bool operator==(const iterator &lhs, node_pointer rhs) {
      return lhs.m_pointer == rhs;
    }
    /// Checks whether two iterators are different.
    friend bool operator!=(const iterator &lhs, const iterator &rhs) {
      return !(lhs == rhs);
    }
    /// Checks whether the underlying pointer of the iterator "lhs" is
    /// different to "rhs".
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

  private:
    node_pointer m_pointer; //!< Pointer to a node.
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

  size_type m_size{0}; //!< Number of elements in the tree.
  iterator m_root;     //!< Iterator to the tree root.
  iterator m_smallest; //!< Iterator to smallest element in the tree.
  iterator m_end;      //!< Iterator to the end of the tree.
};
} // namespace tree

#endif // BST_H
