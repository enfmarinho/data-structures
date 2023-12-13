#include <cstddef>
#include <functional>
#include <initializer_list>
#include <utility>
template <typename T> class BST {
  //=== Aliases.
  using value_type = T;
  using pointer = value_type *;
  using const_pointer = const value_type *;
  using reference = value_type &;
  using const_reference = const value_type &;
  using size_type = size_t;
  using difference_type = std::ptrdiff_t;
  using compare_function = std::function<bool(value_type, value_type)>;

  struct Node {
    T key;
    Node *left_child;
    Node *right_child;
    Node *parent;

    explicit Node(T key, Node *parent = nullptr, Node *left_child = nullptr,
                  Node *right_child = nullptr) {
      this->key = key;
      this->left_child = left_child;
      this->right_child = right_child;
      this->parent = parent;
    }
  };

  class Iterator {
    Node *m_ptr;

  public:
    Iterator(Node *ptr = nullptr) { this->m_ptr = ptr; }
    Iterator(Iterator const &copy) { this->m_ptr = copy.m_ptr; }
    Node *get_parent() { return m_ptr->parent; }
    Iterator goto_right() {
      m_ptr = m_ptr->right_child;
      return Iterator(m_ptr);
    }
    Iterator goto_left() {
      m_ptr = m_ptr->left_child;
      return Iterator(m_ptr);
    }
    friend void goto_right(Iterator &it) { return it.goto_right(); }
    friend void goto_left(Iterator &it) { return it.goto_left(); }
    // TODO fix this.
    Iterator next(value_type key) {
      if (key > m_ptr->key) {
        m_ptr = m_ptr->right_child;
      } else {
        m_ptr = m_ptr->left_child;
      }
      return Iterator(m_ptr);
    }
    Iterator previous() {
      m_ptr = m_ptr->parent;
      return Iterator(m_ptr);
    }
    Iterator operator--() { return previous(); }
    Iterator operator--(int) {
      auto copy = m_ptr;
      previous();
      return Iterator(copy);
    }
    bool operator!=(Node *compare) { return m_ptr != compare; }
    bool operator==(Node *compare) { return m_ptr == compare; }
    reference operator*() { return m_ptr->key; }
    const_reference operator*() const { return m_ptr->key; }
    const_pointer operator->() const { return &(m_ptr->key); }
  };

  // TODO
  class ConstIterator {
    Node *m_ptr;

  public:
    explicit ConstIterator(Node *ptr = nullptr) { m_ptr = ptr; };
  };

  Iterator m_root;
  size_type m_size{0};

public:
  BST() = default;
  BST(std::initializer_list<value_type> ilist) { assing(ilist); }
  template <typename Itr> BST(Itr begin, Itr end) { assing(begin, end); }
  BST(const BST &clone) {
    clear();
    for (auto it = clone.begin(); it != clone.end(); ++it) {
      insert(*it);
    }
  }
  BST(BST &&source) noexcept {
    m_size = source.size();
    std::swap(m_root, source.m_root);
  }
  ~BST() { clear(); }

  [[nodiscard]] size_type size() const { return m_size; }

  //=== [II] MODIFIERS
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

  void insert(const_reference key) {
    Iterator it(m_root);
    Node *parent;
    while (it != nullptr) {
      parent = it.get_parent();
      it.next(key);
    }
    // TODO corrigir pai precisa apontar pro filho
    //    it = new Node(key, parent);
    //    ++m_size;
  }

  void erase(const value_type key) {
    Node *it{m_root};
    while (it != nullptr and it->key != key) {
      if (key > it->key) {
        it = it->right_child;
      } else {
        it = it->left_child;
      }
    }
    if (it != nullptr) {
      // TODO
      --m_size;
    }
  }

  // TODO
  void clear() {
    m_size = 0;
    if (m_root == nullptr) {
      return;
    }
  }
  //=== [III] ITERATORS.
  // TODO fix this. Its returning the root, however it should return the
  // smallest element.
  void begin() { return Iterator(m_root); }
  void end() {
    Iterator it(m_root), last_value;
    while (it != nullptr) {
      last_value = it;
      it.goto_right();
    }
    return last_value;
  }
  // TODO fix this.
  void cbegin() { return ConstIterator(m_root); }
  void cend() {
    ConstIterator it(m_root), last_value;
    while (it != nullptr) {
      last_value = it;
      it.goto_right();
    }
    return last_value;
  }

  Iterator search(const_reference key, compare_function compare,
                  std::function<void(Iterator &)> traverse_method) {
    Iterator it(m_root);
    while (it != nullptr) {
      if (compare(it, key)) {
        return Iterator(it);
      }
      traverse_method(it);
    }
    return end();
  }

  Iterator upper_bound(const T &key) {
    return search(key, std::greater<T>(), Iterator::goto_right());
  }
  Iterator lower_bound(const T &key) {
    return search(key, std::greater_equal<T>(), Iterator::goto_right());
  }
  Iterator first_smaller(const_reference key) {
    return search(key, std::less<T>(), Iterator::goto_left());
  }

  //=== [VI] OPERATORS
  // TODO
};
