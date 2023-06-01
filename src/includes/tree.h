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
    if (index >= max_size) {
      throw std::invalid_argument("Index out of bounds.");
    }
    return container[index];
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
};

#endif