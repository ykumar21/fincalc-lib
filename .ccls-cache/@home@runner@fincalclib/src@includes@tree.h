#ifndef TREE_H
#define TREE_H

#include <cmath>   /* log operations */
#include <cstdlib> /* malloc */
#include <iostream>
#include <stdexcept> /* exceptions */

#define MAX_SIZE 15

template <typename T> class Tree {
private:
  T *container;
  int max_size = MAX_SIZE;
  int cur_index = 0;

public:
  Tree<T>(int size = 2048) { container = (T *)malloc(sizeof(int) * size); };

  friend std::ostream &operator<<(std::ostream &os, Tree &t) {
    os << t.get(0) << '\n';
    int next_break = 2;
    int row_count = 0;
    for (int i = 1; i < t.max_size; ++i) {
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

  void add(T item) {
    if (cur_index == max_size) {
      throw std::invalid_argument("Tree is full!");
    }
    container[cur_index++] = item;
    return;
  };

  T get(int index) {
    if (index > max_size) {
      throw std::invalid_argument("Index out of bounds.");
    }
    return container[index];
  }
  void set(int index, T item) {
    if (index > max_size) {
      throw std::invalid_argument("[Tree-Set] Index out of bound");
    }
    container[index] = item;
  }

  T get_parent(int index) {
    if (index > max_size) {
      throw std::invalid_argument("Index out of bounds!");
    }

    if (index % 2 == 0) {
      return this->container[index / 2 - 1];
    } else {
      return this->container[index / 2];
    }
  }

  std::pair<T, T> get_children(int index) {
    if (floor(std::log2(index)) == floor(std::log2(max_size)) + 1) {
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