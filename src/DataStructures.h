#pragma once
#include "Structures.h"
#include <iostream>
#include <vector>
struct MinHeap {
  std::vector<int> heap;

  void push(int);
  int pop();

  void buildHeap();
  void printHeap();

private:
  void heapify(int nodeIndex);
};

struct HashMap {};
