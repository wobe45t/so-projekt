#pragma once
#include "Tree.h"
#include "TreeState.h"
#include <vector>
#include <thread>
class Forester {
public:
  Forester(std::vector<Tree*> trees);
  void cycle();
private:
  bool running = {true}; 
  vector<Tree*> trees;
  std::thread td;
};