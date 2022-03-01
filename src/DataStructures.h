#pragma once
#include "Structures.h"
#include <functional>
#include <iostream>
#include <vector>
template <typename T> struct MinHeap {
private:
  void heapify(int nodeIndex) {
    size_t leftChild, rightChild, smallar;

    leftChild = nodeIndex * 2;
    rightChild = leftChild + 1;
    smallar = leftChild;

    if (leftChild > heap.size())
      return;

    if (rightChild <= heap.size()) {
      // Right child exists
      if (heap[rightChild - 1] < heap[leftChild - 1])
        smallar = rightChild;
    }
    if (heap[nodeIndex - 1] > heap[smallar - 1]) {
      std::swap(heap[nodeIndex - 1], heap[smallar - 1]);
      heapify(smallar);
    }
  }
  std::function<bool(T, T)> comparator;

public:
  MinHeap(std::function<bool(T, T)> comp) : comparator(comp) {}

  std::vector<T> heap;

  void push(T n) {
    heap.push_back(n);

    if (heap.size() == 1)
      return;
    /*
     * Heap uses weird indexes
     * Array start at 1 so that
     * So that left child is at i * 2
     * Right Child is at i * 2 + 1
     */

    size_t i = heap.size(); // Child
    size_t j = i / 2;       // Parent
    while (true) {
      if (!comparator(heap[i - 1], heap[j - 1])) {
        std::swap(heap[i - 1], heap[j - 1]);
        i = j;
        j = j / 2;
        if (i == 1)
          break;
      } else
        break;
    }
  }

  T pop() {
    T n = heap[0];
    std::swap(heap[0], heap[heap.size() - 1]);
    heap.pop_back();

    /*
     * Heap uses weird indexes
     * Array start at 1 so that
     * So that left child is at i * 2
     * Right Child is at i * 2 + 1
     */
    size_t i = 1;
    while (true) {
      size_t leftChild = i * 2;          // Left
      size_t rightChild = leftChild + 1; // Added
      size_t smallar = leftChild;
      if (leftChild > heap.size())
        break;

      if (rightChild < heap.size() - 1) {
        // Right child exists
        if (!comparator(heap[rightChild - 1], heap[leftChild - 1]))
          smallar = rightChild;
      }
      if (comparator(heap[i - 1], heap[smallar - 1])) {
        std::swap(heap[i - 1], heap[smallar - 1]);
        i = smallar;
      } else
        break;
    }

    return n;
  }
  T top() { return heap[0]; }

  bool empty() { return heap.size() == 0; }

  void buildHeap() {
    for (size_t i = heap.size() / 2 + 2; i > 0; i--) {
      heapify(i - 1);
    }
  }
};

template <typename Key, typename Value> struct HashMap {
  struct Node {
    Key k;
    Value v;
    Node *next;

    Node(Key k, Node *next = nullptr) : k(k), next(next) {}
  };

private:
  int size = 19;
  Node *keys[19];

public:
  HashMap() {
    for (int i = 0; i < size; i++) {
      keys[i] = nullptr;
    }
  }
  ~HashMap() {
    for (int i = 0; i < size; i++) {
      Node *n = keys[i];
      Node *nn;
      while (n != nullptr) {
        nn = n;
        n = n->next;
        delete nn;
      }
    }
  }
  Value operator[](Key k) const {
    size_t hashValue = std::hash<Key>{}(k);
    int index = hashValue % size;

    Node *temp = keys[index];
    while (temp != nullptr) {
      if (temp->k == k)
        return temp->v;
      temp = temp->next;
    }
    throw std::out_of_range("Oh no");
  }
  Value &operator[](Key k) {
    size_t hashValue = std::hash<Key>{}(k);
    int index = hashValue % size;

    Node *newNode;
    Node *temp = keys[index];
    if (temp) {
      if (temp->k == k) {
        return temp->v;
      };

      for (Node *temp = keys[index]; temp->next != nullptr; temp = temp->next)
        if (temp->next->k == k) {
          return temp->next->v;
        };

      newNode = new Node(k);
      newNode->next = keys[index];
      keys[index] = newNode;
    } else {
      newNode = new Node(k);
      keys[index] = newNode;
    }
    return newNode->v;
  }

  Value at(Key k) {
    size_t hashValue = std::hash<Key>{}(k);
    int index = hashValue % size;

    Node *temp = keys[index];
    while (temp != nullptr) {
      if (temp->k == k)
        return temp->v;
      temp = temp->next;
    }
    throw std::out_of_range("Oh no");
  }
};
