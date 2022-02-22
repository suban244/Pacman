#include "DataStructures.h"

void MinHeap::push(int n) {
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
    if (heap[i - 1] < heap[j - 1]) {
      std::swap(heap[i - 1], heap[j - 1]);
      i = j;
      j = j / 2;
      if (i == 1)
        break;
    } else
      break;
  }
}

int MinHeap::pop() {
  int n = heap[0];
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
      if (heap[rightChild - 1] < heap[leftChild - 1])
        smallar = rightChild;
    }
    if (heap[i - 1] > heap[smallar - 1]) {
      std::swap(heap[i - 1], heap[smallar - 1]);
      i = smallar;
    } else
      break;
  }

  return n;
}

void MinHeap::printHeap() {
  for (size_t i = 0; i < heap.size(); i++) {
    std::cout << heap[i] << " ";
  }
  std::cout << std::endl;
}

void MinHeap::buildHeap() {
  for (size_t i = heap.size() / 2; i >= 0; i--) {
    heapify(i);
  }
}

void MinHeap::heapify(int nodeIndex) {
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
