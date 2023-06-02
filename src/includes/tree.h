#ifndef TREE_H
#define TREE_H

#include <cmath>   /* log operations */
#include <cstdlib> /* malloc */
#include <iostream>
#include <stdexcept> /* exceptions */
#include <stdint.h>  /* types */

#define MAX_SIZE 15

/**
 * @brief Class For Modeling Binary Tree
 * 
 * @tparam T Data Type of tree nodes
 */
template <typename T> class Tree {
private:
  const static int _MAX_SIZE = MAX_SIZE;

  T            *container = nullptr;
  uint16_t     size = _MAX_SIZE;
  uint8_t      levels;
  uint16_t     cur_index = 0;

public:
  Tree<T>(int size = Tree::_MAX_SIZE) {
    this->size = size; 
    container = (T *)malloc(sizeof(T) * this->size);
    for (auto i = 0; i < this->size; ++i) {
      container[i] = 0.0;
    }
  };

  /**
   * @brief Overloaded << operator to print tree to output stream
   * 
   * @param os Output stream (destination)
   * @param t  Tree Object 
   * @return std::ostream& 
   */
  friend std::ostream &operator<<(std::ostream &os, Tree &t) {
    os << t.get(0) << '\n';
    uint16_t  next_break = 2; /* next break line */ 
    uint8_t   row_count = 0;  /* current number of items in row */ 
    for (int i = 1; i < t.size; ++i) {
      os << t.get(i) << ' ';
      row_count++;
      if (row_count == next_break) {
        os << '\n';
        next_break *= 2;
        row_count = 0;
      }
    }
    return os;
  };

  /**
   * @brief Add item to the tree
   * 
   * @param item Data node item
   */
  void add(T item) {
    if (cur_index > this->size) {
      throw std::invalid_argument("Tree is full!");
    }
    container[cur_index++] = item;
    return;
  };

  /**
   * @brief Get item at index
   * 
   * @param index node index
   * @return T data item at node 
   */
  T get(uint16_t index) {
    if (index > this->size) {
      throw std::invalid_argument("Index out of bounds.");
    }
    return container[index];
  }
  
  /**
   * @brief Set node item at index
   * 
   * @param index Node index
   * @param item Data item
   */
  void set(uint16_t index, T item) {
    if (index > this->size) {
      printf("%d - %d\n", index, size);
      throw std::invalid_argument("[Tree-Set] Index out of bound");
    }
    container[index] = item;
  }

  /**
   * @brief Get the parent object
   * 
   * @param index node index
   * @return T Parent node data
   */
  T get_parent(uint16_t index) {
    if (index > this->size) {
      throw std::invalid_argument("Index out of bounds!");
    }

    if (index % 2 == 0) {
      return this->container[index / 2 - 1];
    } else {
      return this->container[index / 2];
    }
  }

  /**
   * @brief Get the children object
   * 
   * @param index parent index
   * @return std::pair<T, T> children node data
   */
  std::pair<T, T> get_children(uint16_t index) {
    if (floor(std::log2(index)) == floor(std::log2(this->size)) + 1) {
      throw std::invalid_argument("No children for leaf node");
    }
    std::pair<T, T> children;
    T left_child = this->get(2 * index);
    T right_child = this->get(2 * index + 1);
    children = std::make_pair(left_child, right_child);
    return children;
  }
};

#endif