#ifndef UNION_FIND_H
#define UNION_FIND_H

#include <cstddef> // size_t, ptrdiff_t
#include <utility> // move
#include <vector>

// Namespace for tree data-structures.
namespace tree {
/*!
 * Implementation of a union find data structure. Stores a collection of
 * non-overlapping sets.
 *
 * \author Eduardo Marinho (eduardo.nestor.marinho228@gmail.com)
 */
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
  /*!
   * Creates a container with "size" elements and "size" groups, each element
   * belonging to its own group.
   */
  UnionFind(size_type size) : m_size{size}, m_number_of_groups{size} {
    m_group_id.resize(size, 0);
    m_groups_size.resize(size, 0);
    for (int value{0}; value < size; ++value) {
      m_group_id[value] = value;
      m_groups_size[value] = 1;
    }
  }
  /// Makes this container be equivalent to "other" by coping its data.
  UnionFind(const UnionFind &other) { *this = other; }
  /// Makes this container take ownership of memory of "other".
  UnionFind(UnionFind &&other) { *this = std::move(other); }
  /*!
   * Makes this container be equivalent to "other" by coping its data.
   * \param other container to copy.
   * \return reference to this container.
   */
  UnionFind &operator=(const UnionFind &other) {
    m_size = other.m_size;
    m_number_of_groups = other.m_number_of_groups;
    m_group_id = other.m_group_id;
    m_groups_size = other.m_groups_size;
    return *this;
  }
  /*!
   * Makes this container be equivalent to "other" by taking ownership of its
   * memory. "other" becomes invalid.
   * \param other container to take memory from.
   * \return reference to this container.
   */
  UnionFind &operator=(UnionFind &&other) {
    m_size = std::move(other.m_size);
    m_number_of_groups = std::move(other.m_number_of_groups);
    m_group_id = std::move(other.m_group_id);
    m_groups_size = std::move(other.m_groups_size);
    return *this;
  }

  ///=== [II] Element Access.
  /// Consults the group identifier of the element "e".
  value_type find(value_type e) {
    value_type id_copy = e;
    while (e != m_group_id[e]) {
      e = m_group_id[e];
    }
    // Compressing path to assure amortized constant time complexity
    while (id_copy != m_group_id[id_copy]) {
      value_type id_parent = m_group_id[id_copy];
      m_group_id[id_copy] = e;
      id_copy = id_parent;
    }
    return e;
  }

  ///=== [IV] Capacity.
  /// Consults the number of elements in the container.
  size_type size() const { return m_size; }
  /// Consults the number of different groups in the container.
  size_type groups() const { return m_number_of_groups; }
  /// Consults the size of the group under the identifier "id".
  size_type group_size(const_reference id) const { return m_groups_size[id]; }

  ///=== [V] Modifiers.
  /*!
   * Unites the groups under the "id1" and "id2" identifiers. If already
   * belonging to the same group, does nothing.
   * \param id1 id of the group to join.
   * \param id2 id of the other group to join.
   */
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
    m_group_id[id2] = id1;
    m_groups_size[id1] += m_groups_size[id2];
    --m_number_of_groups;
  }

  ///=== [VI] Operations.
  /// Checks whether "e1" and "e2" belongs to the same group.
  bool connected(value_type e1, value_type e2) { return find(e1) == find(e2); }

private:
  size_type m_size{0};                //!< Number of elements in the container.
  size_type m_number_of_groups;       //!< Number of groups in the container.
  std::vector<value_type> m_group_id; //!< Group id of each element.
  std::vector<value_type> m_groups_size; //!< Sizes of each group
};
} // namespace tree

#endif // UNION_FIND_H
