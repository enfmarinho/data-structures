#ifndef UNION_FIND_H
#define UNION_FIND_H

#include <cstddef> // size_t, ptrdiff_t
#include <utility> // move
#include <vector>

// Namespace for tree data-structures.
namespace tree {
template <typename T> class UnionFind {
public:
  //=== Aliases.
  using value_type = T;
  using pointer = value_type *;
  using const_pointer = const value_type *;
  using reference = value_type &;
  using const_reference = const value_type &;
  using size_type = size_t;
  using difference_type = std::ptrdiff_t;

  ///=== [I] Special Functions.
  UnionFind(size_type size) : m_size{size}, m_number_of_groups{size} {
    m_groups_id.resize(size, 0);
    m_groups_size.resize(size, 0);
    for (int value{0}; value < size; ++value) {
      m_groups_id[value] = value;
      m_groups_size[value] = 1;
    }
  }
  UnionFind(const UnionFind &other) { *this = other; }
  UnionFind(UnionFind &&other) { *this = std::move(other); }
  UnionFind &operator=(const UnionFind &other) {
    m_size = other.m_size;
    m_number_of_groups = other.m_number_of_groups;
    m_groups_id = other.m_groups_id;
    m_groups_size = other.m_groups_size;
    return *this;
  }
  UnionFind &operator=(UnionFind &&other) {
    m_size = std::move(other.m_size);
    m_number_of_groups = std::move(other.m_number_of_groups);
    m_groups_id = std::move(other.m_groups_id);
    m_groups_size = std::move(other.m_groups_size);
    return *this;
  }

  ///=== [II] Element Access.
  value_type find(value_type id) {
    value_type id_copy = id;
    while (id != m_groups_id[id]) {
      id = m_groups_id[id];
    }
    // Compressing path to assure amortized constant time complexity
    while (id_copy != m_groups_id[id_copy]) {
      value_type id_parent = m_groups_id[id_copy];
      m_groups_id[id_copy] = id;
      id_copy = id_parent;
    }
    return id;
  }

  ///=== [IV] Capacity.
  size_type size() const { return m_size; }
  size_type groups() const { return m_number_of_groups; }
  size_type group_size(const_reference id) const { return m_groups_size[id]; }

  ///=== [V] Modifiers.
  void unite(const_reference id1, const_reference id2) {
    id1 = find(id1);
    id2 = find(id2);
    if (id1 == id2) {
      return; // Elements are already in the same group.
    }
    // Assure id1 group is bigger than id2 group to make tree balanced.
    if (m_groups_size[id1] < m_groups_size[id2]) {
      std::swap(id1, id2);
    }
    m_groups_id[id2] = id1;
    m_groups_size[id1] += m_groups_size[id2];
    --m_number_of_groups;
  }

  ///=== [VI] Operations.
  bool connected(value_type id1, value_type id2) {
    return find(id1) == find(id2);
  }

private:
  size_type m_size{0};
  size_type m_number_of_groups;
  std::vector<value_type> m_groups_id;
  std::vector<value_type> m_groups_size;
};
} // namespace tree

#endif // UNION_FIND_H
